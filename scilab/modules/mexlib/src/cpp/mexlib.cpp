/*
 *  Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2011-2011 - Gsoc 2011 - Iuri SILVIO
 *  Copyright (C) 2011-2011 - DIGITEO - Bruno JOFRET
 *  Copyright (C) 2011 - DIGITEO - Antoine ELIAS
 *
 *  Copyright (C) 2012 - 2016 - Scilab Enterprises
 *  Copyright (C) 2017 - Gsoc 2017 - Siddhartha Gairola
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */

/*------------------------------------------------------------------------
 *    mexlib  library
 *
 *    This library emulates Matlab' API functions. It is not fully tested...
 *    -Assumes that Scilab string matrices have one column, e.g.
 *    Str=["qwerty";"123456"]; here this is a 2 x 6 matrix but Scilab
 *    considers Str as a 2 x 1 matrix. ["123";"1234"] is a valid string
 *    matrix which cannot be used here.
 *    -Assumes that sparse matrices have been converted into the Matlab
 *    format. Scilab sparse matrices are stored in the transposed Matlab
 *    format.
  --------------------------------------------------------------------------*/
#include <stdarg.h>

#include <limits>
#include <list>

#include "scilabWrite.hxx"
#include "context.hxx"
#include "symbol.hxx"
#include "parser.hxx"
#include "configvariable.hxx"
#include "overload.hxx"
#include "printvisitor.hxx"

#include "types.hxx"
#include "int.hxx"
#include "double.hxx"
#include "bool.hxx"
#include "string.hxx"
#include "struct.hxx"
#include "container.hxx"
#include "cell.hxx"
#include "sparse.hxx"
#include "localization.hxx"

extern "C"
{
#include "sci_malloc.h"
#include "machine.h"
#include "mex.h"
#include "os_string.h"
#include "freeArrayOfString.h"
}

//#ifdef getType
//#undef getType
//#endif
//
//#ifdef isComplex
//#undef isComplex
//#endif

static void (*exitFcn)(void);

static int mexCallSCILAB(int nlhs, mxArray **plhs, int nrhs, mxArray **prhs, const char *name)
{
    wchar_t* pwst = to_wide_string(name);
    symbol::Context *context = symbol::Context::getInstance();
    symbol::Symbol *symbol = new symbol::Symbol(pwst);
    FREE(pwst);

    types::InternalType *value = context->get(*symbol);
    delete symbol;
    types::Function *func = value->getAs<types::Function>();
    if (func == NULL)
    {
        return 1;
    }

    types::typed_list in;
    types::typed_list out;
    types::optional_list opt;
    for (int i = 0; i < nrhs; i++)
    {
        in.push_back((types::InternalType*)prhs[i]->ptr);
    }

    func->call(in, opt, nlhs, out);

    for (int i = 0; i < nlhs; i++)
    {
        plhs[i] = new mxArray;
        plhs[i]->ptr = (int*) (out[i]);
    }
    return 0;
}

//Validated

//Create or Delete Array
mxArray *mxCreateDoubleMatrix(int m, int n, mxComplexity complexFlag)
{
    types::Double *ptr = new types::Double(m, n, complexFlag == mxCOMPLEX);
    mxArray* ret = new mxArray;
    ret->ptr = (int*)ptr;
    return ret;
}

mxArray *mxCreateDoubleScalar(double value)
{
    mxArray *ptr = mxCreateDoubleMatrix(1, 1, mxREAL);
    ((types::Double *)ptr->ptr)->set(0, value);
    return ptr;
}

mxArray *mxCreateNumericMatrix(int m, int n, mxClassID CLASS, mxComplexity complexFlag)
{
    int dims[2] = {m, n};
    return mxCreateNumericArray(2, dims, CLASS, complexFlag);
}

mxArray *mxCreateNumericArray(int ndim, const int *dims, mxClassID CLASS, mxComplexity complexFlag)
{
    types::GenericType *ptr;

    switch (CLASS)
    {
        case mxDOUBLE_CLASS:
            ptr = new types::Double(ndim, (int *)dims, complexFlag == mxCOMPLEX);
            ((types::Double *)ptr)->fillDefaultValues();
            break;
        case mxINT8_CLASS:
            ptr = new types::Int8(ndim, (int *)dims);
            ((types::Int8 *)ptr)->fillDefaultValues();
            break;
        case mxUINT8_CLASS:
            ptr = new types::UInt8(ndim, (int *)dims);
            ((types::UInt8 *)ptr)->fillDefaultValues();
            break;
        case mxINT16_CLASS:
            ptr = new types::Int16(ndim, (int *)dims);
            ((types::Int16 *)ptr)->fillDefaultValues();
            break;
        case mxUINT16_CLASS:
            ptr = new types::UInt16(ndim, (int *)dims);
            ((types::UInt16 *)ptr)->fillDefaultValues();
            break;
        case mxINT32_CLASS:
            ptr = new types::Int32(ndim, (int *)dims);
            ((types::Int32 *)ptr)->fillDefaultValues();
            break;
        case mxUINT32_CLASS:
            ptr = new types::UInt32(ndim, (int *)dims);
            ((types::UInt32 *)ptr)->fillDefaultValues();
            break;
        case mxINT64_CLASS:
            ptr = new types::Int64(ndim, (int *)dims);
            ((types::Int64 *)ptr)->fillDefaultValues();
            break;
        case mxUINT64_CLASS:
            ptr = new types::UInt64(ndim, (int *)dims);
            ((types::UInt64 *)ptr)->fillDefaultValues();
            break;
        default:
            ptr = NULL;
    }

    if (ptr == NULL)
    {
        return NULL;
    }

    mxArray* ret = new mxArray;
    ret->ptr = (int*)ptr;
    return ret;
}

mxArray *mxCreateUninitNumericMatrix(size_t m, size_t n, mxClassID classid, mxComplexity ComplexFlag)
{
    int dims[2] = {(int)m, (int)n};
    return mxCreateUninitNumericArray(2, (size_t *)dims, classid, ComplexFlag);
}

mxArray *mxCreateUninitNumericArray(size_t ndim, size_t *dims, mxClassID classid, mxComplexity ComplexFlag)
{
    types::GenericType *ptr;

    switch (classid)
    {
        case mxDOUBLE_CLASS:
            ptr = new types::Double((int)ndim, (int *)dims, ComplexFlag == mxCOMPLEX);
            break;
        case mxINT8_CLASS:
            ptr = new types::Int8((int)ndim, (int *)dims);
            break;
        case mxUINT8_CLASS:
            ptr = new types::UInt8((int)ndim, (int *)dims);
            break;
        case mxINT16_CLASS:
            ptr = new types::Int16((int)ndim, (int *)dims);
            break;
        case mxUINT16_CLASS:
            ptr = new types::UInt16((int)ndim, (int *)dims);
            break;
        case mxINT32_CLASS:
            ptr = new types::Int32((int)ndim, (int *)dims);
            break;
        case mxUINT32_CLASS:
            ptr = new types::UInt32((int)ndim, (int *)dims);
            break;
        case mxINT64_CLASS:
            ptr = new types::Int64((int)ndim, (int *)dims);
            break;
        case mxUINT64_CLASS:
            ptr = new types::UInt64((int)ndim, (int *)dims);
            break;
        default:
            ptr = NULL;
    }

    if (ptr == NULL)
    {
        return NULL;
    }

    mxArray* ret = new mxArray;
    ret->ptr = (int*)ptr;
    return ret;
}

mxArray *mxCreateString(const char *string)
{
    types::String *ptr = new types::String(string);
    mxArray* ret = new mxArray;
    ret->ptr = (int*)ptr;
    return ret;
}

mxArray *mxCreateCharMatrixFromStrings(int m, const char **str)
{
    int n = 1;
    wchar_t** strings = NULL;
    strings = (wchar_t**)MALLOC(sizeof(wchar_t*) * m);
    for (int k = 0; k < m; k++)
    {
        strings[k] = to_wide_string(str[k]);
    }

    types::String *ptr = new types::String(m, n, strings);
    freeArrayOfWideString(strings, m);
    mxArray* ret = new mxArray;
    ret->ptr = (int*)ptr;
    return ret;
}

mxArray *mxCreateCharArray(int ndim, const int *dims)
{
    if (ndim == 0 || ndim == 1)
    {
        ndim = 2;
    }

    types::String *ptr = new types::String(ndim, (int *)dims);
    int size = ptr->getSize();
    for (int i = 0; i < size; ++i)
    {
        ptr->set(i, L"");
    }
    mxArray* ret = new mxArray;
    ret->ptr = (int*)ptr;
    return ret;
}

mxArray *mxCreateLogicalScalar(mxLogical value)
{
    mxArray *ptr = mxCreateLogicalMatrix(1, 1);

    ((types::Bool *)ptr->ptr)->set(0, value);
    return ptr;
}

mxArray *mxCreateLogicalMatrix(int m, int n)
{
    types::Bool *ptr = new types::Bool(m, n);
    mxArray* ret = new mxArray;
    ret->ptr = (int*)ptr;
    return ret;
}

mxArray *mxCreateLogicalArray(int ndim, const int *dims)
{
    types::Bool *ptr = new types::Bool(ndim, (int *)dims);
    mxArray* ret = new mxArray;
    ret->ptr = (int*)ptr;
    return ret;
}

mxArray *mxCreateSparseLogicalMatrix(mwSize m, mwSize n, mwSize nzmax)
{
    types::SparseBool* ptr = new types::SparseBool(m, n);
    mxArray* ret = new mxArray;
    ret->ptr = (int*)ptr;
    return ret;
}

mxArray *mxCreateSparse(int m, int n, int nzmax, mxComplexity cmplx)
{
    types::Sparse* ptr = new types::Sparse(m, n, cmplx == mxCOMPLEX);
    mxArray* ret = new mxArray;
    ret->ptr = (int*)ptr;
    return ret;
}

mxArray *mxCreateStructMatrix(int m, int n, int nfields, const char **field_names)
{
    int dims[2] = {m, n};
    return mxCreateStructArray(2, dims, nfields, field_names);
}

mxArray *mxCreateStructArray(int ndim, const int *dims, int nfields, const char **field_names)
{
    types::Struct *ptr = new types::Struct(ndim, (int *)dims);
    for (int i = 0; i < nfields; i++)
    {
        wchar_t *name = to_wide_string(field_names[i]);
        ptr->addField(name);
        FREE(name);
    }
    mxArray* ret = new mxArray;
    ret->ptr = (int*)ptr;
    return ret;
}

mxArray *mxCreateCellArray(int ndim, const int *dims)
{
    types::Cell *ptr = new types::Cell(ndim, (int *)dims);
    mxArray* ret = new mxArray;
    ret->ptr = (int*)ptr;
    return ret;
}

mxArray *mxCreateCellMatrix(int m, int n)
{
    int dims[2] = {m, n};
    return mxCreateCellArray(2, dims);
}

void mxDestroyArray(mxArray *ptr)
{
    if (mxGetClassID(ptr) != mxUNKNOWN_CLASS)
    {
        types::InternalType* pIT = (types::InternalType*)ptr->ptr;
        pIT->killMe();
    }

    delete ptr;
}

mxArray *mxDuplicateArray(const mxArray *ptr)
{
    types::InternalType *pIT = (types::InternalType *)ptr->ptr;
    if (pIT == NULL)
    {
        return 0;
    }

    mxArray* ret = new mxArray;
    ret->ptr = (int*)pIT->clone();
    return ret;
}

void *mxCalloc(size_t n, size_t size)
{
    //TODO
    return CALLOC(n, size);
}

void *mxMalloc(size_t nsize)
{
    //TODO
    return MALLOC(nsize);
}

void *mxRealloc(void *ptr, size_t nsize)
{
    //TODO
    return REALLOC(ptr, nsize);
}

void mxFree(void *ptr)
{
    FREE(ptr);
    ptr = NULL;
}

//Validate Data
int mxIsDouble(const mxArray *ptr)
{
    return mxGetClassID(ptr) == mxDOUBLE_CLASS;
}

int mxIsSingle(const mxArray *ptr)
{
    return mxGetClassID(ptr) == mxSINGLE_CLASS;
}

int mxIsComplex(const mxArray *ptr)
{
    types::InternalType *pIT = (types::InternalType *)ptr->ptr;
    if (pIT == NULL || pIT->isGenericType() == false)
    {
        return 0;
    }

    types::GenericType *pGT = pIT->getAs<types::GenericType>();

    return pGT->isComplex() ? 1 : 0;
}

int mxIsNumeric(const mxArray *ptr)
{
    return mxIsDouble(ptr) || mxIsSingle(ptr) ||
           mxIsInt8(ptr) || mxIsUint8(ptr) ||
           mxIsInt16(ptr) || mxIsUint16(ptr) || mxIsInt32(ptr) || mxIsUint32(ptr) || mxIsInt64(ptr) || mxIsUint64(ptr);
}

int mxIsInt64(const mxArray *ptr)
{
    return mxGetClassID(ptr) == mxINT64_CLASS;
}

int mxIsUint64(const mxArray *ptr)
{
    return mxGetClassID(ptr) == mxUINT64_CLASS;
}

int mxIsInt32(const mxArray *ptr)
{
    return mxGetClassID(ptr) == mxINT32_CLASS;
}

int mxIsUint32(const mxArray *ptr)
{
    return mxGetClassID(ptr) == mxUINT32_CLASS;
}

int mxIsInt16(const mxArray *ptr)
{
    return mxGetClassID(ptr) == mxINT16_CLASS;
}

int mxIsUint16(const mxArray *ptr)
{
    return mxGetClassID(ptr) == mxUINT16_CLASS;
}

int mxIsInt8(const mxArray *ptr)
{
    return mxGetClassID(ptr) == mxINT8_CLASS;
}

int mxIsUint8(const mxArray *ptr)
{
    return mxGetClassID(ptr) == mxUINT8_CLASS;
}

int mxIsScalar(const mxArray *array_ptr)
{

    types::InternalType *pIT = (types::InternalType *)array_ptr->ptr;

    if (pIT == NULL || pIT->isGenericType() == false)
    {
        return 0;
    }

    types::GenericType *pGT = pIT->getAs<types::GenericType>();

    if( pGT->isScalar() == true)
        return 1;
    else
        return 0;
}

int mxIsChar(const mxArray *ptr)
{
    return mxGetClassID(ptr) == mxCHAR_CLASS;
}

int mxIsLogical(const mxArray *ptr)
{
    return mxGetClassID(ptr) == mxLOGICAL_CLASS;
}

int mxIsLogicalScalar(const mxArray *ptr)
{
    return mxIsLogical(ptr) && mxGetNumberOfElements(ptr) == 1;
}

int mxIsLogicalScalarTrue(const mxArray *ptr)
{
    if (mxIsLogicalScalar(ptr) == false)
    {
        return 0;
    }

    if (*mxGetLogicals(ptr) == 0)
    {
        return 0;
    }

    return 1;
}

int mxIsStruct(const mxArray *ptr)
{
    return mxGetClassID(ptr) == mxSTRUCT_CLASS;
}

int mxIsCell(const mxArray *ptr)
{
    return mxGetClassID(ptr) == mxCELL_CLASS;
}

int mxIsClass(const mxArray *ptr, const char *name)
{
    if (strcmp(name, "cell") == 0)
    {
        return mxIsCell(ptr);
    }
    if (strcmp(name, "char") == 0)
    {
        return mxIsChar(ptr);
    }
    if (strcmp(name, "double") == 0)
    {
        return mxIsDouble(ptr);
    }
    if (strcmp(name, "int8") == 0)
    {
        return mxIsInt8(ptr);
    }
    if (strcmp(name, "int16") == 0)
    {
        return mxIsInt16(ptr);
    }
    if (strcmp(name, "int32") == 0)
    {
        return mxIsInt32(ptr);
    }
    if (strcmp(name, "int64") == 0)
    {
        return mxIsInt64(ptr);
    }
    if (strcmp(name, "logical") == 0)
    {
        return mxIsLogical(ptr);
    }
    if (strcmp(name, "single") == 0)
    {
        return mxIsSingle(ptr);
    }
    if (strcmp(name, "struct") == 0)
    {
        return mxIsStruct(ptr);
    }
    if (strcmp(name, "uint8") == 0)
    {
        return mxIsUint8(ptr);
    }
    if (strcmp(name, "uint16") == 0)
    {
        return mxIsUint16(ptr);
    }
    if (strcmp(name, "uint32") == 0)
    {
        return mxIsUint32(ptr);
    }
    if (strcmp(name, "uint64") == 0)
    {
        return mxIsUint64(ptr);
    }
    // TODO: how to handle <class_name> and <class_id>?
    return 0;
}

int mxIsInf(double x)
{
    if (x == x + 1)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

int mxIsFinite(double x)
{
    if (x < x + 1)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

int mxIsNaN(double x)
{
    if (x != x)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

int mxIsEmpty(const mxArray *ptr)
{
    types::InternalType * pIT = (types::InternalType *)ptr->ptr;
    if (pIT == NULL)
    {
        //true or false, whatever ;)
        return 1;
    }

    switch (pIT->getType())
    {
        case types::InternalType::ScilabDouble:
        {
            types::Double *pD = pIT->getAs<types::Double>();
            return pD->getSize() == 0;
        }
        case types::InternalType::ScilabCell:
        {
            types::Cell *pC = pIT->getAs<types::Cell>();
            return pC->getSize() == 0;
        }
        case types::InternalType::ScilabContainer:
        case types::InternalType::ScilabList:
        case types::InternalType::ScilabMList:
        case types::InternalType::ScilabTList:
        {
            types::Container *pC = pIT->getAs<types::Container>();
            return pC->getSize() == 0;
        }
        default:
        {
            //other type can not be empty
            return 0;
        }
    }
}

int mxIsSparse(const mxArray *ptr)
{
    types::InternalType *pIT = (types::InternalType *)ptr->ptr;
    if (pIT == NULL || pIT->isGenericType() == false)
    {
        return 0;
    }

    types::GenericType *pGT = pIT->getAs<types::GenericType>();

    if (pGT->isSparse() == true || pGT->isSparseBool() == true)
        return 1;
    else
        return 0;
}

int mxIsFromGlobalWS(const mxArray *pm)
{
    //TODO
    return 0;
}

//Convert Data Types
char *mxArrayToString(const mxArray *ptr)
{
    if (!mxIsChar(ptr))
    {
        return (char *)0;
    }

    types::String *pa = (types::String *)ptr->ptr;
    int items = mxGetM(ptr);
    int index = 0;
    int length = 1; // one extra char to \0
    wchar_t **wstrings = pa->get();
    for (int k = 0; k < items; k++)
    {
        length += (int)wcslen(wstrings[k]);
    }

    char *str = (char *)malloc(sizeof(char) * length);
    for (int k = 0; k < items; k++)
    {
        char *dest = wide_string_to_UTF8(wstrings[k]);
        int dest_length = strlen(dest);
        memcpy(str + index, dest, dest_length);
        index += dest_length;
        FREE(dest);
    }

    str[index] = '\0';
    return str;
}

char *mxArrayToUTF8String(const mxArray *array_ptr)
{
    return mxArrayToString(array_ptr);
}

int mxGetString(const mxArray *ptr, char *str, int strl)
{
    if (!mxIsChar(ptr))
    {
        return 1;
    }

    types::String *pa = (types::String *)ptr->ptr;
    int items = mxGetM(ptr);
    int index = 0;
    int free_space = strl - 1;
    for (int k = 0; k < items; k++)
    {
        wchar_t *to_copy = pa->get(k);
        char *dest = wide_string_to_UTF8(to_copy);
        int length = (int)strlen(dest);
        memcpy(str + index, dest, free_space);
        index += std::min(length, free_space);
        free_space -= length;
        FREE(dest);
        if (free_space <= 0)
        {
            break;
        }
    }

    str[index] = '\0';
    return free_space >= 0 ? 0 : 1;
}

int mxSetClassName(mxArray *array_ptr, const char *classname)
{
    //TODO
    return 0;
}

int mxGetNumberOfDimensions(const mxArray *ptr)
{
    types::InternalType *pIT = (types::InternalType *)ptr->ptr;
    if (pIT == NULL)
    {
        return 0;
    }

    types::GenericType *pGT = pIT->getAs<types::GenericType>();
    if (pGT == NULL)
    {
        //InternalType but not GenericType, so mono dimension type.
        return 1;
    }

    return pGT->getDims();
}

int mxGetElementSize(const mxArray *ptr)
{
    if (mxIsChar(ptr))
    {
        return sizeof(wchar_t*);
    }
    else if (mxIsLogical(ptr))
    {
        return sizeof(int);
    }
    else if (mxIsDouble(ptr))
    {
        return sizeof(double);
    }
    else if (mxIsSparse(ptr))
    {
        return sizeof(double);
    }
    else if (mxIsInt8(ptr))
    {
        return sizeof(char);
    }
    else if (mxIsInt16(ptr))
    {
        return sizeof(short);
    }
    else if (mxIsInt32(ptr))
    {
        return sizeof(int);
    }
    else if (mxIsInt64(ptr))
    {
        return sizeof(long long);
    }
    else if (mxIsUint8(ptr))
    {
        return sizeof(unsigned char);
    }
    else if (mxIsUint16(ptr))
    {
        return sizeof(unsigned short);
    }
    else if (mxIsUint32(ptr))
    {
        return sizeof(unsigned int);
    }
    else if (mxIsUint64(ptr))
    {
        return sizeof(unsigned long long);
    }
    else if (mxIsCell(ptr))
    {
        return sizeof(types::InternalType*);
    }
    else if (mxIsStruct(ptr))
    {
        return sizeof(types::SingleStruct*);
    }
    return 0;
}

mwSize *mxGetDimensions(const mxArray *ptr)
{
    types::InternalType *pIT = (types::InternalType *)ptr->ptr;
    if (pIT == NULL)
    {
        return NULL;
    }

    switch (pIT->getType())
    {
        case types::InternalType::ScilabList:
        case types::InternalType::ScilabMList:
        case types::InternalType::ScilabTList:
        {
            int *piDims = (int *)MALLOC(sizeof(int));

            piDims[0] = pIT->getAs<types::Container>()->getSize();
            return piDims;
        }
        default:
        {
            if(pIT->isGenericType() == false)
            {
               return NULL;
            }
            types::GenericType *pGT = pIT->getAs<types::GenericType>();

            return pGT->getDimsArray();
        }
    }
    return NULL;
}

int mxSetDimensions(mxArray *array_ptr, const int *dims, int ndim)
{
    if (mxIsCell(array_ptr))
    {
        ((types::Cell *)array_ptr->ptr)->resize((int *)dims, ndim);
    }
    else if (mxIsChar(array_ptr))
    {
        ((types::String *)array_ptr->ptr)->resize((int *)dims, ndim);
    }
    else if (mxIsDouble(array_ptr))
    {
        ((types::Double *)array_ptr->ptr)->resize((int *)dims, ndim);
    }
    else if (mxIsSparse(array_ptr))
    {
        if (ndim == 1)
        {
            ((types::Sparse*)array_ptr->ptr)->resize(dims[0], 1);
        }
        else
        {
            ((types::Sparse*)array_ptr->ptr)->resize(dims[0], dims[1]);
        }
        ((types::Sparse *)array_ptr->ptr)->reshape((int *)dims, ndim);
    }
    else if (mxIsInt8(array_ptr))
    {
        ((types::Int8 *)array_ptr->ptr)->resize((int *)dims, ndim);
    }
    else if (mxIsInt16(array_ptr))
    {
        ((types::Int16 *)array_ptr->ptr)->resize((int *)dims, ndim);
    }
    else if (mxIsInt32(array_ptr))
    {
        ((types::Int32 *)array_ptr->ptr)->resize((int *)dims, ndim);
    }
    else if (mxIsInt64(array_ptr))
    {
        ((types::Int64 *)array_ptr->ptr)->resize((int *)dims, ndim);
    }
    else if (mxIsLogical(array_ptr))
    {
        ((types::Bool *)array_ptr->ptr)->resize((int *)dims, ndim);
    }
    else if (mxIsStruct(array_ptr))
    {
        ((types::Struct *)array_ptr->ptr)->resize((int *)dims, ndim);
    }
    else if (mxIsUint8(array_ptr))
    {
        ((types::UInt8 *)array_ptr->ptr)->resize((int *)dims, ndim);
    }
    else if (mxIsUint16(array_ptr))
    {
        ((types::UInt16 *)array_ptr->ptr)->resize((int *)dims, ndim);
    }
    else if (mxIsUint32(array_ptr))
    {
        ((types::UInt32 *)array_ptr->ptr)->resize((int *)dims, ndim);
    }
    else if (mxIsUint64(array_ptr))
    {
        ((types::UInt64 *)array_ptr->ptr)->resize((int *)dims, ndim);
    }

    return 0;
}

int mxGetNumberOfElements(const mxArray *ptr)
{
    types::InternalType *pIT = (types::InternalType *)ptr->ptr;
    if (pIT == NULL || pIT->isGenericType() == false)
    {
        return 0;
    }

    types::GenericType *pGT = dynamic_cast<types::GenericType *>(pIT);

    return pGT->getSize();
}

int mxCalcSingleSubscript(const mxArray *ptr, int nsubs, const int *subs)
{
    int index = 0;
    int iMult = 1;
    mwSize *dims = mxGetDimensions(ptr);
    for (int i = 0; i < nsubs; i++)
    {
        index += subs[i] * iMult;
        iMult *= dims[i];
    }
    return index;
}

int mxGetM(const mxArray *ptr)
{
    types::InternalType *pIT = (types::InternalType *)ptr->ptr;
    if (pIT == NULL || pIT->isGenericType() == false)
    {
        return 0;
    }

    types::GenericType *pGT = pIT->getAs<types::GenericType>();
    return pGT->getRows();
}

void mxSetM(mxArray *ptr, int M)
{
    types::InternalType *pIT = (types::InternalType *)ptr->ptr;
    if (pIT == NULL || pIT->isGenericType() == false)
    {
        return;
    }

    types::GenericType *pGT = pIT->getAs<types::GenericType>();

    types::InternalType* res = pGT->resize(M, pGT->getCols());
    ptr->ptr = (int*)res;
}

int mxGetN(const mxArray *ptr)
{
    types::InternalType * pIT = (types::InternalType *)ptr->ptr;
    if (pIT == NULL || pIT->isGenericType() == false)
    {
        return 0;
    }

    types::GenericType * pGT = pIT->getAs<types::GenericType>();
    return pGT->getCols();
}

void mxSetN(mxArray *ptr, int N)
{
    types::InternalType * pIT = (types::InternalType *)ptr->ptr;
    if (pIT == NULL || pIT->isGenericType() == false)
    {
        return;
    }

    types::GenericType * pGT = pIT->getAs<types::GenericType>();

    types::InternalType* res = pGT->resize(pGT->getRows(), N);
    ptr->ptr = (int*)res;
}

double mxGetScalar(const mxArray *ptr)
{
    // TODO: review spec
    types::InternalType *pIT = (types::InternalType *)ptr->ptr;
    if (pIT == NULL)
    {
        return 0;
    }

    switch (pIT->getType())
    {
        case types::InternalType::ScilabDouble:
        {
            types::Double *pD = pIT->getAs<types::Double>();
            return pD->get(0);
        }
        case types::InternalType::ScilabBool:
        {
            types::Bool *pB = pIT->getAs<types::Bool>();
            return (double)pB->get(0);
        }
        case types::InternalType::ScilabInt8:
        {
            types::Int8 *pI = pIT->getAs<types::Int8>();
            return (double)pI->get(0);
        }
        case types::InternalType::ScilabUInt8:
        {
            types::UInt8 *pI = pIT->getAs<types::UInt8>();
            return (double)pI->get(0);
        }
        case types::InternalType::ScilabInt16:
        {
            types::Int16 *pI = pIT->getAs<types::Int16>();
            return (double)pI->get(0);
        }
        case types::InternalType::ScilabUInt16:
        {
            types::UInt16 *pI = pIT->getAs<types::UInt16>();
            return (double)pI->get(0);
        }
        case types::InternalType::ScilabInt32:
        {
            types::Int32 *pI = pIT->getAs<types::Int32>();
            return (double)pI->get(0);
        }
        case types::InternalType::ScilabUInt32:
        {
            types::UInt32 *pI = pIT->getAs<types::UInt32>();
            return (double)pI->get(0);
        }
        case types::InternalType::ScilabInt64:
        {
            types::Int64 *pI = pIT->getAs<types::Int64>();
            return (double)pI->get(0);
        }
        case types::InternalType::ScilabUInt64:
        {
            types::UInt64 *pI = pIT->getAs<types::UInt64>();
            return (double)pI->get(0);
        }
        default:
            return 0;
    }
}

double *mxGetPr(const mxArray *ptr)
{
    types::InternalType *pIT = (types::InternalType *)ptr->ptr;
    if (pIT == NULL || pIT->isDouble() == false)
    {
        return NULL;
    }

    types::Double *pD = pIT->getAs<types::Double>();
    return pD->get();
}

void mxSetPr(mxArray *ptr, double *pr)
{
    ((types::Double *)ptr->ptr)->set(pr);
}

double *mxGetPi(const mxArray *ptr)
{
    return ((types::Double *)ptr->ptr)->getImg();
}

void mxSetPi(mxArray *ptr, double *pi)
{
    ((types::Double *)ptr->ptr)->setImg(pi);
}

void *mxGetData(const mxArray *ptr)
{
    types::InternalType *pIT = (types::InternalType *)ptr->ptr;
    if (pIT == NULL)
    {
        return NULL;
    }

    switch (pIT->getType())
    {
        case types::InternalType::ScilabDouble:
        {
            types::Double *pD = pIT->getAs<types::Double>();
            return pD->get();
        }
        case types::InternalType::ScilabBool:
        {
            types::Bool *pB = pIT->getAs<types::Bool>();
            return pB->get();
        }
        case types::InternalType::ScilabInt8:
        {
            types::Int8 *pI = pIT->getAs<types::Int8>();
            return pI->get();
        }
        case types::InternalType::ScilabUInt8:
        {
            types::UInt8 *pI = pIT->getAs<types::UInt8>();
            return pI->get();
        }
        case types::InternalType::ScilabInt16:
        {
            types::Int16 *pI = pIT->getAs<types::Int16>();
            return pI->get();
        }
        case types::InternalType::ScilabUInt16:
        {
            types::UInt16 *pI = pIT->getAs<types::UInt16>();
            return pI->get();
        }
        case types::InternalType::ScilabInt32:
        {
            types::Int32 *pI = pIT->getAs<types::Int32>();
            return pI->get();
        }
        case types::InternalType::ScilabUInt32:
        {
            types::UInt32 *pI = pIT->getAs<types::UInt32>();
            return pI->get();
        }
        case types::InternalType::ScilabInt64:
        {
            types::Int64 *pI = pIT->getAs<types::Int64>();
            return pI->get();
        }
        case types::InternalType::ScilabUInt64:
        {
            types::UInt64 *pI = pIT->getAs<types::UInt64>();
            return pI->get();
        }
        default:
            return NULL;
    }
}

void mxSetData(mxArray *array_ptr, void *data_ptr)
{
    if (mxIsChar(array_ptr))
    {
        array_ptr->ptr = (int*)((types::String *)array_ptr->ptr)->set((wchar_t **)data_ptr);
    }
    else if (mxIsDouble(array_ptr))
    {
        array_ptr->ptr = (int*)((types::Double *)array_ptr->ptr)->set((double *)data_ptr);
    }
    else if (mxIsInt8(array_ptr))
    {
        array_ptr->ptr = (int*)((types::Int8 *)array_ptr->ptr)->set((char *)data_ptr);
    }
    else if (mxIsInt16(array_ptr))
    {
        array_ptr->ptr = (int*)((types::Int16 *)array_ptr->ptr)->set((short *)data_ptr);
    }
    else if (mxIsInt32(array_ptr))
    {
        array_ptr->ptr = (int*)((types::Int32 *)array_ptr->ptr)->set((int *)data_ptr);
    }
    else if (mxIsInt64(array_ptr))
    {
        array_ptr->ptr = (int*)((types::Int64 *)array_ptr->ptr)->set((long long *)data_ptr);
    }
    else if (mxIsLogical(array_ptr))
    {
        array_ptr->ptr = (int*)((types::Bool *)array_ptr->ptr)->set((int *)data_ptr);
    }
    else if (mxIsUint8(array_ptr))
    {
        array_ptr->ptr = (int*)((types::UInt8 *)array_ptr->ptr)->set((unsigned char *)data_ptr);
    }
    else if (mxIsUint16(array_ptr))
    {
        array_ptr->ptr = (int*)((types::UInt16 *)array_ptr->ptr)->set((unsigned short *)data_ptr);
    }
    else if (mxIsUint32(array_ptr))
    {
        array_ptr->ptr = (int*)((types::UInt32 *)array_ptr->ptr)->set((unsigned int *)data_ptr);
    }
    else if (mxIsUint64(array_ptr))
    {
        array_ptr->ptr = (int*)((types::UInt64 *)array_ptr->ptr)->set((unsigned long long *) data_ptr);
    }
}

void *mxGetImagData(const mxArray *ptr)
{
    types::InternalType *pIT = (types::InternalType *)ptr->ptr;
    if (pIT == NULL)
    {
        return NULL;
    }

    switch (pIT->getType())
    {
        case types::InternalType::ScilabDouble:
        {
            types::Double *pD = pIT->getAs<types::Double>();
            return pD->getImg();
        }
        case types::InternalType::ScilabBool:
        {
            types::Bool *pB = pIT->getAs<types::Bool>();
            return pB->getImg();
        }
        case types::InternalType::ScilabInt8:
        {
            types::Int8 *pI = pIT->getAs<types::Int8>();
            return pI->getImg();
        }
        case types::InternalType::ScilabUInt8:
        {
            types::UInt8 *pI = pIT->getAs<types::UInt8>();
            return pI->getImg();
        }
        case types::InternalType::ScilabInt16:
        {
            types::Int16 *pI = pIT->getAs<types::Int16>();
            return pI->getImg();
        }
        case types::InternalType::ScilabUInt16:
        {
            types::UInt16 *pI = pIT->getAs<types::UInt16>();
            return pI->getImg();
        }
        case types::InternalType::ScilabInt32:
        {
            types::Int32 *pI = pIT->getAs<types::Int32>();
            return pI->getImg();
        }
        case types::InternalType::ScilabUInt32:
        {
            types::UInt32 *pI = pIT->getAs<types::UInt32>();
            return pI->getImg();
        }
        case types::InternalType::ScilabInt64:
        {
            types::Int64 *pI = pIT->getAs<types::Int64>();
            return pI->getImg();
        }
        case types::InternalType::ScilabUInt64:
        {
            types::UInt64 *pI = pIT->getAs<types::UInt64>();
            return pI->getImg();
        }
        default:
            return NULL;
    }
}

void mxSetImagData(mxArray *array_ptr, void *data_ptr)
{
    if (mxIsDouble(array_ptr))
    {
        ((types::Double *)array_ptr->ptr)->setImg((double *)data_ptr);
    }
}

mxChar *mxGetChars(mxArray *array_ptr)
{
    if (!mxIsChar(array_ptr))
    {
        return NULL;
    }
    wchar_t *chars = ((types::String *)array_ptr->ptr)->get(0);
    return (mxChar *)wide_string_to_UTF8(chars);
}

mxLogical *mxGetLogicals(const mxArray *ptr)
{
    types::InternalType *pIT = (types::InternalType *)ptr->ptr;
    if (pIT == NULL || pIT->isBool() == false)
    {
        return NULL;
    }

    types::Bool *pB = pIT->getAs<types::Bool>();
    return (mxLogical *)pB->get();
}

mxClassID mxGetClassID(const mxArray *ptr)
{
    types::InternalType *pIT = dynamic_cast<types::InternalType*>((types::InternalType*)ptr->ptr);
    if (pIT == NULL)
    {
        return mxUNKNOWN_CLASS;
    }

    switch (pIT->getType())
    {
        case types::InternalType::ScilabInt8:
            return mxINT8_CLASS;
        case types::InternalType::ScilabUInt8:
            return mxUINT8_CLASS;
        case types::InternalType::ScilabInt16:
            return mxINT16_CLASS;
        case types::InternalType::ScilabUInt16:
            return mxUINT16_CLASS;
        case types::InternalType::ScilabInt32:
            return mxINT32_CLASS;
        case types::InternalType::ScilabUInt32:
            return mxUINT32_CLASS;
        case types::InternalType::ScilabInt64:
            return mxINT64_CLASS;
        case types::InternalType::ScilabUInt64:
            return mxUINT64_CLASS;
        case types::InternalType::ScilabString:
            return mxCHAR_CLASS;
        case types::InternalType::ScilabDouble:
            return mxDOUBLE_CLASS;
        case types::InternalType::ScilabBool:
            return mxLOGICAL_CLASS;
        case types::InternalType::ScilabFloat:
            return mxSINGLE_CLASS;
        case types::InternalType::ScilabStruct:
            return mxSTRUCT_CLASS;
        case types::InternalType::ScilabCell:
            return mxCELL_CLASS;
        case types::InternalType::ScilabFunction:
            return mxFUNCTION_CLASS;
        default:
            return mxUNKNOWN_CLASS;
    }
}

const char *mxGetClassName(const mxArray *ptr)
{
    if (mxIsDouble(ptr))
    {
        return "double";
    }
    if (mxIsChar(ptr))
    {
        return "char";
    }
    if (mxIsLogical(ptr))
    {
        return "bool";
    }
    if (mxIsSparse(ptr))
    {
        return "sparse";
    }
    if (mxIsInt8(ptr))
    {
        return "int8";
    }
    if (mxIsInt16(ptr))
    {
        return "int16";
    }
    if (mxIsInt32(ptr))
    {
        return "int32";
    }
    if (mxIsInt64(ptr))
    {
        return "int64";
    }
    if (mxIsUint8(ptr))
    {
        return "uint8";
    }
    if (mxIsUint16(ptr))
    {
        return "uint16";
    }
    if (mxIsUint32(ptr))
    {
        return "uint32";
    }
    if (mxIsUint64(ptr))
    {
        return "uint64";
    }
    if (mxIsCell(ptr))
    {
        return "cell";
    }
    if (mxIsStruct(ptr))
    {
        return "struct";
    }
    return "unknown";
}

mxArray *mxGetProperty(const mxArray *pa, mwIndex index, const char *propname)
{
    //TODO : Dummy function as it requires objects which are Matlab class instances and there are no classes in Scilab.
    return NULL;
}

void mxSetProperty(mxArray *pa, mwIndex index, const char *propname, const mxArray *value)
{
    //TODO : Dummy function as it requires objects which are Matlab class instances and there are no classes in Scilab.
}

mxArray *mxGetField(const mxArray *ptr, int lindex, const char *string)
{
    int field_num = mxGetFieldNumber(ptr, string);
    if (field_num < 0)
    {
        return NULL;
    }
    return mxGetFieldByNumber(ptr, lindex, field_num);
}

void mxSetField(mxArray *array_ptr, int lindex, const char *field_name, mxArray *value)
{
    int field_num = mxGetFieldNumber(array_ptr, field_name);
    if (field_num >= 0)
    {
        mxSetFieldByNumber(array_ptr, lindex, field_num, value);
    }
}

int mxGetNumberOfFields(const mxArray *ptr)
{
    if (!mxIsStruct(ptr))
    {
        return 0;
    }

    types::Struct * pa = (types::Struct*)ptr->ptr;
    return pa->getFieldNames()->getSize();
}

const char *mxGetFieldNameByNumber(const mxArray *array_ptr, int field_number)
{
    if (!mxIsStruct(array_ptr))
    {
        return NULL;
    }
    if (field_number < 0 || field_number >= mxGetNumberOfFields(array_ptr))
    {
        return NULL;
    }
    types::String *names = ((types::Struct*)array_ptr->ptr)->getFieldNames();
    wchar_t *name = names->get(field_number);
    return (const char *)wide_string_to_UTF8(name);
}

int mxGetFieldNumber(const mxArray *ptr, const char *string)
{
    if (!mxIsStruct(ptr))
    {
        return -1;
    }

    types::Struct *pa = (types::Struct *)ptr->ptr;
    types::String *names = pa->getFieldNames();
    wchar_t *field_name = to_wide_string(string);

    for (int i = 0; i < names->getSize(); i++)
    {
        if (wcscmp(names->get(i), field_name) == 0)
        {
            FREE(field_name);
            return i;
        }
    }
    FREE(field_name);
    return -1;
}

mxArray *mxGetFieldByNumber(const mxArray *ptr, int lindex, int field_number)
{
    if (!mxIsStruct(ptr))
    {
        return NULL;
    }
    if (lindex >= mxGetNumberOfElements(ptr) || lindex < 0)
    {
        return NULL;
    }
    if (field_number >= mxGetNumberOfFields(ptr) || field_number < 0)
    {
        return NULL;
    }

    types::Struct *pa = (types::Struct *)ptr->ptr;
    types::String *names = pa->getFieldNames();
    types::SingleStruct *s = pa->get(lindex);

    mxArray* ret = new mxArray;
    ret->ptr = (int*)s->get(names->get(field_number));
    return ret;
}

void mxSetFieldByNumber(mxArray *array_ptr, int lindex, int field_number, mxArray *value)
{
    if (mxIsStruct(array_ptr) && lindex < mxGetNumberOfElements(array_ptr))
    {
        types::SingleStruct *ptr = ((types::Struct*)array_ptr->ptr)->get(lindex);
        types::String *names = ptr->getFieldNames();
        ptr->set(names->get(field_number), (types::InternalType *)value->ptr);
    }
}

int mxAddField(mxArray *ptr, const char *fieldname)
{
    if (!mxIsStruct(ptr))
    {
        return -1;
    }

    types::Struct *pa = (types::Struct*)ptr->ptr;
    wchar_t *wfieldname = to_wide_string(fieldname);
    ptr->ptr = (int*)pa->addField(wfieldname);
    FREE(wfieldname);
    return mxGetFieldNumber(ptr, fieldname);
}

void mxRemoveField(mxArray *pm, int fieldnumber)
{
    //TODO
}

mxArray *mxGetCell(const mxArray *ptr, int lindex)
{
    types::Cell * pa = (types::Cell *)ptr->ptr;
    mxArray* ret = new mxArray;
    ret->ptr = (int*)pa->get(lindex);
    return ret;
}

void mxSetCell(mxArray *array_ptr, int lindex, mxArray *value)
{
    array_ptr->ptr = (int*)((types::Cell *)array_ptr->ptr)->set(lindex, (types::InternalType *)value->ptr);
}

int mxGetNzmax(const mxArray *ptr)
{
    if (mxIsSparse(ptr) == 0)
    {
        return 0;
    }

    types::InternalType *pIT = (types::InternalType *)ptr->ptr;
    if (pIT == NULL || pIT->isGenericType() == false)
    {
        return 0;
    }

    types::GenericType *pGT = pIT->getAs<types::GenericType>();

    int nzmax = ((types::Sparse *)pGT)->nonZeros();

    return nzmax;
}

void mxSetNzmax(mxArray *array_ptr, int nzmax)
{
    // TODO
}

int *mxGetIr(const mxArray *ptr)
{
    if ( mxIsSparse(ptr) == 0)
    {
        return NULL;
    }
    int innercount = 0;
    int *ir = ((types::Sparse *)ptr->ptr)->getInnerPtr(&innercount);

    return ir;
}

void mxSetIr(mxArray *array_ptr, int *ir_data)
{
    // TODO
}

int *mxGetJc(const mxArray *ptr)
{
    if (mxIsSparse(ptr) == 0)
    {
        return NULL;
    }

    types::InternalType *pIT = (types::InternalType *)ptr->ptr;
    if (pIT == NULL || pIT->isGenericType() == false)
    {
        return NULL;
    }

    types::GenericType *pGT = pIT->getAs<types::GenericType>();

    int nzmax = ((types::Sparse *)pGT)->nonZeros();
    int *colPos = new int[nzmax];
    ((types::Sparse *)pGT)->getColPos(colPos);

    return colPos;
}

void mxSetJc(mxArray *array_ptr, int *jc_data)
{
    // TODO
}

void setmexFunctionName(const char* name)
{
    ConfigVariable::setMexFunctionName(name);
}

const char *mexFunctionName(void)
{
    return ConfigVariable::getMexFunctionName().c_str();
}

int mexAtExit(void(*func)(void))
{
    exitFcn = func;
    return 0;
}

int mexCallMATLAB(int nlhs, mxArray **plhs, int nrhs, mxArray **prhs, const char *name)
{
    return mexCallSCILAB(nlhs, plhs, nrhs, prhs, name);;
}

mxArray *mexCallMATLABWithTrap(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[], const char *functionName)
{
    //TODO
    return NULL;
}

int mexEvalString(const char *name)
{
    types::typed_list in;
    types::typed_list out;
    in.push_back(new types::String(name));
    types::Callable::ReturnValue ret = Overload::call(L"execstr", in, 1, out);
    in.back()->killMe();
    if (ret != types::Callable::OK)
    {
        return 1;
    }

    return 0;
}

mxArray *mexEvalStringWithTrap(const char *command)
{
    //TODO
    return NULL;
}

const mxArray *mexGet(double handle, const char *property)
{
    //TODO : Dummy function as it requires objects which are Matlab class instances and there are no classes in Scilab.
    return NULL;
}

int mexSet(double handle, const char *property, mxArray *value)
{
    //TODO : Dummy function as it requires objects which are Matlab class instances and there are no classes in Scilab.
    return 0;
}

mxArray *mexGetVariable(const char *workspace, const char *name)
{
    mxArray* ptr = const_cast<mxArray*>(mexGetVariablePtr(workspace, name));

    if (ptr && ptr->ptr)
    {
        ptr->ptr = (int*)((types::InternalType*)ptr->ptr)->clone();
    }

    return ptr;
}

const mxArray *mexGetVariablePtr(const char *workspace, const char *name)
{
    symbol::Context *context = symbol::Context::getInstance();
    wchar_t *key = to_wide_string(name);
    mxArray* ret = new mxArray;
    ret->ptr = NULL;
    symbol::Symbol sym = symbol::Symbol(key);
    if (strcmp(workspace, "base") == 0)
    {
        ret->ptr = (int*)context->get(sym);
    }
    else if (strcmp(workspace, "caller") == 0)
    {
        if (context->isGlobalVisible(sym) == false)
        {
            ret->ptr = (int*)context->get(sym);
        }
    }
    else if (strcmp(workspace, "global") == 0)
    {
        if (context->isGlobalVisible(sym))
        {
            ret->ptr = (int*)context->getGlobalValue(sym);
        }
    }
    FREE(key);
    if (ret->ptr == NULL)
    {
        delete ret;
        return NULL;
    }

    return ret;
}

int mexPutVariable(const char *workspace, const char *varname, const mxArray *pm)
{
    symbol::Context *context = symbol::Context::getInstance();
    wchar_t *dest = to_wide_string(varname);
    if (strcmp(workspace, "base") == 0)
    {
        context->putInPreviousScope(context->getOrCreate(symbol::Symbol(dest)), (types::InternalType *)pm->ptr);
    }
    else if (strcmp(workspace, "caller") == 0)
    {
        context->put(symbol::Symbol(dest), (types::InternalType *)pm->ptr);
    }
    else if (strcmp(workspace, "global") == 0)
    {
        context->setGlobalVisible(symbol::Symbol(dest), true);
        context->put(symbol::Symbol(dest), (types::InternalType *)pm->ptr);
    }
    else
    {
        FREE(dest);
        return 1;
    }
    FREE(dest);
    return 0;
}

int mexIsGlobal(const mxArray *ptr)
{
    symbol::Context *context = symbol::Context::getInstance();
    std::list<std::pair<std::wstring,int>> lst;
    context->getGlobalInfoForWho(lst, false);

    for (auto it : lst)
    {
        symbol::Symbol s = symbol::Symbol(it.first);
        types::InternalType* value = context->getGlobalValue(s);
        if ((int*)value == ptr->ptr)
        {
            return 1;
        }
    }
    return 0;
}

int mexPrintf(const char *format, ...)
{
    // TODO: define this size limit
    char string[1024];
    va_list arg_ptr;
    va_start(arg_ptr, format);
    vsnprintf(string, 1024, format, arg_ptr);
    va_end(arg_ptr);
    scilabWrite(string);
    return 0;
}

void mexSetTrapFlag(int trapflag)
{
    //TODO
}

void mexErrMsgIdAndTxt(const char *errorid, const char *errormsg, ...)
{
    //TODO: Have to handle errorid.
    mexErrMsgTxt(errormsg);
}

void mexWarnMsgIdAndTxt(const char *warningid, const char *warningmsg, ...)
{
    //TODO: Have to handle warningid.
    mexWarnMsgTxt(warningmsg);
}

void mexErrMsgTxt(const char *error_msg)
{
    throw ast::InternalError(error_msg);
}

void mexWarnMsgTxt(const char *warning_msg)
{
    scilabWrite(_("Warning: "));
    scilabWrite(warning_msg);
    scilabWrite("\n\n");
}

int mexIsLocked(void)
{
    //TODO
    return 0;
}

void mexLock(void)
{
    //TODO
}

void mexUnlock(void)
{
    //TODO
}

void mexMakeArrayPersistent(void *ptr)
{
    //Dummy Function as in Scilab6 Memory is not tracked.
}

void mexMakeMemoryPersistent(void *ptr)
{
    //Dummy Function as in Scilab6 Memory is not tracked.
}

double mxGetInf(void)
{
    types::InternalType *pITInf = symbol::Context::getInstance()->get(symbol::Symbol(L"%inf"));
    if (pITInf && pITInf->isDouble())
    {
        return pITInf->getAs<types::Double>()->get(0);
    }

    return -1;
}

double mxGetNaN(void)
{
    types::InternalType *pITInf = symbol::Context::getInstance()->get(symbol::Symbol(L"%nan"));
    if (pITInf)
    {
        return pITInf->getAs<types::Double>()->get(0);
    }

    return -1;
}

double mxGetEps(void)
{
    types::InternalType *pITEps = symbol::Context::getInstance()->get(symbol::Symbol(L"%eps"));
    if (pITEps && pITEps->isDouble())
    {
        return pITEps->getAs<types::Double>()->get(0);
    }

    return -1;
}

