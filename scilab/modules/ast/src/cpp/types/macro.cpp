/*
 *  Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2009-2009 - DIGITEO - Bruno JOFRET
 *  Copyright (C) 2023 - Dassault Systèmes - Antoine ELIAS
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */

#include <cstdio>
#include <cmath>
#include <memory>
#include <iostream>
#include <sstream>
#include <algorithm>

#include "UTF8.hxx"
#include "configvariable.hxx"
#include "context.hxx"
#include "filemanager.hxx"
#include "list.hxx"
#include "listinsert.hxx"
#include "macro.hxx"
#include "string.hxx"
#include "int.hxx"
#include "double.hxx"
#include "runvisitor.hxx"
#include "scilabWrite.hxx"
#include "serializervisitor.hxx"
#include "symbol.hxx"
#include "parser.hxx"
#include "types_comparison_eq.hxx"
#include "types_comparison_ne.hxx"
#include "types_comparison_lt_le_gt_ge.hxx"
#include "user.hxx"
#include "mlist.hxx"
#include "tlist.hxx"
#include "argumentvisitor.hxx"
#include "macrovarvisitor.hxx"

extern "C"
{
#include "Scierror.h"
#include "localization.h"
#include "os_string.h"
#include "sci_malloc.h"
#include "sciprint.h"
#include "Sciwarning.h"
#include "graphicObjectProperties.h"
#include "hasUIControlStyle.h"
#include "hasHandleType.h"
#include "FileExist.h"
#include "isdir.h"
#include "expandPathVariable.h"
}


bool mustBeDouble(types::InternalType* x) { return x->isDouble(); }
bool mustBeString(types::InternalType* x) { return x->isString(); }
bool mustBeBool(types::InternalType* x) { return x->isBool(); }
bool mustBeInt(types::InternalType* x) { return x->isInt(); }
bool mustBeInt8(types::InternalType* x) { return x->isInt8(); }
bool mustBeUInt8(types::InternalType* x) { return x->isUInt8(); }
bool mustBeInt16(types::InternalType* x) { return x->isInt16(); }
bool mustBeUInt16(types::InternalType* x) { return x->isUInt16(); }
bool mustBeInt32(types::InternalType* x) { return x->isInt32(); }
bool mustBeUInt32(types::InternalType* x) { return x->isUInt32(); }
bool mustBeInt64(types::InternalType* x) { return x->isInt64(); }
bool mustBeUInt64(types::InternalType* x) { return x->isUInt64(); }
bool mustBePoly(types::InternalType* x) { return x->isPoly(); }
bool mustBeList(types::InternalType* x) { return x->isList(); }
bool mustBeTList(types::InternalType* x) { return x->isTList(); }
bool mustBeMList(types::InternalType* x) { return x->isMList(); }
bool mustBePointer(types::InternalType* x) { return x->isPointer(); }
bool mustBeHandle(types::InternalType* x) { return x->isHandle(); }
bool mustBeStruct(types::InternalType* x) { return x->isStruct(); }
bool mustBeCell(types::InternalType* x) { return x->isCell(); }
bool mustBeLib(types::InternalType* x) { return x->isLibrary(); }
bool mustBeFunction(types::InternalType* x) { return x->isCallable(); }
bool mustBeBuiltin(types::InternalType* x) { return x->isFunction(); }
bool mustBeMacro(types::InternalType* x) { return x->isMacro() || x->isMacroFile(); }
bool mustBeSparse(types::InternalType* x) { return x->isSparse(); }
bool mustBeBoolSparse(types::InternalType* x) { return x->isSparseBool(); }
bool mustBeImplicitList(types::InternalType* x) { return x->isImplicitList(); }
template<int go> bool mustBeHandleType(types::InternalType* x) { return x->isHandle() && hasHandleType(x->getAs<types::GraphicHandle>()->get()[0], go); }
template<int go> bool mustBeUIControlStyle(types::InternalType* x) { return mustBeHandleType<__GO_UICONTROL__>(x) && hasUIControlStyle(x->getAs<types::GraphicHandle>()->get()[0], go); }

std::map<std::wstring, std::function<bool(types::InternalType*)>> typeValidator = {
    {L"double", mustBeDouble}, {L"constant", mustBeDouble}, {L"bool", mustBeBool}, {L"boolean", mustBeBool}, {L"string", mustBeString}, {L"int", mustBeInt},
    {L"int8", mustBeInt8}, {L"uint8", mustBeUInt8}, {L"int16", mustBeInt16}, {L"uint16", mustBeUInt16}, {L"int32", mustBeInt32}, {L"uint32", mustBeUInt32},
    {L"int64", mustBeInt64}, {L"uint64", mustBeUInt64}, {L"poly", mustBePoly}, {L"polynomial", mustBePoly}, {L"list", mustBeList}, {L"tlist", mustBeTList},
    {L"mlist", mustBeMList}, {L"pointer", mustBePointer}, {L"handle", mustBeHandle}, {L"struct", mustBeStruct}, {L"st", mustBeStruct}, {L"cell", mustBeCell}, 
    {L"library", mustBeLib}, {L"lib", mustBeLib}, {L"function", mustBeFunction}, {L"builtin", mustBeBuiltin}, {L"gateway", mustBeBuiltin}, {L"macro", mustBeMacro}, 
    {L"sparse", mustBeSparse}, {L"booleansparse", mustBeBoolSparse}, {L"boolsparse", mustBeBoolSparse}, {L"implicitlist", mustBeImplicitList}, {L"range", mustBeImplicitList}, 
    {L"axes", mustBeHandleType<__GO_AXES__>}, {L"axis", mustBeHandleType<__GO_AXIS__>}, {L"champ", mustBeHandleType<__GO_CHAMP__>}, {L"compound", mustBeHandleType<__GO_COMPOUND__>},
    {L"fac3d", mustBeHandleType<__GO_FAC3D__>}, {L"fec", mustBeHandleType<__GO_FEC__>}, {L"figure", mustBeHandleType<__GO_FIGURE__>},  {L"grayplot", mustBeHandleType<__GO_GRAYPLOT__>}, 
    {L"label", mustBeHandleType<__GO_LABEL__>}, {L"legend", mustBeHandleType<__GO_LEGEND__>}, {L"matplot", mustBeHandleType<__GO_MATPLOT__>}, {L"plot3d", mustBeHandleType<__GO_PLOT3D__>}, 
    {L"polyline", mustBeHandleType<__GO_POLYLINE__>}, {L"rect", mustBeHandleType<__GO_RECTANGLE__>}, {L"segs", mustBeHandleType<__GO_SEGS__>}, /*{L"text", mustBeHandleType<__GO_TEXT__>},*/ 
    {L"uicontrol", mustBeHandleType<__GO_UICONTROL__>}, {L"uimenu", mustBeHandleType<__GO_UIMENU__>}, 
    {L"checkbox", mustBeUIControlStyle<__GO_UI_CHECKBOX__>}, {L"edit", mustBeUIControlStyle<__GO_UI_EDIT__>}, {L"spinner", mustBeUIControlStyle<__GO_UI_SPINNER__>}, 
    {L"frame", mustBeUIControlStyle<__GO_UI_FRAME__>}, {L"image", mustBeUIControlStyle<__GO_UI_IMAGE__>}, {L"listbox", mustBeUIControlStyle<__GO_UI_LISTBOX__>}, 
    {L"popupmenu", mustBeUIControlStyle<__GO_UI_POPUPMENU__>}, {L"pushbutton", mustBeUIControlStyle<__GO_UI_PUSHBUTTON__>}, {L"radiobutton", mustBeUIControlStyle<__GO_UI_RADIOBUTTON__>}, 
    {L"slider", mustBeUIControlStyle<__GO_UI_SLIDER__>}, {L"text", mustBeUIControlStyle<__GO_UI_TEXT__>}, {L"layer", mustBeUIControlStyle<__GO_UI_LAYER__>}, 
    {L"tab", mustBeUIControlStyle<__GO_UI_TAB__>}
};

static types::InternalType* callComparison(std::function<types::InternalType*(types::InternalType*, types::InternalType*)> cmp, ast::OpExp::Oper oper, types::InternalType* x, types::InternalType* y)
{
    types::InternalType* pIT = cmp(x, y);
    if (pIT == nullptr)
    {
        types::typed_list in = {x, y};
        types::typed_list out;
        types::Function::ReturnValue ret = Overload::generateNameAndCall(Overload::getNameFromOper(oper), in, 1, out, true);
        if (ret == types::Function::ReturnValue::OK)
        {
            return out[0];
        }
    }

    return pIT;
}

static types::InternalType* callComparison(std::function<types::InternalType*(types::InternalType*, types::InternalType*, const std::wstring&)> cmp, ast::OpExp::Oper oper, const std::wstring& operstr, types::InternalType* x, types::InternalType* y)
{
    types::InternalType* pIT = cmp(x, y, operstr);
    if (pIT == nullptr)
    {
        types::typed_list in = {x, y};
        types::typed_list out;
        types::Function::ReturnValue ret = Overload::generateNameAndCall(Overload::getNameFromOper(oper), in, 1, out, true);
        if (ret == types::Function::ReturnValue::OK)
        {
            return out[0];
        }
    }

    return pIT;
}

template<class T_OUT, class T_IN>
types::InternalType* convertNum(types::InternalType* val)
{
    T_IN* in = val->getAs<T_IN>();
    T_OUT* out = new T_OUT(in->getDims(), in->getDimsArray());
    typename T_OUT::type* pout = out->get();
    typename T_IN::type* pin = in->get();
    for (int i = 0; i < in->getSize(); ++i)
    {
        pout[i] = static_cast<typename T_OUT::type>(pin[i]);
    }

    return out;
}

template<class T_IN>
types::InternalType* convertBool(types::InternalType* val)
{
    T_IN* in = val->getAs<T_IN>();
    types::Bool* out = new types::Bool(in->getDims(), in->getDimsArray());
    int* pout = out->get();
    typename T_IN::type* pin = in->get();
    for (int i = 0; i < in->getSize(); ++i)
    {
        pout[i] = pin[i] == 0 ? 0 : 1;
    }

    return out;
}

types::InternalType* toDouble(types::InternalType* val, const std::wstring& name)
{
    switch (val->getType())
    {
        case types::InternalType::ScilabDouble:
            return val;
        case types::InternalType::ScilabBool:
            return convertNum<types::Double, types::Bool>(val);
        case types::InternalType::ScilabInt8:
            return convertNum<types::Double, types::Int8>(val);
        case types::InternalType::ScilabUInt8:
            return convertNum<types::Double, types::UInt8>(val);
        case types::InternalType::ScilabInt16:
            return convertNum<types::Double, types::Int16>(val);
        case types::InternalType::ScilabUInt16:
            return convertNum<types::Double, types::UInt16>(val);
        case types::InternalType::ScilabInt32:
            return convertNum<types::Double, types::Int32>(val);
        case types::InternalType::ScilabUInt32:
            return convertNum<types::Double, types::UInt32>(val);
        case types::InternalType::ScilabInt64:
            return convertNum<types::Double, types::Int64>(val);
        case types::InternalType::ScilabUInt64:
            return convertNum<types::Double, types::UInt64>(val);
        case types::InternalType::ScilabString:
        {
            types::String* in = val->getAs<types::String>();
            types::Double* out = new types::Double(in->getDims(), in->getDimsArray());
            double* pout = out->get();
            for (int i = 0; i < in->getSize(); ++i)
            {
                pout[i] = wcstod(in->get()[i], NULL);
            }

            return out;
        }
        default:
        {
            char msg[128];
            os_sprintf(msg, _("%ls: Unable to convert '%ls' to double.\n"), name.data(), val->getTypeStr().data());
            throw ast::InternalError(scilab::UTF8::toWide(msg));
        }
    }
}

template<class T>
types::InternalType* toInt(types::InternalType* val, const std::wstring& name)
{
    switch (val->getType())
    {
        case types::InternalType::ScilabDouble:
            return convertNum<T, types::Double>(val);
        case types::InternalType::ScilabBool:
            return convertNum<T, types::Bool>(val);
        case types::InternalType::ScilabInt8:
            return convertNum<T, types::Int8>(val);
        case types::InternalType::ScilabUInt8:
            return convertNum<T, types::UInt8>(val);
        case types::InternalType::ScilabInt16:
            return convertNum<T, types::Int16>(val);
        case types::InternalType::ScilabUInt16:
            return convertNum<T, types::UInt16>(val);
        case types::InternalType::ScilabInt32:
            return convertNum<T, types::Int32>(val);
        case types::InternalType::ScilabUInt32:
            return convertNum<T, types::UInt32>(val);
        case types::InternalType::ScilabInt64:
            return convertNum<T, types::Int64>(val);
        case types::InternalType::ScilabUInt64:
            return convertNum<T, types::UInt64>(val);
        case types::InternalType::ScilabString:
        {
            types::String* in = val->getAs<types::String>();
            T* out = new T(in->getDims(), in->getDimsArray());
            typename T::type* pout = out->get();
            for (int i = 0; i < in->getSize(); ++i)
            {
                pout[i] = static_cast<typename T::type>(std::wcstoull(in->get()[i], NULL, 10));
            }

            return out;
        }
        default:
        {
            char msg[128];
            os_sprintf(msg, _("%ls: Unable to convert '%ls' to int.\n"), name.data(), val->getTypeStr().data());
            throw ast::InternalError(scilab::UTF8::toWide(msg));
        }
    }
}

types::InternalType* toBool(types::InternalType* val, const std::wstring& name)
{
    switch (val->getType())
    {
        case types::InternalType::ScilabBool:
            return val;
        case types::InternalType::ScilabDouble:
        case types::InternalType::ScilabInt8:
        case types::InternalType::ScilabUInt8:
        case types::InternalType::ScilabInt16:
        case types::InternalType::ScilabUInt16:
        case types::InternalType::ScilabInt32:
        case types::InternalType::ScilabUInt32:
        case types::InternalType::ScilabInt64:
        case types::InternalType::ScilabUInt64:
            return toInt<types::Bool>(val, name);
        case types::InternalType::ScilabString:
        {
            types::String* in = val->getAs<types::String>();
            types::Bool* out = new types::Bool(in->getDims(), in->getDimsArray());
            int* pout = out->get();
            for (int i = 0; i < in->getSize(); ++i)
            {
                pout[i] = wcscmp(in->get()[i], L"T") == 0 ? 1 : 0;
            }

            return out;
        }
        default:
        {
            char msg[128];
            os_sprintf(msg, _("%ls: Unable to convert '%ls' to boolean.\n"), name.data(), val->getTypeStr().data());
            throw ast::InternalError(scilab::UTF8::toWide(msg));
        }
    }
}

template <class T>
types::String* toStringNum(T* in)
{
    types::String* s = new types::String(in->getDims(), in->getDimsArray());
    typename T::type* pin = in->get();

    for (int i = 0; i < in->getSize(); ++i)
    {
        s->set(i, std::to_wstring(pin[i]).c_str());
    }

    return s;
}

types::InternalType* toStringNum(types::Double* d)
{
    if (d->isEmpty())
    {
        return d;
    }

    types::String* s = new types::String(d->getDims(), d->getDimsArray());
    if (d->isComplex())
    {
        std::wostringstream ostr;
        double* pR = d->get();
        double* pI = d->getImg();
        for (int i = 0; i < d->getSize(); ++i)
        {
            DoubleComplexMatrix2String(&ostr, pR[i], pI[i]);
            s->set(i, ostr.str().c_str());
            ostr.str(L"");
        }
    }
    else
    {
        std::wostringstream ostr;
        double* pR = d->get();
        for (int i = 0; i < d->getSize(); ++i)
        {
            DoubleComplexMatrix2String(&ostr, pR[i], 0);
            s->set(i, ostr.str().c_str());
            ostr.str(L"");
        }
    }

    return s;
}

types::InternalType* toStringBool(types::Bool* b)
{
    types::String* s = new types::String(b->getDims(), b->getDimsArray());
    int* pin = b->get();
    const wchar_t* True = L"T";
    const wchar_t* False = L"F";
    for (int i = 0; i < b->getSize(); ++i)
    {
        s->set(i, pin[i] == 0 ? False : True);
    }

    return s;
}

types::InternalType* toString(types::InternalType* val, const std::wstring& name)
{
    switch (val->getType())
    {
        case types::InternalType::ScilabDouble:
            return toStringNum(val->getAs<types::Double>());
        case types::InternalType::ScilabInt8:
            return toStringNum(val->getAs<types::Int8>());
        case types::InternalType::ScilabUInt8:
            return toStringNum(val->getAs<types::UInt8>());
        case types::InternalType::ScilabInt16:
            return toStringNum(val->getAs<types::Int16>());
        case types::InternalType::ScilabUInt16:
            return toStringNum(val->getAs<types::UInt16>());
        case types::InternalType::ScilabInt32:
            return toStringNum(val->getAs<types::Int32>());
        case types::InternalType::ScilabUInt32:
            return toStringNum(val->getAs<types::UInt32>());
        case types::InternalType::ScilabInt64:
            return toStringNum(val->getAs<types::Int64>());
        case types::InternalType::ScilabUInt64:
            return toStringNum(val->getAs<types::UInt64>());
        case types::InternalType::ScilabBool:
            return toStringBool(val->getAs<types::Bool>());
        case types::InternalType::ScilabString:
            return val;
        default:
        {
            char msg[128];
            os_sprintf(msg, _("%ls: Unable to convert '%ls' to string.\n"), name.data(), val->getTypeStr().data());
            throw ast::InternalError(scilab::UTF8::toWide(msg));
        }
    }
}

std::map<std::wstring, std::function<types::InternalType*(types::InternalType*, const std::wstring& name)>> typeConvertors = {
    {L"double", toDouble},
    {L"constant", toDouble},
    {L"int", toInt<types::Int32>},
    {L"uint", toInt<types::UInt32>},
    {L"int8", toInt<types::Int8>},
    {L"uint8", toInt<types::UInt8>},
    {L"int16", toInt<types::Int16>},
    {L"uint6", toInt<types::UInt16>},
    {L"int32", toInt<types::Int32>},
    {L"uint32", toInt<types::UInt32>},
    {L"int64", toInt<types::Int64>},
    {L"uint64", toInt<types::UInt64>},
    {L"bool", toBool},
    {L"boolean", toBool},
    {L"string", toString}
};


bool andBool(types::InternalType* ret)
{
    if (ret && ret->isBool())
    {
        types::Bool* b = ret->getAs<types::Bool>();
        for (int i = 0; i < b->getSize(); ++i)
        {
            if (b->get()[i] == 0)
            {
                return false;
            }
        }
    }

    return true;
}

bool orBool(types::InternalType* ret)
{
    if (ret && ret->isBool())
    {
        types::Bool* b = ret->getAs<types::Bool>();
        for (int i = 0; i < b->getSize(); ++i)
        {
            if (b->get()[i] == 1)
            {
                return true;
            }
        }
    }

    return false;
}

int mustBePositive(types::typed_list& x)
{
    types::InternalType* tmp2 = callComparison(GenericGreater, ast::OpExp::Oper::gt, x[0], new types::Double(0));
    if (tmp2)
    {
        bool res = andBool(tmp2);
        tmp2->killMe();
        return res ? 0 : 1;
    }

    return 1;
}

int mustBeNonpositive(types::typed_list& x)
{
    types::InternalType* tmp2 = callComparison(GenericLessEqual, ast::OpExp::Oper::le, L"<=", x[0], new types::Double(0));
    if (tmp2)
    {
        bool res = andBool(tmp2);
        tmp2->killMe();
        return res ? 0 : 1;
    }

    return 1;
}

int mustBeNonnegative(types::typed_list& x)
{
    types::InternalType* tmp2 = callComparison(GenericGreaterEqual, ast::OpExp::Oper::ge, x[0], new types::Double(0)); 
    if (tmp2)
    {
        bool res = andBool(tmp2);
        tmp2->killMe();
        return res ? 0 : 1;
    }

    return 1;
}

int mustBeNegative(types::typed_list& x)
{
    types::InternalType* tmp2 = callComparison(GenericLess, ast::OpExp::Oper::le, L"<", x[0], new types::Double(0));
    if (tmp2)
    {
        bool res = andBool(tmp2);
        tmp2->killMe();
        return res ? 0 : 1;
    }

    return 1;
}

int mustBeNumeric(types::typed_list& x)
{
    return x[0]->isDouble() || x[0]->isInt() ? 0 : 1;
}

int mustBeFinite(types::typed_list& x)
{
    if (mustBeNumeric(x) != 0)
    {
        return 1;
    }

    if (x[0]->isDouble())
    {
        double* p = x[0]->getAs<types::Double>()->get();
        for (int i = 0; i < x[0]->getAs<types::Double>()->getSize(); ++i)
        {
            if (std::isfinite(p[i]) == false)
            {
                return 1;
            }
        }
    }
    return 0;
}

int mustBeNonNan(types::typed_list& x)
{
    if (mustBeNumeric(x) != 0)
    {
        return 1;
    }

    if (x[0]->isDouble())
    {
        double* p = x[0]->getAs<types::Double>()->get();
        for (int i = 0; i < x[0]->getAs<types::Double>()->getSize(); ++i)
        {
            if (std::isnan(p[i]))
            {
                return 1;
            }
        }
    }
    return 0;
}

int mustBeNonzero(types::typed_list& x)
{
    types::Double* tmp = new types::Double(0);
    types::InternalType* tmp2 = GenericComparisonNonEqual(x[0], tmp);
    tmp->killMe();
    if (tmp2)
    {
        bool res = andBool(tmp2);
        tmp2->killMe();
        return res ? 0 : 1;
    }

    return 1;
}

int mustBeNonsparse(types::typed_list& x)
{
    return x[0]->isSparse() ? 1 : 0;
}

static int isComplex(const double* val, size_t size, double eps)
{
    for (size_t i = 0; i < size; ++i)
    {
        if (abs(val[i]) > eps)
        {
            return 1;
        }
    }

    return 0;
}

int mustBeReal(types::typed_list& x)
{
    if (x[0]->isDouble() || x[0]->isPoly() || x[0]->isSparse())
    {
        if (x[0]->isDouble() && x[0]->getAs<types::Double>()->isComplex())
        {
            types::Double* d = x[0]->getAs<types::Double>();
            return isComplex(d->getImg(), d->getSize(), x.size() > 1 ? x[1]->getAs<types::Double>()->get()[0] : 0);
        }

        if (x[0]->isPoly() && x[0]->getAs<types::Polynom>()->isComplex())
        {
            types::Polynom* p = x[0]->getAs<types::Polynom>();
            types::Double* d = p->getCoef();
            int ret = isComplex(d->getImg(), d->getSize(), x.size() > 1 ? x[1]->getAs<types::Double>()->get()[0] : 0);
            d->killMe();
            return ret;
        }

        if (x[0]->isSparse() && x[0]->getAs<types::Sparse>()->isComplex())
        {
            types::Sparse* sp = x[0]->getAs<types::Sparse>();
            size_t nonZeros = sp->nonZeros();
            std::vector<double> NonZeroR(nonZeros);
            std::vector<double> NonZeroI(nonZeros);
            sp->outputValues(NonZeroR.data(), NonZeroI.data());

            return isComplex(NonZeroI.data(), nonZeros, x.size() > 1 ? x[1]->getAs<types::Double>()->get()[0] : 0);
        }
    }

    return 0;
}

int mustBeInteger(types::typed_list& x)
{
    if (mustBeNumeric(x) != 0)
    {
        return 1;
    }

    if (x[0]->isDouble())
    {
        double* p = x[0]->getAs<types::Double>()->get();
        for (int i = 0; i < x[0]->getAs<types::Double>()->getSize(); ++i)
        {
            if (floor(p[i]) != p[i])
            {
                return 1;
            }
        }
    }

    return 0;
}

int mustBeMember(types::typed_list& x)
{
    types::InternalType* tmp = nullptr;
    if (x[1]->isCell())
    {
        types::Cell* ce = x[1]->getAs<types::Cell>();
        types::Bool* tmp2 = new types::Bool(1, ce->getSize());
        for (int i = 0; i < ce->getSize(); ++i)
        {
            types::InternalType* tmp3 = GenericComparisonEqual(x[0], ce->get(i));
            bool res = andBool(tmp3);
            tmp2->set(i, res);
            tmp3->killMe();
        }

        tmp = tmp2;
    }
    else
    {
        tmp = GenericComparisonEqual(x[0], x[1]);
    }

    if (tmp)
    {
        bool res = orBool(tmp);
        tmp->killMe();
        return res ? 0 : 1;
    }

    return 1;
}

int mustBeGreaterThan(types::typed_list& x)
{
    types::InternalType* tmp = callComparison(GenericGreater, ast::OpExp::Oper::gt, x[0], x[1]);
    if (tmp)
    {
        bool res = andBool(tmp);
        tmp->killMe();
        return res ? 0 : 1;
    }

    return 1;
}

int mustBeGreaterThanOrEqual(types::typed_list& x)
{
    types::InternalType* tmp = callComparison(GenericGreaterEqual, ast::OpExp::Oper::ge, x[0], x[1]);
    if (tmp)
    {
        bool res = andBool(tmp);
        tmp->killMe();
        return res ? 0 : 1;
    }

    return 1;
}

int mustBeLessThan(types::typed_list& x)
{
    types::InternalType* tmp = callComparison(GenericLess, ast::OpExp::Oper::lt, L"<", x[0], x[1]);
    if (tmp)
    {
        bool res = andBool(tmp);
        tmp->killMe();
        return res ? 0 : 1;
    }

    return 1;
}

int mustBeLessThanOrEqual(types::typed_list& x)
{
    types::InternalType* tmp = callComparison(GenericLessEqual, ast::OpExp::Oper::le, L"<=", x[0], x[1]);
    if (tmp)
    {
        bool res = andBool(tmp);
        tmp->killMe();
        return res ? 0 : 1;
    }

    return 1;
}

int mustBeA(types::typed_list& x)
{
    types::String* types = x[1]->getAs<types::String>();

    for (int i = 0; i < types->getSize(); ++i)
    {
        if (typeValidator.find(types->get()[i]) != typeValidator.end())
        {
            if (typeValidator[types->get()[i]](x[0]))
            {
                return 0;
            }
        }
        else
        {
            std::wstring type;
            if (x[0]->isUserType())
            {
                type = x[0]->getAs<types::UserType>()->getTypeStr();
            }

            if (x[0]->isTList() || x[0]->isMList())
            {
                type = x[0]->getAs<types::TList>()->getTypeStr();
            }

            if (type == types->get()[i])
            {
                return 0;
            }
        }
    }

    return 1;
}

int mustBeNumericOrLogical(types::typed_list& x)
{
    return (mustBeNumeric(x) == 0 || x[0]->isBool()) ? 0 : 1;
}

int mustBeNonempty(types::typed_list& x)
{
    return (x[0]->isDouble() && x[0]->getAs<types::Double>()->isEmpty()) ? 1 : 0;
}

int mustBeScalarOrEmpty(types::typed_list& x)
{
    return (x[0]->isGenericType() && (x[0]->getAs<types::GenericType>()->getSize() == 0 || x[0]->getAs<types::GenericType>()->isScalar())) ? 0 : 1;
}

int mustBeVector(types::typed_list& x)
{
    return (x[0]->isGenericType() && x[0]->getAs<types::GenericType>()->isVector()) ? 0 : 1;
}

int mustBeSquare(types::typed_list& x)
{
    if (x[0]->isGenericType() == false)
    {
        return 1;
    }
    
    types::GenericType* gt = x[0]->getAs<types::GenericType>();

    if (gt->isDouble() && gt->getAs<types::Double>()->isEmpty())
    {
        return 1;
    }

    if (gt->getDims() != 2)
    {
        return 1;
    }

    int* dims = gt->getDimsArray();
    int ref = dims[0];
    for (int i = 1; i < gt->getDims(); ++i)
    {
        if (dims[i] < 1 || dims[i] != ref) //-1 0
        {
            return 1;
        }
    }

    return 0;
}

int mustBeInRange(types::typed_list& x)
{
    #define checkFunc(name) [](types::InternalType* x1, types::InternalType* x2) { return name(x1, x2); }
    typedef std::function<types::InternalType*(types::InternalType*, types::InternalType*)> checker;
    checker checkLeft = checkFunc(GenericGreaterEqual);
    checker checkRight = checkFunc(GenericLessEqual);

    if (x.size() == 4)
    {
        std::wstring bounds = x[3]->getAs<types::String>()->get()[0];
        if (bounds == L"exclusive")
        {
            checkLeft = checkFunc(GenericGreater);
            checkRight = checkFunc(GenericLess);
        }
        else if (bounds == L"exclude-lower")
        {
            checkLeft = checkFunc(GenericGreater);
        }
        else if (bounds == L"exclude-upper")
        {
            checkRight = checkFunc(GenericLess);
        }
    }

    return andBool(checkLeft(x[0], x[1])) && andBool(checkRight(x[0], x[2])) ? 0 : 1;
}

int mustBeFile(types::typed_list& x)
{
    if (x[0]->isString())
    {
        wchar_t* f = x[0]->getAs<types::String>()->get()[0];
        wchar_t* e = expandPathVariableW(f);
        if (e == nullptr)
        {
            return 1;
        }

        std::wstring exp(e);
        FREE(e);
        return (isdirW(exp.data()) == false && FileExistW(exp.data())) ? 0 : 1;
    }

    return 1;
}

int mustBeFolder(types::typed_list& x)
{
    if (x[0]->isString())
    {
        wchar_t* f = x[0]->getAs<types::String>()->get()[0];
        wchar_t* e = expandPathVariableW(f);
        if (e == nullptr)
        {
            return 1;
        }

        std::wstring exp(e);
        FREE(e);
        return isdirW(exp.data()) ? 0 : 1;
    }

    return 1;
}

int mustBeNonzeroLengthText(types::typed_list& x)
{
    if (x[0]->isString() && x[0]->getAs<types::String>()->isScalar())
    {
        return wcslen(x[0]->getAs<types::String>()->get()[0]) > 0 ? 0 : 1;
    }

    return 1;
}

int mustBeValidVariableName(types::typed_list& x)
{
    if (x[0]->isString() && x[0]->getAs<types::String>()->isScalar())
    {
        return symbol::Context::getInstance()->isValidVariableName(x[0]->getAs<types::String>()->get()[0]) ? 0 : 1;
    }

    return 1;
}

int mustBeEqualDims(types::typed_list& x)
{
    types::typed_list in1 = {x[0]};
    types::typed_list out1;
    if (Overload::call(L"size", in1, 1, out1) != types::Function::OK)
    {
        return 1;
    }

    types::typed_list in2 = {x[1]};
    types::typed_list out2;
    if (Overload::call(L"size", in2, 1, out2) != types::Function::OK)
    {
        return 1;
    }

    types::Double* p1 = out1[0]->getAs<types::Double>();
    std::vector<int> dims1(p1->get(), p1->get() + p1->getSize());
    p1->killMe();

    types::Double* p2 = out2[0]->getAs<types::Double>();
    std::vector<int> dims2(p2->get(), p2->get() + p2->getSize());
    p2->killMe();

    std::vector<int> ref = {-1};
    if (x.size() == 3)
    {
        types::Double* pref = x[2]->getAs<types::Double>();
        ref.clear();
        ref.reserve(pref->getSize());
        for (int i = 0; i < pref->getSize(); ++i)
        {
            ref.push_back(static_cast<int>(pref->get()[i]));
        }
    }

    if (ref.size() >= 1 && ref[0] != -1)
    {
        for (int i = 0; i < ref.size(); ++i)
        {
            if (dims1.size() < ref[i] || dims2.size() < ref[i])
            {
                return 1;
            }
        }
    }
    else
    {
        if (dims1.size() != dims2.size())
        {
            return 1;
        }
    }

    if (ref.size() >= 1 && ref[0] != -1)
    {
        for (int i = 0; i < ref.size(); ++i)
        {
            if (dims1[ref[i] - 1] != dims2[ref[i] - 1])
            {
                return 1;
            }
        }
    }
    else
    {
        for (int i = 0; i < dims1.size(); ++i)
        {
            if (dims1[i] != dims2[i])
            {
                return 1;
            }
        }
    }

    return 0;
}

int mustBeSameType(types::typed_list& x)
{
    if (x[0]->isInt() && x[1]->isInt())
    {
        return 0;
    }

    return (x[0]->getType() == x[1]->getType()) ? 0 : 1;
}

int mustBeEqualDimsOrScalar(types::typed_list& x)
{
    int size = static_cast<int>(x.size());

    for (int i = 0; i < size - 1; ++i)
    {
        for (int j = i + 1; j < size; ++j)
        {
            // if tested is scilar => OK
            if (x[i]->getAs<types::GenericType>()->isScalar())
            {
                continue;
            }

            // if ref is scalar => OK
            if (x[j]->getAs<types::GenericType>()->isScalar())
            {
                continue;
            }

            types::typed_list tl = {x[i], x[j]};
            if (mustBeEqualDims(tl) != 0)
            {
                return j + 1;
            }
        }
    }

    return 0;
}

std::map<std::wstring, std::tuple<std::function<int(types::typed_list&)>, std::vector<int>>> functionValidators = {
    {L"mustBePositive", {mustBePositive, {1}}},
    {L"mustBeNonpositive", {mustBeNonpositive, {1}}},
    {L"mustBeNonnegative", {mustBeNonnegative, {1}}},
    {L"mustBeNegative", {mustBeNegative, {1}}},
    {L"mustBeFinite", {mustBeFinite, {1}}},
    {L"mustBeNonNan", {mustBeNonNan, {1}}},
    {L"mustBeNonzero", {mustBeNonzero, {1}}},
    {L"mustBeNonsparse", {mustBeNonsparse, {1}}},
    {L"mustBeReal", {mustBeReal, {1, 2}}},
    {L"mustBeInteger", {mustBeInteger, {1}}},
    {L"mustBeGreaterThan", {mustBeGreaterThan, {2}}},
    {L"mustBeLessThan", {mustBeLessThan, {2}}},
    {L"mustBeGreaterThanOrEqual", {mustBeGreaterThanOrEqual, {2}}},
    {L"mustBeLessThanOrEqual", {mustBeLessThanOrEqual, {2}}},
    {L"mustBeA", {mustBeA, {2}}},
    {L"mustBeNumeric", {mustBeNumeric, {1}}},
    {L"mustBeNumericOrLogical", {mustBeNumericOrLogical, {1}}},
    {L"mustBeNumericOrBoolean", {mustBeNumericOrLogical, {1}}},
    {L"mustBeNonempty", {mustBeNonempty, {1}}},
    {L"mustBeScalarOrEmpty", {mustBeScalarOrEmpty, {1}}},
    {L"mustBeVector", {mustBeVector, {1}}},
    {L"mustBeSquare", {mustBeSquare, {1}}},
    {L"mustBeMember", {mustBeMember, {2}}},
    {L"mustBeInRange", {mustBeInRange, {3, 4}}},
    {L"mustBeFile", {mustBeFile, {1}}},
    {L"mustBeFolder", {mustBeFolder, {1}}},
    {L"mustBeNonzeroLengthText", {mustBeNonzeroLengthText, {1}}},
    {L"mustBeValidVariableName", {mustBeValidVariableName, {1}}},
    {L"mustBeEqualDims", {mustBeEqualDims, {2, 3}}},
    {L"mustBeSameType", {mustBeSameType, {2}}},
    {L"mustBeEqualDimsOrScalar", {mustBeEqualDimsOrScalar, {-1}}}
};

std::map<std::wstring, std::tuple<std::string, int>> errorValidators = {
    {L"mustBePositive", {"%s: Wrong value for input argument #%d: Positive numbers expected.\n", 2}},
    {L"mustBeNonpositive", {"%s: Wrong value for input argument #%d: Non positive numbers expected.\n", 2}},
    {L"mustBeNonnegative", {"%s: Wrong value for input argument #%d: Non negative numbers expected.\n", 2}},
    {L"mustBeNegative", {"%s: Wrong value for input argument #%d: Negative numbers expected.\n", 2}},
    {L"mustBeFinite", {"%s: Wrong value for input argument #%d: Finite numbers expected.\n", 2}},
    {L"mustBeNonNan", {"%s: Wrong value for input argument #%d: Nan are not allowed.\n", 2}},
    {L"mustBeNonzero", {"%s: Wrong value for input argument #%d: Zero are not allowed.\n", 2}},
    {L"mustBeNonsparse", {"%s: Wrong value for input argument #%d: Sparse are not allowed.\n", 2}},
    {L"mustBeReal", {"%s: Wrong value for input argument #%d: Real numbers expected.\n", 2}},
    {L"mustBeInteger", {"%s: Wrong value for input argument #%d: Integer numbers expected.\n", 2}},
    {L"mustBeMember", {"%s: Wrong value for input argument #%d: Must be in %s.\n", 3}},
    {L"mustBeGreaterThan", {"%s: Wrong value for input argument #%d: Must be > %s.\n", 3}},
    {L"mustBeGreaterThanOrEqual", {"%s: Wrong value for input argument #%d: Must be >= %s.\n", 3}},
    {L"mustBeLessThan", {"%s: Wrong value for input argument #%d: Must be < %s.\n", 3}},
    {L"mustBeLessThanOrEqual", {"%s: Wrong value for input argument #%d: Must be <= %s.\n", 3}},
    {L"mustBeA", {"%s: Wrong type for input argument #%d: Must be in %s.\n", 3}},
    {L"mustBeNumeric", {"%s: Wrong type for input argument #%d: Must be numeric values.\n", 2}},
    {L"mustBeNumericOrLogical", {"%s: Wrong type for input argument #%d: Must be numeric values or boolean.\n", 2}},
    {L"mustBeNumericOrBoolean", {"%s: Wrong type for input argument #%d: Must be numeric values or boolean.\n", 2}},
    {L"mustBeNonempty", {"%s: Wrong type for input argument #%d: Must not be empty.\n", 2}},
    {L"mustBeScalarOrEmpty", {"%s: Wrong type for input argument #%d: Must be a scalar or empty.\n", 2}},
    {L"mustBeVector", {"%s: Wrong type for input argument #%d: Must be a vector.\n", 2}},
    {L"mustBeSquare", {"%s: Wrong type for input argument #%d: Must be a square matrix.\n", 2}},
    {L"mustBeMember", {"%s: Wrong type for input argument #%d: Must be member of %s.\n", 3}},
    {L"mustBeInRange", {"%s: Wrong value for input argument #%d: Must be in range [%s, %s].\n", 4}},
    {L"mustBeFile", {"%s: Wrong type for input argument #%d: Must be a file.\n", 2}},
    {L"mustBeFolder", {"%s: Wrong type for input argument #%d: Must be a folder.\n", 2}},
    {L"mustBeNonzeroLengthText", {"%s: Wrong type for input argument #%d: Must not be an empty string.\n", 2}},
    {L"mustBeValidVariableName", {"%s: Wrong type for input argument #%d: Must be a valid variable name.\n", 2}},
    {L"mustBeEqualDims", {"%s: Wrong size for input argument #%d: Must be of the same dimensions of #%s.\n", 3}},
    {L"mustBeSameType", {"%s: Wrong type for input argument #%d: Must be same type of #%s.\n", 3}},
    {L"mustBeEqualDimsOrScalar", {"%s: Wrong size for input argument #%d: Must be of the same dimensions of #%s or scalar.\n", -3}},
};

std::map<std::wstring, std::vector<std::tuple<int, std::string>>> errorArgs = {
    {L"mustBeMember", {{1, ""}}},
    {L"mustBeGreaterThan", {{1, ""}}},
    {L"mustBeGreaterThanOrEqual", {{1, ""}}},
    {L"mustBeLessThan", {{1, ""}}},
    {L"mustBeLessThanOrEqual", {{1, ""}}},
    {L"mustBeA", {{1, ""}}},
    {L"mustBeInRange", {{1, ""}, {2, ""}}},
    {L"mustBeEqualDims", {{1, ""}}},
    {L"mustBeSameType", {{1, ""}}},
    {L"mustBeEqualDimsOrScalar", {{1, ""}}},
};

types::InternalType* transposevar(types::InternalType* x, const std::vector<std::tuple<std::vector<int>, symbol::Variable*>>& dims)
{
    if (x->isArrayOf())
    {
        types::GenericType* gt = x->getAs<types::GenericType>();
        // if dims == (1, x) ou (x, 1) & input is a vector, transpose it to match good shape
        if (dims.size() == 2)
        {
            std::vector<int> transposeDims;

            for (int i = 0; i < 2; ++i)
            {
                std::vector<int> dim;
                symbol::Variable* v;
                std::tie(dim, v) = dims[i];

                if (v != nullptr)
                {
                    types::InternalType* pIT = v->get();
                    if (pIT && pIT->isDouble())
                    {
                        types::Double* d = pIT->getAs<types::Double>();
                        if (d->isScalar())
                        {
                            transposeDims.push_back(static_cast<int>(d->get()[0]));
                        }
                    }
                }
                else
                {
                    if (dim.size() == 1)
                    {
                        transposeDims.push_back(dim[0]);
                    }
                }
            }

            if (transposeDims.size() == 2)
            {
                if (transposeDims[0] == 1)
                {
                    if (transposeDims[1] == -1 || transposeDims[1] == gt->getRows())
                    {
                        types::InternalType* transposed;
                        gt->transpose(transposed);
                        return transposed;
                    }
                }
                else if (transposeDims[1] == 1)
                {
                    if (transposeDims[0] == -1 || transposeDims[0] == gt->getCols())
                    {
                        types::InternalType* transposed;
                        gt->transpose(transposed);
                        return transposed;
                    }
                }
            }
        }
    }

    return nullptr;
}

types::InternalType* expandvar(types::InternalType* x, const std::vector<std::tuple<std::vector<int>, symbol::Variable*>>& dims, bool isStatic)
{
    if (x->isArrayOf())
    {
        std::vector<int> convertDims;
        types::GenericType* gt = x->getAs<types::GenericType>();
        if (gt->isScalar())
        {
            if (isStatic)
            {
                for (auto&& d : dims)
                {
                    convertDims.push_back(std::get<0>(d)[0]);
                }
            }
            else
            {
                for (auto&& d : dims)
                {
                    std::vector<int> dim;
                    symbol::Variable* v;
                    std::tie(dim, v) = d;
                    if (v == nullptr)
                    {
                        if (dim.size() == 1 && dim[0] != -1)
                        {
                            convertDims.push_back(dim[0]);
                        }
                        else
                        {
                            convertDims.clear();
                        }
                    }
                    else
                    {
                        types::InternalType* pIT = v->get();
                        if (pIT && pIT->isDouble())
                        {
                            types::Double* d = pIT->getAs<types::Double>();
                            if (d->isScalar())
                            {
                                convertDims.push_back(static_cast<int>(d->get()[0]));
                            }
                            else
                            {
                                convertDims.clear();
                            }
                        }
                        else
                        {
                            convertDims.clear();
                        }
                    }
                }
            }
        }

        if (convertDims.size() != 0)
        {
            int size = 1;
            std::for_each(convertDims.begin(), convertDims.end(), [&size](int v) { size *= v; });

            if (size != 1)
            {
                // clone to keep the same type of input argument
                types::GenericType* clone = gt->clone();
                clone->resize(convertDims.data(), static_cast<int>(convertDims.size()));

                types::typed_list in;
                types::InternalType* colon = new types::Colon();
                in.push_back(colon);

                clone = clone->insert(&in, gt);
                colon->killMe();
                return clone;
            }
        }
    }

    return x;
}

types::InternalType* checksize(types::InternalType* x, const std::vector<std::tuple<std::vector<int>, symbol::Variable*>>& dims, bool isStatic)
{
    if (x->isGenericType() == false)
    {
        return nullptr;
    }

    types::GenericType* g = x->getAs<types::GenericType>();

    types::typed_list in1 = {x};
    types::typed_list out1;
    if (Overload::call(L"size", in1, 1, out1) != types::Function::OK)
    {
        return nullptr;
    }

    types::Double* p1 = out1[0]->getAs<types::Double>();
    std::vector<int> dims1(p1->get(), p1->get() + p1->getSize());
    p1->killMe();

    if (dims1.size() > dims.size())
    {
        return nullptr;
    }

    if (dims.size() == 1 && std::get<1>(dims[0]) == nullptr)
    {
        auto&& d = std::get<0>(dims[0]);
        for (int i = 0; i < d.size(); ++i)
        {
            if (d[i] == dims1[0])
            {
                return x;
            }
        }

        return nullptr;
    }

    if (g->isScalar())
    {
        return expandvar(x, dims, isStatic);
    }

    bool status = true;
    for (int i = 0; i < dims.size(); ++i)
    {
        std::vector<int> dim;
        symbol::Variable* v;
        std::tie(dim, v) = dims[i];
        int ref = i < dims1.size() ? dims1[i] : 1;
        bool ok = false;
        if (v != nullptr)
        {
            types::InternalType* pIT = v->get();
            if (pIT && pIT->isDouble())
            {
                types::Double* d = pIT->getAs<types::Double>();

                for (int j = 0; j < d->getSize(); ++j)
                {
                    if (d->get()[j] == ref)
                    {
                        ok = true;
                        break;
                    }
                }
            }
        }
        else
        {
            for (int j = 0; j < dim.size(); ++j)
            {
                if (dim[j] == -1 || dim[j] == ref)
                {
                    ok = true;
                    break;
                }
            }
        }

        status &= ok;

        if (ok == false && dims1[i] == 1)
        {
            return transposevar(x, dims);
        }
    }

    if (status)
    {
        return x;
    }

    return nullptr;
}

std::vector<std::vector<int>> todims(const std::vector<std::tuple<std::vector<int>, symbol::Variable*>>& dims)
{
    std::vector<std::vector<int>> ret;
    for (auto&& s : dims)
    {
        std::vector<int> d;
        symbol::Variable* v;
        std::tie(d, v) = s;
        if (v == nullptr)
        {
            ret.push_back(d);
        }
        else
        {
            ret.push_back({static_cast<int>(v->top()->m_pIT->getAs<types::Double>()->get()[0])});
        }
    }

    return ret;
}

std::wstring dims2str(const std::vector<std::tuple<std::vector<int>, symbol::Variable*>>& dims)
{
    std::wstring res = L"";

    std::vector<std::vector<int>> c = todims(dims);
    for (int i = 0; i < c.size(); ++i)
    {
        auto s = c[i];
        if (res.empty() == false)
        {
            res += L" x ";
        }

        if (s.size() == 1)
        {
            res += s[0] == -1 ? std::wstring(1, L'm' + i) : std::to_wstring(s[0]);
        }
        else
        {
            std::wstring res2;
            for (auto&& d : s)
            {
                if (res2.empty() == false)
                {
                    res2 += L", ";
                }

                res2 += std::to_wstring(d);
            }

            res += L"[" + res2 + L"]";
        }
    }

    return res;
}

namespace types
{
Macro::Macro(std::vector<symbol::Variable*>& _inputArgs, ast::SeqExp& _body, const std::wstring& _stModule, std::unordered_map<std::wstring, types::InternalType*> captured) : Callable(),
    m_inputArgs(&_inputArgs), m_body(_body.clone()), m_isLambda(true), m_outputArgs(nullptr),
    m_Nargin(symbol::Context::getInstance()->getOrCreate(symbol::Symbol(L"nargin"))),
    m_Nargout(symbol::Context::getInstance()->getOrCreate(symbol::Symbol(L"nargout"))),
    m_Varargin(symbol::Context::getInstance()->getOrCreate(symbol::Symbol(L"varargin"))),
    m_Varargout(symbol::Context::getInstance()->getOrCreate(symbol::Symbol(L"varargout"))),
    m_captured(captured)
{
    setName(L"anonymous");
    setModule(_stModule);
    m_pDblArgIn = new Double(1);
    m_pDblArgIn->IncreaseRef(); // never delete
    m_pDblArgOut = new Double(1);
    m_pDblArgOut->IncreaseRef(); // never delete

    m_body->setReturnable();
    m_stPath = L"";

    updateArguments();

    // check variables/macros in body
    ast::MacrovarVisitor visit;
    getBody()->accept(visit);

    for (auto&& c : m_captured)
    {
        c.second->IncreaseRef();//protect loaded variable
    }

    // external variables
    auto externals = visit.getExternal();
    for (auto&& e : externals)
    {
        symbol::Symbol var = symbol::Symbol(e);
        if (std::find_if(m_inputArgs->begin(), m_inputArgs->end(), [var](symbol::Variable* v)
                         { return v->getSymbol() == var; }) != m_inputArgs->end())
        {
            // input parameter
            continue;
        }

        types::InternalType* pIT = symbol::Context::getInstance()->get(var);
        if (pIT == nullptr && m_captured.find(e) == m_captured.end())
        {
            char msg[128];
            os_sprintf(msg, _("%s: variable `'%ls\' must exist.\n"), "lambda", e.data());
            throw ast::InternalError(scilab::UTF8::toWide(msg));
        }

        if (pIT)
        {
            m_captured[e] = pIT->clone();
            m_captured[e]->IncreaseRef();
        }
    }

    // called functions
    auto called = visit.getCalled();
    for (auto&& c : called)
    {
        symbol::Symbol var = symbol::Symbol(c);
        if (std::find_if(m_inputArgs->begin(), m_inputArgs->end(), [var](symbol::Variable* v)
                         { return v->getSymbol() == var; }) != m_inputArgs->end())
        {
            // input parameter
            continue;
        }

        types::InternalType* pIT = symbol::Context::getInstance()->get(var);
        if (pIT == nullptr && m_captured.find(c) == m_captured.end())
        {
            char msg[128];
            os_sprintf(msg, _("%s: variable `%ls` must exist.\n"), "lambda", c.data());
            throw ast::InternalError(scilab::UTF8::toWide(msg));
        }

        if (pIT)
        {
            symbol::Variable* v = symbol::Context::getInstance()->getOrCreate(var);
            if (v->empty())
            {
                types::InternalType* p = symbol::Context::getInstance()->get(var);
                if (p)
                {
                    m_captured[c] = p->clone();
                    m_captured[c]->IncreaseRef();
                }
            }
            else if (v->top()->m_iLevel > SCOPE_GATEWAY)
            {
                // sciprint("level: %ls(%d)\n", c.data(), v->top()->m_iLevel);
                //  not a original function of Scilab
                m_captured[c] = pIT->clone();
                m_captured[c]->IncreaseRef();
            }
        }
    }
}

Macro::Macro(const std::wstring& _stName, std::vector<symbol::Variable*>& _inputArgs, std::vector<symbol::Variable*>& _outputArgs, ast::SeqExp& _body, const std::wstring& _stModule) : Callable(),
    m_inputArgs(&_inputArgs), m_outputArgs(&_outputArgs), m_body(_body.clone()), m_isLambda(false),
    m_Nargin(symbol::Context::getInstance()->getOrCreate(symbol::Symbol(L"nargin"))),
    m_Nargout(symbol::Context::getInstance()->getOrCreate(symbol::Symbol(L"nargout"))),
    m_Varargin(symbol::Context::getInstance()->getOrCreate(symbol::Symbol(L"varargin"))),
    m_Varargout(symbol::Context::getInstance()->getOrCreate(symbol::Symbol(L"varargout")))
{
    setName(_stName);
    setModule(_stModule);
    m_pDblArgIn = new Double(1);
    m_pDblArgIn->IncreaseRef(); // never delete
    m_pDblArgOut = new Double(1);
    m_pDblArgOut->IncreaseRef(); // never delete

    m_body->setReturnable();
    m_stPath = L"";

    // Do not enable debug for Macro called when checking arguments (calling sci2exp)
    bool isDebug = ConfigVariable::getEnableDebug();
    ConfigVariable::setEnableDebug(false);
    updateArguments();
    ConfigVariable::setEnableDebug(isDebug);
}

Macro::~Macro()
{
    delete m_body;
    m_pDblArgIn->DecreaseRef();
    m_pDblArgIn->killMe();
    m_pDblArgOut->DecreaseRef();
    m_pDblArgOut->killMe();

    if (m_inputArgs)
    {
        delete m_inputArgs;
    }

    if (m_outputArgs)
    {
        delete m_outputArgs;
    }

    for (auto&& sub : m_submacro)
    {
        sub.second->DecreaseRef();
        sub.second->killMe();
    }

    if (isLambda())
    {
        for (auto&& c : m_captured)
        {
            c.second->DecreaseRef();
            c.second->killMe();
        }
    }
    m_submacro.clear();
}

void Macro::cleanCall(symbol::Context* pContext, int oldPromptMode)
{
    // restore previous prompt mode
    ConfigVariable::setPromptMode(oldPromptMode);
    // close the current scope
    pContext->scope_end();
    ConfigVariable::macroFirstLine_end();
}

Macro* Macro::clone()
{
    IncreaseRef();
    return this;
}

void Macro::whoAmI()
{
    std::cout << "types::Macro";
}

ast::SeqExp* Macro::getBody(void)
{
    return m_body;
}

bool Macro::toString(std::wostringstream& ostr)
{
    // get macro name
    wchar_t* wcsVarName = NULL;
    if (ostr.str() == SPACES_LIST)
    {
        wcsVarName = os_wcsdup(getName().c_str());
    }
    else
    {
        wcsVarName = os_wcsdup(ostr.str().c_str());
    }

    ostr.str(L"");

    if (isLambda())
    {
        ostr << wcsVarName << L": ";
    }
    else
    {
        ostr << L"[";
        // output arguments [a,b,c] = ....
        if (m_outputArgs->empty() == false)
        {
            std::vector<symbol::Variable*>::iterator OutArg = m_outputArgs->begin();
            std::vector<symbol::Variable*>::iterator OutArgfter = OutArg;
            OutArgfter++;

            for (; OutArgfter != m_outputArgs->end(); OutArgfter++)
            {
                ostr << (*OutArg)->getSymbol().getName();
                ostr << ",";
                OutArg++;
            }

            ostr << (*OutArg)->getSymbol().getName();
        }

        ostr << L"]";
        // function name
        ostr << L"=" << wcsVarName;
    }

    ostr << L"(";
    // input arguments function(a,b,c)
    if (m_inputArgs->empty() == false)
    {
        std::vector<symbol::Variable*>::iterator inArg = m_inputArgs->begin();
        std::vector<symbol::Variable*>::iterator inRagAfter = inArg;
        inRagAfter++;

        for (; inRagAfter != m_inputArgs->end(); inRagAfter++)
        {
            ostr << (*inArg)->getSymbol().getName();
            ostr << ",";
            inArg++;
        }

        ostr << (*inArg)->getSymbol().getName();
    }

    ostr << L")" << std::endl;

    FREE(wcsVarName);
    return true;
}

Callable::ReturnValue Macro::call(typed_list& in, optional_list& opt, int _iRetCount, typed_list& out)
{
    int rhs = (int)in.size();
    bool bVarargout = false;

    int iRetCount = std::max(0, _iRetCount);

    ReturnValue RetVal = Callable::OK;
    symbol::Context* pContext = symbol::Context::getInstance();

    // open a new scope
    pContext->scope_begin();
    // store the line number where is stored this macro in file.
    ConfigVariable::macroFirstLine_begin(getFirstLine());

    // check excepted and input/output parameters numbers
    //  Scilab Macro can be called with less than prototyped arguments,
    //  but not more execpts with varargin

    bool bVarargin = false;
    // varargin management
    if (m_inputArgs->size() > 0 && m_inputArgs->back()->getSymbol().getName() == L"varargin")
    {
        bVarargin = true;
        List* pL = new List();
        int iVarPos = rhs;
        if (iVarPos > static_cast<int>(m_inputArgs->size()) - 1)
        {
            iVarPos = static_cast<int>(m_inputArgs->size()) - 1;
        }

        // add variables in context or varargin list
        std::vector<symbol::Variable*>::iterator itName = m_inputArgs->begin();
        for (int i = 0; i < rhs; ++i)
        {
            if (in[i]->isListInsert())
            {
                // named
                std::wstring var(in[i]->getAs<ListInsert>()->getInsert()->getAs<String>()->get()[0]);
                if (i < iVarPos)
                {
                    pContext->put(symbol::Symbol(var), opt[var]);
                    ++itName;
                }
                else
                {
                    // varargin
                    pL->append(opt[var]);
                }
            }
            else
            {
                // context
                if (i < iVarPos)
                {
                    pContext->put(*itName, in[i]);
                    ++itName;
                }
                else
                {
                    // varargin
                    pL->append(in[i]);
                }
            }
        }

        // add varargin to macro scope
        pContext->put(m_Varargin, pL);
    }
    else if (rhs > m_inputArgs->size())
    {
        if (m_inputArgs->size() == 0)
        {
            Scierror(999, _("Wrong number of input arguments: This function has no input argument.\n"));
        }
        else
        {
            Scierror(999, _("Wrong number of input arguments.\n"));
        }

        pContext->scope_end();
        ConfigVariable::fillWhereError(getBody()->getLocation());
        ConfigVariable::macroFirstLine_end();
        return Callable::Error;
    }
    else
    {
        // assign value to variable in the new context
        std::vector<symbol::Variable*>::iterator i;
        typed_list::const_iterator j;

        for (i = m_inputArgs->begin(), j = in.begin(); j != in.end(); ++j, ++i)
        {
            if (*j && (*j)->isListInsert() == false)
            {
                // prevent assignation of NULL value
                pContext->put(*i, *j);
            }
        }

        if (m_arguments.size() != 0 && opt.size() != 0)
        {
            Scierror(999, _("%s: Named argument are not compatible with arguments block.\n"), scilab::UTF8::toUTF8(m_wstName).data());
            pContext->scope_end();
            ConfigVariable::fillWhereError(getBody()->getLocation());
            ConfigVariable::macroFirstLine_end();
            return Callable::Error;
        }

        // add optional parameters in current scope
        for (auto&& it : opt)
        {
            if (it.second)
            {
                pContext->put(symbol::Symbol(it.first), it.second);
            }
        }
    }
    /*argument checker*/
    if (m_arguments.size() != 0)
    {
        try
        {
            types::InternalType* skipArgs = symbol::Context::getInstance()->get(symbol::Symbol(L"%skipArgs"));
            if (skipArgs == nullptr)
            {
                int expectedmin = 0;
                int expectedmax = 0;
                for (auto&& a : m_arguments)
                {
                    expectedmin += a.second.default_value == nullptr ? 1 : 0;
                    expectedmax += 1;
                }

                if (in.size() < expectedmin || (bVarargin == false && in.size() > m_arguments.size()))
                {
                    char msg[128];
                    if (expectedmin != expectedmax)
                    {
                        os_sprintf(msg, _("%s: Wrong number of input arguments: %d to %d expected.\n"), scilab::UTF8::toUTF8(m_wstName).data(), expectedmin, expectedmax);
                    }
                    else
                    {
                        if (bVarargin)
                        {
                            os_sprintf(msg, _("%s: Wrong number of input argument(s): at least %d expected.\n"), scilab::UTF8::toUTF8(m_wstName).data(), (int)m_arguments.size());
                        }
                        else
                        {
                            os_sprintf(msg, _("%s: Wrong number of input argument(s): %d expected.\n"), scilab::UTF8::toUTF8(m_wstName).data(), (int)m_arguments.size());
                        }
                    }

                    throw ast::InternalError(scilab::UTF8::toWide(msg), 999, getBody()->getLocation());
                }
            }

            //manage default_value of all inputs before everything else
            for (int i = 0; i < m_inputArgs->size(); ++i)
            {
                std::wstring name = (*m_inputArgs)[i]->getSymbol().getName();
                if (m_arguments.find(name) == m_arguments.end())
                {
                    continue;
                }

                ARG arg = m_arguments[name];
                if (i >= in.size())
                {
                    if (arg.default_value)
                    {
                        ast::RunVisitor* exec = (ast::RunVisitor*)ConfigVariable::getDefaultVisitor();
                        arg.default_value->accept(*exec);
                        InternalType* pIT = exec->getResult();
                        if (pIT == nullptr || pIT->isAssignable() == false)
                        {
                            char msg[128];
                            os_sprintf(msg, _("%s: Unable to evaluate default value.\n"), scilab::UTF8::toUTF8(m_wstName).data());
                            throw ast::InternalError(scilab::UTF8::toWide(msg), 999, arg.default_value->getLocation());
                        }

                        pIT->IncreaseRef();
                        pContext->put(symbol::Symbol(name), pIT);
                        in.push_back(pIT);
                        delete exec;
                    }
                }
            }

            for (int i = 0; i < m_inputArgs->size(); ++i)
            {
                std::wstring name = (*m_inputArgs)[i]->getSymbol().getName();
                if (m_arguments.find(name) == m_arguments.end())
                {
                    continue;
                }

                ARG arg = m_arguments[name];
                if (arg.dimsConvertor)
                {
                    // check size + expand + transpose
                    types::InternalType* p = arg.dimsConvertor(in[i]);
                    if (p == nullptr)
                    {
                        if (skipArgs == nullptr)
                        {
                            char msg[128];
                            os_sprintf(msg, _("%s: Wrong size of input argument #%d: %ls expected.\n"), scilab::UTF8::toUTF8(m_wstName).data(), i + 1, arg.dimsStr().c_str());
                            throw ast::InternalError(scilab::UTF8::toWide(msg));
                        }

                        p = in[i]; // no error and send "bad formatted var to function, following 'skipArguments' status"
                    }
                    else
                    {
                        if (in[i] != p)
                        {
                            // update var
                            pContext->put(symbol::Symbol(name), p);
                        }
                    }
                }

                for (auto&& convertor : arg.convertors)
                {
                    types::InternalType* p = convertor.convertor(in[i]);
                    if (p)
                    {
                        if (arg.dimsConvertor)
                        {
                            p = arg.dimsConvertor(p);
                        }

                        pContext->put(symbol::Symbol(name), p);
                    }
                }

                if (skipArgs == nullptr)
                {
                    for (int j = 0; j < arg.validators.size(); ++j)
                    {
                        types::typed_list args;
                        for (int k = 0; k < arg.validators[j].inputs.size(); ++k)
                        {
                            int index = -1;
                            types::InternalType* val = nullptr;
                            std::tie(index, val) = arg.validators[j].inputs[k];
                            if (index != -1)
                            {
                                args.push_back(in[index]);
                            }
                            else
                            {
                                args.push_back(val);
                            }
                        }

                        int ret = arg.validators[j].validator(args);
                        if (ret != 0)
                        {
                            auto error = arg.validators[j].error;
                            auto errorArgs = arg.validators[j].errorArgs;
                            char msg[128];

                            switch (abs(std::get<1>(error)))
                            {
                                case 2:
                                    os_sprintf(msg, _(std::get<0>(error).data()), scilab::UTF8::toUTF8(m_wstName).data(), i + 1);
                                    break;
                                case 3:
                                    os_sprintf(msg, _(std::get<0>(error).data()), scilab::UTF8::toUTF8(m_wstName).data(), i + 1, errorArgs[0].data());
                                    break;
                                case 4:
                                    os_sprintf(msg, _(std::get<0>(error).data()), scilab::UTF8::toUTF8(m_wstName).data(), i + 1, errorArgs[0].data(), errorArgs[1].data());
                                    break;
                                case 5:
                                    os_sprintf(msg, _(std::get<0>(error).data()), scilab::UTF8::toUTF8(m_wstName).data(), i + 1, errorArgs[0].data(), errorArgs[1].data(), errorArgs[2].data());
                                    break;
                            }

                            throw ast::InternalError(scilab::UTF8::toWide(msg), 999, arg.loc);
                        }
                    }
                }
            }
        }
        catch (const ast::InternalError& ie)
        {
            pContext->scope_end();
            ConfigVariable::fillWhereError(ie.GetErrorLocation());
            ConfigVariable::macroFirstLine_end();
            //return types::Function::Error;
            throw ie;
        }
    }

    // varargout management
    // rules :
    // varargout is a list
    // varargout can containt more items than caller need
    // varargout must containt at leat caller needs

    if (isLambda() == false)
    {
        if (m_outputArgs->size() >= 1 && m_outputArgs->back()->getSymbol().getName() == L"varargout")
        {
            bVarargout = true;
            List* pL = new List();
            pContext->put(m_Varargout, pL);
        }

        // iRetCount = 0 is granted to the macro (as argn(0))
        // when there is no formal output argument
        // or if varargout is the only formal output argument.
        if (m_outputArgs->size() - (bVarargout ? 1 : 0) >= 1)
        {
            iRetCount = std::max(1, iRetCount);
        }
    }

    // common part with or without varargin/varargout

    // Declare nargin & nargout in function context.
    if (m_pDblArgIn->getRef() > 1)
    {
        m_pDblArgIn->DecreaseRef();
        m_pDblArgIn = m_pDblArgIn->clone();
        m_pDblArgIn->IncreaseRef();
    }
    m_pDblArgIn->set(0, static_cast<double>(rhs));

    if (m_pDblArgOut->getRef() > 1)
    {
        m_pDblArgOut->DecreaseRef();
        m_pDblArgOut = m_pDblArgOut->clone();
        m_pDblArgOut->IncreaseRef();
    }

    m_pDblArgOut->set(0, iRetCount);

    pContext->put(m_Nargin, m_pDblArgIn);
    pContext->put(m_Nargout, m_pDblArgOut);

    // add sub macro in current context
    for (auto&& sub : m_submacro)
    {
        pContext->put(sub.first, sub.second);
    }

    if (isLambda())
    {
        //add varargout in new context
        //List* pL = new List();
        //pContext->put(m_Varargout, pL);

        for (auto&& c : m_captured)
        {
            pContext->put(symbol::Symbol(c.first), c.second);
        }
    }

    // save current prompt mode
    int oldVal = ConfigVariable::getPromptMode();
    std::wstring iExecFile = ConfigVariable::getExecutedFile();
    std::unique_ptr<ast::ConstVisitor> exec(ConfigVariable::getDefaultVisitor());
    ((ast::RunVisitor*)exec.get())->setLambda(isLambda());

    try
    {
        ConfigVariable::setExecutedFile(m_stPath);
        ConfigVariable::setPromptMode(-1);
        m_body->accept(*exec);
        // restore previous prompt mode
        ConfigVariable::setPromptMode(oldVal);
        ConfigVariable::setExecutedFile(iExecFile);
    }
    catch (const ast::InternalError& ie)
    {
        if (m_arguments.size() != 0)
        {
            types::InternalType* pIT = symbol::Context::getInstance()->get(symbol::Symbol(L"%skipArgs"));
            if (pIT)
            {
                wchar_t* func = pIT->getAs<types::String>()->get()[0];
                Sciwarning("WARNING: \"skipArguments\" was called in \"%ls\".\n", func);
            }
        }

        ConfigVariable::setExecutedFile(iExecFile);
        cleanCall(pContext, oldVal);
        throw ie;
    }
    catch (const ast::InternalAbort& ia)
    {
        ConfigVariable::setExecutedFile(iExecFile);
        cleanCall(pContext, oldVal);
        throw ia;
    }

    // nb excepted output without varargout
    int iRet = iRetCount;

    if (isLambda() == false)
    {
        iRet = std::min((int)m_outputArgs->size() - (bVarargout ? 1 : 0), std::max(1, iRetCount));

        // normal output management
        for (auto arg : *m_outputArgs)
        {
            iRet--;
            if (iRet < 0)
            {
                break;
            }

            InternalType* pIT = pContext->get(arg);
            if (pIT)
            {
                out.push_back(pIT);
                pIT->IncreaseRef();
            }
            else
            {
                const int size = (const int)out.size();
                for (int j = 0; j < size; ++j)
                {
                    out[j]->DecreaseRef();
                    out[j]->killMe();
                }
                out.clear();
                cleanCall(pContext, oldVal);

                char* pstArgName = wide_string_to_UTF8(arg->getSymbol().getName().c_str());
                char* pstMacroName = wide_string_to_UTF8(getName().c_str());
                Scierror(999, _("Undefined variable '%s' in function '%s'.\n"), pstArgName, pstMacroName);
                FREE(pstArgName);
                FREE(pstMacroName);
                return Callable::Error;
            }
        }

        // varargout management
        if (bVarargout)
        {
            InternalType* pOut = pContext->get(m_Varargout);
            if (pOut == NULL)
            {
                cleanCall(pContext, oldVal);
                Scierror(999, _("Invalid index.\n"));
                return Callable::Error;
            }

            if (pOut->isList() == false)
            {
                cleanCall(pContext, oldVal);
                char* pstMacroName = wide_string_to_UTF8(getName().c_str());
                Scierror(999, _("%s: Wrong type for %s: A list expected.\n"), pstMacroName, "Varargout");
                FREE(pstMacroName);
                return Callable::Error;
            }

            List* pVarOut = pOut->getAs<List>();
            const int size = std::min(pVarOut->getSize(), std::max(1, iRetCount) - (int)out.size());
            for (int i = 0; i < size; ++i)
            {
                InternalType* pIT = pVarOut->get(i);
                if (pIT->isVoid())
                {
                    for (int j = 0; j < i; ++j)
                    {
                        out[j]->DecreaseRef();
                        out[j]->killMe();
                    }
                    out.clear();
                    cleanCall(pContext, oldVal);

                    Scierror(999, _("List element number %d is Undefined.\n"), i + 1);
                    return Callable::Error;
                }

                pIT->IncreaseRef();
                out.push_back(pIT);
            }
        }
    }
    else
    {
        InternalType* pOut = pContext->get(m_Varargout);
        if (pOut == NULL)
        {
            types::InternalType* result = ((ast::RunVisitor*)exec.get())->getLambdaResult();
            if (result)
            {
                types::InternalType* p = result;
                p->IncreaseRef();
                out.push_back(p);
                ((ast::RunVisitor*)exec.get())->clearLambdaResult();
            }
        }
        else
        {
            if (pOut->isList() == false)
            {
                cleanCall(pContext, oldVal);
                char* pstMacroName = wide_string_to_UTF8(getName().c_str());
                Scierror(999, _("%s: Wrong type for %s: A list expected.\n"), pstMacroName, "Varargout");
                FREE(pstMacroName);
                return Callable::Error;
            }

            List* pVarOut = pOut->getAs<List>();
            const int size = std::min(pVarOut->getSize(), iRetCount);
            for (int i = 0; i < size; ++i)
            {
                types::InternalType* p = pVarOut->get(i);
                p->IncreaseRef();
                out.push_back(p);
            }
        }
    }

    // close the current scope
    cleanCall(pContext, oldVal);

    //reduce ref of outputs to case of in and out have same symbol
    for (typed_list::iterator i = out.begin(), end = out.end(); i != end; ++i)
    {
        (*i)->DecreaseRef();
    }

    return RetVal;
}

std::vector<symbol::Variable*>* Macro::getInputs()
{
    return m_inputArgs;
}

std::vector<symbol::Variable*>* Macro::getOutputs()
{
    return m_outputArgs;
}

int Macro::getNbInputArgument(void)
{
    return (int)m_inputArgs->size();
}

int Macro::getNbOutputArgument(void)
{
    if (isLambda())
    {
        return -1;//will be manage later in call()
    }

    if (m_outputArgs->size() >= 1 && m_outputArgs->back()->getSymbol().getName() == L"varargout")
    {
        return -1;
    }

    return (int)m_outputArgs->size();
}

bool Macro::getMemory(long long* _piSize, long long* _piSizePlusType)
{
    ast::SerializeVisitor serialMacro(m_body);
    unsigned char* macroSerial = serialMacro.serialize(false, false);
    unsigned int macroSize = *((unsigned int*)macroSerial);

    *_piSize = macroSize;
    *_piSizePlusType = *_piSize + sizeof(Macro);
    return true;
}

bool Macro::operator==(const InternalType& it)
{
    if (const_cast<InternalType&>(it).isMacro() == false)
    {
        return false;
    }

    std::vector<symbol::Variable*>* pInput = NULL;
    std::vector<symbol::Variable*>* pOutput = NULL;
    types::Macro* pRight = const_cast<InternalType&>(it).getAs<types::Macro>();

    if (pRight->isLambda() != isLambda())
    {
        return false;
    }

    // check inputs
    pInput = pRight->getInputs();
    if (pInput->size() != m_inputArgs->size())
    {
        return false;
    }

    std::vector<symbol::Variable*>::iterator itOld = pInput->begin();
    std::vector<symbol::Variable*>::iterator itEndOld = pInput->end();
    std::vector<symbol::Variable*>::iterator itMacro = m_inputArgs->begin();

    for (; itOld != itEndOld; ++itOld, ++itMacro)
    {
        if ((*itOld)->getSymbol() != (*itMacro)->getSymbol())
        {
            return false;
        }
    }

    if (isLambda() == false)
    {
        // check outputs
        pOutput = pRight->getOutputs();
        if (pOutput->size() != m_outputArgs->size())
        {
            return false;
        }

        itOld = pOutput->begin();
        itEndOld = pOutput->end();
        itMacro = m_outputArgs->begin();

        for (; itOld != itEndOld; ++itOld, ++itMacro)
        {
            if ((*itOld)->getSymbol() != (*itMacro)->getSymbol())
            {
                return false;
            }
        }
    }

    ast::Exp* pExp = pRight->getBody();
    ast::SerializeVisitor serialOld(pExp);
    unsigned char* oldSerial = serialOld.serialize(false, false);
    ast::SerializeVisitor serialMacro(m_body);
    unsigned char* macroSerial = serialMacro.serialize(false, false);

    // check buffer length
    unsigned int oldSize = *((unsigned int*)oldSerial);
    unsigned int macroSize = *((unsigned int*)macroSerial);
    if (oldSize != macroSize)
    {
        return false;
    }

    bool ret = (memcmp(oldSerial, macroSerial, oldSize) == 0);
    return ret;
}

void Macro::add_submacro(const symbol::Symbol& s, Macro* macro)
{
    macro->IncreaseRef();
    symbol::Context* ctx = symbol::Context::getInstance();
    symbol::Variable* var = ctx->getOrCreate(s);
    m_submacro[var] = macro;
}

void Macro::updateArguments()
{
    // build a map of inputs argument name and position
    std::vector<std::wstring> inputNames;
    for (auto&& in : *m_inputArgs)
    {
        inputNames.push_back(in->getSymbol().getName());
    }

    bool needDefaultValue = false;
    bool bvarargin = false;
    for (auto&& e : m_body->getExps())
    {
        if (e->isCommentExp()) continue;

        if (e->isArgumentsExp())
        {
            for (int  i = 0; i < e->getExps().size(); ++i)
            {
                ast::Exp* d = e->getExps()[i];
                if (d->isCommentExp()) continue;

                ast::ArgumentDec* dec = d->getAs<ast::ArgumentDec>();
                std::wstring name;
                if (dec->getArgumentName()->isSimpleVar())
                {
                    std::vector<std::wstring> allowedVar = {L"%eps", L"%i", L"%inf", L"%nan"};
                    name = dec->getArgumentName()->getAs<ast::SimpleVar>()->getSymbol().getName();
                    if (std::find(allowedVar.begin(), allowedVar.end(), name) == allowedVar.end())
                    {
                        if (m_arguments.size() >= inputNames.size() || inputNames[m_arguments.size()] != name)
                        {
                            char msg[128];
                            if (std::find(inputNames.begin(), inputNames.end(), name) == inputNames.end())
                            {
                                os_sprintf(msg, _("%s: Identifier '%s' must be an input argument.\n"), scilab::UTF8::toUTF8(m_wstName).data(), scilab::UTF8::toUTF8(name).data());
                            }
                            else
                            {
                                os_sprintf(msg, _("%s: Identifier must be define in same order that parameters.\n"), scilab::UTF8::toUTF8(m_wstName).data());
                            }

                            throw ast::InternalError(scilab::UTF8::toWide(msg), 999, dec->getArgumentType()->getLocation());
                        }
                    }
                }
                else // FieldExp
                {
                    /*
                    const ast::FieldExp* f = dec->getArgumentName()->getAs<ast::FieldExp>();
                    name = f->getHead()->getAs<ast::SimpleVar>()->getSymbol().getName();
                    if (m_arguments.size() >= inputNames.size() || inputNames[m_arguments.size()] != name)
                    {
                        char msg[128];
                        if (std::find(inputNames.begin(), inputNames.end(), name) == inputNames.end())
                        {
                            os_sprintf(msg, _("%s: Identifier '%s' must be an input argument.\n"), scilab::UTF8::toUTF8(m_wstName).data(), scilab::UTF8::toUTF8(name).data());
                        }
                        else
                        {
                            os_sprintf(msg, _("%s: Identifier must be define in same order that parameters.\n"), scilab::UTF8::toUTF8(m_wstName).data());
                        }

                        throw ast::InternalError(scilab::UTF8::toWide(msg), 999, dec->getArgumentType()->getLocation());
                    }

                    name += L".";
                    name += f->getTail()->getAs<ast::SimpleVar>()->getSymbol().getName();
                    */

                    char msg[128];
                    os_sprintf(msg, _("%s: Expression with field are not managed.\n"), "arguments");
                    throw ast::InternalError(scilab::UTF8::toWide(msg), 999, dec->getArgumentType()->getLocation());
                }

                if (name == L"varargin")
                {
                    //check that there is no information !
                    if (dec->getArgumentDims()->getExps().size() != 0 ||
                        dec->getArgumentDefaultValue()->getExps().size() != 0 ||
                        dec->getArgumentType()->getExps().size() != 0 ||
                        dec->getArgumentValidators()->getExps().size() != 0)
                    {
                        char msg[128];
                        os_sprintf(msg, _("%s: varargin must be declared without parameter.\n"), "arguments");
                        throw ast::InternalError(scilab::UTF8::toWide(msg), 999, dec->getArgumentType()->getLocation());
                    }

                    bvarargin = true;
                    continue;
                }


                ARG arg;
                arg.loc = d->getLocation();

                // dims
                std::vector<std::tuple<std::vector<int>, symbol::Variable*>> dims = {};
                for (auto&& dim : dec->getArgumentDims()->getExps())
                {
                    // TODO
                    if (dim->isSimpleVar())
                    {
                        std::wstring name = dim->getAs<ast::SimpleVar>()->getSymbol().getName();
                        if (std::find(inputNames.begin(), inputNames.end(), name) == inputNames.end())
                        {
                            char msg[128];
                            os_sprintf(msg, _("%s: Dimension must be an input parameter.\n"), scilab::UTF8::toUTF8(m_wstName).data());
                            throw ast::InternalError(scilab::UTF8::toWide(msg), 999, dec->getArgumentType()->getLocation());
                        }

                        symbol::Variable* var = symbol::Context::getInstance()->getOrCreate(dim->getAs<ast::SimpleVar>()->getSymbol());
                        dims.push_back({{-1}, var});
                    }
                    else if (dim->isColonVar())
                    {
                        dims.push_back({{-1}, nullptr});
                    }
                    else if (dim->isDoubleExp())
                    {
                        dims.push_back({{static_cast<int>(dim->getAs<ast::DoubleExp>()->getValue())}, nullptr});
                    }
                    else if (dim->isMatrixExp())
                    {
                        std::vector<int> d;
                        //allow only one line matrix
                        ast::MatrixExp* m = dim->getAs<ast::MatrixExp>();
                        if (m->getLines().size() != 1)
                        {
                            char msg[128];
                            os_sprintf(msg, _("%s: Dimension must be a number, row vector or ':'.\n"), scilab::UTF8::toUTF8(m_wstName).data());
                            throw ast::InternalError(scilab::UTF8::toWide(msg), 999, dec->getArgumentType()->getLocation());
                        }

                        ast::MatrixLineExp* ml = m->getLines()[0]->getAs<ast::MatrixLineExp>();
                        for (auto&& c : ml->getColumns())
                        {
                            if (c->isDoubleExp() == false)
                            {
                                char msg[128];
                                os_sprintf(msg, _("%s: Dimension must be a number, row vector or ':'.\n"), scilab::UTF8::toUTF8(m_wstName).data());
                                throw ast::InternalError(scilab::UTF8::toWide(msg), 999, dec->getArgumentType()->getLocation());
                            }

                            d.push_back(static_cast<int>(c->getAs<ast::DoubleExp>()->getValue()));
                        }

                        dims.push_back({d, nullptr});
                    }
                    else
                    {
                        char msg[128];
                        os_sprintf(msg, _("%s: Dimension must be a number, row vector or ':'.\n"), scilab::UTF8::toUTF8(m_wstName).data());
                        throw ast::InternalError(scilab::UTF8::toWide(msg), 999, dec->getArgumentType()->getLocation());
                    }
                }

                arg.dimsConvertor = nullptr;
                if (dims.size() != 0)
                {
                    bool isStatic = checkStaticDims(dims);

                    arg.dimsConvertor = [dims, isStatic](types::InternalType* x) { return checksize(x, dims, isStatic); };
                    arg.dimsStr = [dims](){ return dims2str(dims); };
                }

                // conversion
                if (dec->getArgumentType()->isSimpleVar())
                {
                    ARG_CONVERTOR argConv;
                    std::wstring name = dec->getArgumentType()->getAs<ast::SimpleVar>()->getSymbol().getName();
                    auto f = typeConvertors[name];
                    if (f == nullptr)
                    {
                        char msg[128];
                        os_sprintf(msg, _("%s: Unknown conversion function '%s'\n"), scilab::UTF8::toUTF8(m_wstName).data(), scilab::UTF8::toUTF8(name).data());
                        throw ast::InternalError(scilab::UTF8::toWide(msg), 999, dec->getArgumentType()->getLocation());
                    }

                    std::wstring callerName(m_wstName);
                    argConv.convertor = [f, callerName](types::InternalType* x) { return f(x, callerName); };
                    arg.convertors.push_back(argConv);
                }

                // default value
                if (dec->getArgumentDefaultValue()->isNilExp() == false)
                {
                    needDefaultValue = true;
                    arg.default_value = dec->getArgumentDefaultValue();
                }
                else if (needDefaultValue)
                {
                    char msg[128];
                    os_sprintf(msg, _("%s: Identifier '%s' needs a default value.\n"), scilab::UTF8::toUTF8(m_wstName).data(), scilab::UTF8::toUTF8(name).data());
                    throw ast::InternalError(scilab::UTF8::toWide(msg), 999, dec->getArgumentType()->getLocation());
                }

                // validators
                for (auto&& v : dec->getArgumentValidators()->getExps())
                {
                    if (v->isSimpleVar())
                    {
                        std::vector<int> rhs;
                        std::wstring name = v->getAs<ast::SimpleVar>()->getSymbol().getName();
                        std::function<int(typed_list&)> f;
                        std::tie(f, rhs) = functionValidators[name];

                        if (f == nullptr)
                        {
                            char msg[128];
                            os_sprintf(msg, _("%s: Unknown validation function '%s'\n"), scilab::UTF8::toUTF8(m_wstName).data(), scilab::UTF8::toUTF8(name).data());
                            throw ast::InternalError(scilab::UTF8::toWide(msg), 999, dec->getArgumentType()->getLocation());
                        }

                        if (rhs[0] != 1)
                        {
                            char msg[128];
                            os_sprintf(msg, _("%s: Wrong number of input argument(s): %d expected.\n"), scilab::UTF8::toUTF8(m_wstName).data(), 1);
                            throw ast::InternalError(scilab::UTF8::toWide(msg), 999, dec->getArgumentType()->getLocation());
                        }

                        ARG_VALIDATOR argValidator;
                        argValidator.validator = f;
                        argValidator.inputs.push_back({i, nullptr});

                        argValidator.error = errorValidators[name];
                        auto args = errorArgs[name];
                        for (int k = 0; k < args.size(); ++k)
                        {
                            if (std::get<0>(args[k]) != -1)
                            {
                                //never occur
                                sciprint("arg != -1");
                                argValidator.errorArgs.push_back("arg != -1");
                            }
                            else
                            {
                                argValidator.errorArgs.push_back(std::get<1>(args[k]));
                            }
                        }

                        arg.validators.push_back(argValidator);
                    }
                    else // CallExp
                    {
                        ast::CallExp* c = v->getAs<ast::CallExp>();
                        std::vector<int> rhs;
                        std::wstring name = (&c->getName())->getAs<ast::SimpleVar>()->getSymbol().getName();
                        std::function<int(typed_list&)> f;
                        std::tie(f, rhs) = functionValidators[name];
                        int size = static_cast<int>(c->getArgs().size());

                        if (f == nullptr)
                        {
                            char msg[128];
                            os_sprintf(msg, _("%s: \"%ls\" is not a validation function.\n"), scilab::UTF8::toUTF8(m_wstName).data(), name.c_str());
                            throw ast::InternalError(scilab::UTF8::toWide(msg), 999, dec->getArgumentType()->getLocation());
                        }

                        if (rhs.size() == 1)
                        {
                            if (rhs[0] != size && rhs[0] != -1)
                            {
                                char msg[128];
                                os_sprintf(msg, _("%s: Wrong number of input argument(s): %d expected.\n"), scilab::UTF8::toUTF8(m_wstName).data(), rhs[0]);
                                throw ast::InternalError(scilab::UTF8::toWide(msg), 999, dec->getArgumentType()->getLocation());
                            }
                        }
                        else
                        {
                            if (size < rhs[0] || size > rhs[1])
                            {
                                char msg[128];
                                os_sprintf(msg, _("%s: Wrong number of input argument(s): between %d and %d expected.\n"), scilab::UTF8::toUTF8(m_wstName).data(), rhs[0], rhs[1]);
                                throw ast::InternalError(scilab::UTF8::toWide(msg), 999, dec->getArgumentType()->getLocation());
                            }
                        }

                        ARG_VALIDATOR argValidator;
                        argValidator.validator = f;
                        ast::exps_t inputs = c->getArgs();
                        for (int i = 0; i < size; ++i)
                        {
                            if (inputs[i]->isSimpleVar())
                            {
                                std::vector<std::wstring> allowedVar = {L"%eps", L"%i", L"%inf", L"%nan"};
                                std::wstring name = inputs[i]->getAs<ast::SimpleVar>()->getSymbol().getName();
                                if (std::find(allowedVar.begin(), allowedVar.end(), name) == allowedVar.end())
                                {
                                    if (std::find(inputNames.begin(), inputNames.end(), name) == inputNames.end())
                                    {
                                        char msg[128];
                                        os_sprintf(msg, _("%s: Identifier '%s' must be an input argument.\n"), scilab::UTF8::toUTF8(m_wstName).data(), scilab::UTF8::toUTF8(name).data());
                                        throw ast::InternalError(scilab::UTF8::toWide(msg), 999, dec->getArgumentType()->getLocation());
                                    }

                                    int pos = static_cast<int>(std::find(inputNames.begin(), inputNames.end(), inputs[i]->getAs<ast::SimpleVar>()->getSymbol().getName()) - inputNames.begin());
                                    argValidator.inputs.push_back({pos, nullptr});
                                }
                                else
                                {
                                    argValidator.inputs.push_back({-1, symbol::Context::getInstance()->get(symbol::Symbol(name))});
                                }
                            }
                            else // constant
                            {
                                if (checkArgument(inputs[i]) == false)
                                {
                                    char msg[128];
                                    os_sprintf(msg, _("%s: argument must be constant expression.\n"), scilab::UTF8::toUTF8(m_wstName).data());
                                    throw ast::InternalError(scilab::UTF8::toWide(msg), 999, dec->getArgumentType()->getLocation());
                                }

                                ast::RunVisitor* exec = (ast::RunVisitor*)ConfigVariable::getDefaultVisitor();
                                try
                                {
                                    inputs[i]->accept(*exec);
                                }
                                catch (const ast::InternalError& /*ie*/)
                                {
                                    char msg[128];
                                    os_sprintf(msg, _("%s: Unable to evaluate argument.\n"), scilab::UTF8::toUTF8(m_wstName).data());
                                    throw ast::InternalError(scilab::UTF8::toWide(msg), 999, inputs[i]->getLocation());
                                }

                                types::InternalType* pIT = exec->getResult();
                                if (pIT == nullptr || pIT->isAssignable() == false)
                                {
                                    char msg[128];
                                    os_sprintf(msg, _("%s: Unable to evaluate argument.\n"), scilab::UTF8::toUTF8(m_wstName).data());
                                    throw ast::InternalError(scilab::UTF8::toWide(msg), 999, inputs[i]->getLocation());
                                }

                                pIT->IncreaseRef();
                                argValidator.inputs.push_back({-1, pIT});
                                delete exec;
                            }
                        }

                        argValidator.error = errorValidators[name];
                        auto args = errorArgs[name];
                        for (int k = 0; k < args.size(); ++k)
                        {
                            
                            if (std::get<0>(args[k]) != -1) //not a constant string
                            {
                                types::InternalType* pIT = std::get<1>(argValidator.inputs[std::get<0>(args[k])]);
                                if (pIT) //data from variable
                                {
                                    std::wostringstream ostr;
                                    std::wstring output;
                                    types::InternalType* pIT = std::get<1>(argValidator.inputs[std::get<0>(args[k])]);
                                    typed_list in = {pIT};
                                    optional_list opt;
                                    typed_list out;
                                    types::InternalType* pCall = symbol::Context::getInstance()->get(symbol::Symbol(L"sci2exp"));
                                    if (pCall && pCall->isCallable())
                                    {
                                        if (pCall->getAs<types::Callable>()->call(in, opt, 1, out) == types::Function::OK)
                                        {
                                            if (out.size() == 1 && out[0]->isString())
                                            {
                                                output = out[0]->getAs<types::String>()->get()[0];
                                                if (pIT->isGenericType() == false || pIT->getAs<types::GenericType>()->getSize() == 1)
                                                {
                                                    // output = L"[" + output + L"]";
                                                }

                                                pIT = nullptr;
                                            }
                                        }
                                    }

                                    if (pIT)
                                    {
                                        pIT->toString(ostr);
                                        output = ostr.str();
                                    }

                                    argValidator.errorArgs.push_back(scilab::UTF8::toUTF8(output));
                                }
                                else //use position of variable as message information
                                {
                                    int pos = std::get<0>(argValidator.inputs[std::get<0>(args[k])]) + 1;
                                    argValidator.errorArgs.push_back(std::to_string(pos));
                                }
                            }
                            else
                            {
                                argValidator.errorArgs.push_back(std::get<1>(args[k]));
                            }
                        }

                        arg.validators.push_back(argValidator);
                    }
                }

                m_arguments[name] = arg;
            } //for

            if (m_arguments.size() + (bvarargin ? 1 : 0) != m_inputArgs->size())
            {
                char msg[128];
                os_sprintf(msg, _("%s: All parameters must be specified in arguments block.\n"), scilab::UTF8::toUTF8(m_wstName).data());
                throw ast::InternalError(scilab::UTF8::toWide(msg), 999, e->getLocation());
            }
        }
    }
}

bool Macro::checkArgument(ast::Exp* exp)
{
    ast::ArgumentVisitor v;
    exp->accept(v);
    return v.getStatus();
}

bool Macro::checkStaticDims(const std::vector<std::tuple<std::vector<int>, symbol::Variable*>>& dims)
{
    bool res = true;
    for (auto&& d : dims)
    {
        if (std::get<1>(d) == nullptr)
        {
            return false;
        }

        if (std::get<0>(d).size() != 1)
        {
            return false;
        }
    }

    return res;
}
} // namespace types
