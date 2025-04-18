/*
* Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2014 - Scilab Enterprises - Antoine ELIAS
* Copyright (C) 2015 - Scilab Enterprises - Sylvain GENIN
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

#include <string.h>
#include "function.hxx"
#include "io_gw.hxx"
#include "string.hxx"
#include "double.hxx"
#include "int.hxx"
#include "filemanager.hxx"
#include "readwrite.hxx"

extern "C"
{
#include "sci_malloc.h"
#include "Scierror.h"
#include "expandPathVariable.h"

    extern int C2F(clunit)(int*, char const*, int*, int);

    extern int C2F(readdoublefile)(int* ID, double* dat, int* m, int* n, int* err);
    extern int C2F(readdoublefileform)(int* ID, char* form, double* dat, int* m, int* n, int* err, size_t);
    extern int C2F(readdoublelinefile)(int* ID, double* dat, int* n, int* err);
    extern int C2F(readdoublelinefileform)(int* ID, char* form, double* dat, int* n, int* err, size_t);

    extern int C2F(readintfileform)(int* ID, char* form, int* dat, int* m, int* n, int* err, size_t);
    extern int C2F(readintlinefileform)(int* ID, char* form, int* dat, int* n, int* err, size_t);

    extern int C2F(readstringfile)(int* ID, char* form, char* dat, int* siz, int* err, size_t);
    extern int C2F(readstring)(char* form, char* dat, int* siz, int* err, size_t);

}

template<typename T>
bool is_of_type(const std::string & Str)
{
    std::istringstream iss(Str);

    T tmp;
    return (iss >> tmp) && (iss.eof());
}

/*--------------------------------------------------------------------------*/
types::Function::ReturnValue sci_read(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    int iID = 0;
    int iRhs = static_cast<int>(in.size());
    char* pstFormat = NULL;
    types::InternalType::ScilabType itTypeOfData = types::InternalType::ScilabDouble;

    if (iRhs < 3 || iRhs > 5)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d to %d expected.\n"), "read", 2, 4);
        return types::Function::Error;
    }

    //file
    if (in[0]->isString())
    {
        types::String* pSPath = in[0]->getAs<types::String>();

        if (pSPath->isScalar() == false)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d : string expected.\n"), "read", 1);
            return types::Function::Error;
        }

        int piMode[2] = { -1, 0};
        char* pstFilename = wide_string_to_UTF8(pSPath->get(0));
        int iErr = C2F(clunit)(&iID, pstFilename, piMode, static_cast<int>(strlen(pstFilename)));

        if (iErr == 240)
        {
            closeFile(in[0], iID);
            Scierror(999, _("File \"%s\" already exists or directory write access denied.\n"), pstFilename);
            FREE(pstFilename);
            return types::Function::Error;
        }

        if (iErr == 241)
        {
            closeFile(in[0], iID);
            Scierror(999, _("File \"%s\" does not exist or read access denied.\n"), pstFilename);
            FREE(pstFilename);
            return types::Function::Error;
        }

        FREE(pstFilename);
    }
    else if (in[0]->isDouble())
    {
        types::Double* pDId = in[0]->getAs<types::Double>();
        if (pDId->isScalar() == false)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d : A real scalar expected.\n"), "read", 1);
            return types::Function::Error;
        }

        iID = (int)pDId->get(0);
        if (iID == -1)
        {
            iID = FileManager::getCurrentFile();
        }
    }
    else
    {
        Scierror(999, _("%s: Wrong type for input argument #%d : A real scalar or file descriptor expected.\n"), "read", 1);
        return types::Function::Error;
    }

    if (iRhs > 3)
    {
        int iPos = iRhs - 1;
        if (in[iPos]->isString() == false)
        {
            closeFile(in[0], iID);
            Scierror(999, _("%s: Wrong type for input argument #%d : string expected.\n"), "read", iRhs);
            return types::Function::Error;
        }

        types::String* pSFormat = in[iPos]->getAs<types::String>();
        if (pSFormat->isScalar() == false)
        {
            closeFile(in[0], iID);
            Scierror(999, _("%s: Wrong type for input argument #%d : string expected.\n"), "read", iRhs);
            return types::Function::Error;
        }

        //checkformat
        pstFormat = wide_string_to_UTF8(pSFormat->get(0));

        itTypeOfData = checkformat(pstFormat);
        if (itTypeOfData == types::InternalType::ScilabNull)
        {
            FREE(pstFormat);
            closeFile(in[0], iID);
            Scierror(999, _("Incorrect file or format.\n"));
            return types::Function::Error;
        }
    }

    int error = 0;

    if (in[1]->isDouble() == false)
    {
        if (pstFormat)
        {
            FREE(pstFormat);
        }

        closeFile(in[0], iID);
        Scierror(999, _("%s: Wrong type for input argument #%d: A scalar integer value expected.\n"), "read", 2);
        return types::Function::Error;
    }

    types::Double* pIn1 = in[1]->getAs<types::Double>();
    if (pIn1->isScalar() == false)
    {
        if (pstFormat)
        {
            FREE(pstFormat);
        }
        closeFile(in[0], iID);
        Scierror(999, _("%s: Wrong size for input argument #%d: A scalar integer value expected.\n"), "read", 2);
        return types::Function::Error;
    }

    if (in[2]->isDouble() == false)
    {
        if (pstFormat)
        {
            FREE(pstFormat);
        }
        closeFile(in[0], iID);
        Scierror(999, _("%s: Wrong type for input argument #%d: A scalar integer value expected.\n"), "read", 3);
        return types::Function::Error;
    }

    types::Double* pIn2 = in[2]->getAs<types::Double>();
    if (pIn2->isScalar() == false)
    {
        if (pstFormat)
        {
            FREE(pstFormat);
        }
        closeFile(in[0], iID);
        Scierror(999, _("%s: Wrong size for input argument #%d: A scalar integer value expected.\n"), "read", 3);
        return types::Function::Error;
    }

    int iRows = (int)pIn1->get(0);
    int iCols = (int)pIn2->get(0);

    //test dims
    if ( (iCols <= 0) || (iRows == 0))
    {
        if (pstFormat)
        {
            FREE(pstFormat);
        }
        out.push_back(types::Double::Empty());
        closeFile(in[0], iID);
        return types::Function::OK;
    }

    if (iRows < 0)
    {
        if (iID != 5 /*stdin*/)
        {
            switch (itTypeOfData)
            {
                case types::InternalType::ScilabDouble:
                {
                    std::vector<std::vector<double>> all;
                    std::vector<double> line(iCols);

                    if (pstFormat == NULL)
                    {
                        while (error == 0)
                        {
                            C2F(readdoublelinefile)(&iID, line.data(), &iCols, &error);
                            if (error == 0)
                            {
                                all.push_back(line);
                            }
                        }
                    }
                    else
                    {
                        while (error == 0)
                        {
                            C2F(readdoublelinefileform)(&iID, pstFormat, line.data(), &iCols, &error, strlen(pstFormat));
                            if (error == 0)
                            {
                                all.push_back(line);
                            }
                        }
                    }

                    if (error != 2)
                    {
                        iRows = static_cast<int>(all.size());
                        types::Double* pD = new types::Double(iRows, iCols);
                        double* p = pD->get();

                        for (int i = 0; i < iRows; ++i)
                        {
                            for (int j = 0; j < iCols; ++j)
                            {
                                p[j * iRows + i] = all[i][j];
                            }
                        }

                        out.push_back(pD);
                    }
                }
                break;
                case types::InternalType::ScilabInt32:
                {
                    std::vector<std::vector<int>> all;
                    std::vector<int> line(iCols);

                    int* piData = new int[iCols];
                    while (error == 0)
                    {
                        C2F(readintlinefileform)(&iID, pstFormat, line.data(), &iCols, &error, strlen(pstFormat));
                        if (error == 0)
                        {
                            for (int i = 0; i < iCols; ++i)
                            {
                                all.push_back(line);
                            }
                        }
                    }

                    delete[] piData;

                    if (error != 2)
                    {
                        iRows = static_cast<int>(all.size());
                        types::Int32* pI = new types::Int32(iRows, iCols);
                        int* p = pI->get();

                        for (int i = 0; i < iRows; ++i)
                        {
                            for (int j = 0; j < iCols; ++j)
                            {
                                p[j * iRows + i] = all[i][j];
                            }
                        }

                        out.push_back(pI);
                    }
                }
                break;
                case types::InternalType::ScilabString:
                {
                    if (iCols != 1)
                    {
                        if (pstFormat)
                        {
                            FREE(pstFormat);
                        }
                        closeFile(in[0], iID);
                        Scierror(999, _("%s: Wrong input argument %d.\n"), "read", 3);
                        return types::Function::Error;
                    }
                    else
                    {
                        char pCt[4096];
                        std::vector<std::string> all;
                        while (error == 0)
                        {
                            int siz = 0;
                            C2F(readstringfile)(&iID, pstFormat, pCt, &siz, &error, strlen(pstFormat));
                            pCt[siz] = '\0';
                            if (error == 0)
                            {
                                all.push_back(pCt);
                            }
                        }

                        if (error != 2)
                        {
                            iRows = static_cast<int>(all.size());
                            // on empty file, data are not set
                            if (iRows == 0)
                            {
                                out.push_back(types::Double::Empty());
                            }
                            else
                            {
                                types::String* pS = new types::String(iRows, iCols);
                                for (int i = 0; i < iRows; ++i)
                                {
                                    pS->set(i, all[i].data());
                                }
                                
                                out.push_back(pS);
                            }
                        }
                    }
                }
                break;
                default:
                {
                    if (pstFormat)
                    {
                        FREE(pstFormat);
                    }
                    closeFile(in[0], iID);
                    Scierror(999, _("%s: Wrong type for input argument #%d : string expected.\n"), "read", 2);
                    return types::Function::Error;
                }
            }

            closeFile(in[0], iID);
        }
        else//read from the console
        {
            switch (itTypeOfData)
            {
                case types::InternalType::ScilabDouble:
                {
                    iRows = 1;
                    types::Double* pD = new types::Double(iRows, iCols, false);

                    char pstString[4] = "(a)";
                    char pCt[4096];

                    while (error != 2)
                    {
                        int siz = 0;
                        C2F(readstring)(pstString, pCt, &siz, &error, strlen(pstString));
                        pCt[siz] = '\0';

                        if ((siz == 1) && (pCt[0] == ' '))
                        {
                            break;
                        }

                        char* pch;
                        int iColsTempo = 0;
                        pch = strtok(pCt, " ");
                        double* pdData = new double[iCols];
                        while ((pch != NULL) && (error == 0) && (iColsTempo < iCols))
                        {
                            if (is_of_type<double>(pch))
                            {
                                pdData[iColsTempo] = atof(pch);
                                iColsTempo++;
                            }
                            else
                            {
                                error = 2;
                            }
                            pch = strtok(NULL, " ");
                        }

                        if ((siz == 1) && (iColsTempo == 0))
                        {
                            delete[] pdData;
                            break;
                        }
                        if (iColsTempo != iCols)
                        {
                            delete[] pdData;
                            delete pD;
                            if (pstFormat)
                            {
                                FREE(pstFormat);
                            }

                            Scierror(999, _("%s: Input missing from console.\n"), "read");
                            return types::Function::Error;
                        }
                        else
                        {
                            pD->resize(iRows, iCols);
                            for (int i = 0; i < iCols; i++)
                            {
                                pD->set((iRows - 1), i, pdData[i]);
                            }
                            delete[] pdData;
                        }

                        //next line
                        ++iRows;
                    }

                    if (error == 0)
                    {
                        out.push_back(pD);
                    }
                }
                break;
                case types::InternalType::ScilabInt32:
                {
                    if (pstFormat)
                    {
                        FREE(pstFormat);
                    }

                    Scierror(999, _("Incorrect file or format.\n"));
                    return types::Function::Error;
                }
                case types::InternalType::ScilabString:
                {
                    if (iCols != 1)
                    {
                        if (pstFormat)
                        {
                            FREE(pstFormat);
                        }

                        Scierror(999, _("%s: Wrong input argument %d.\n"), "read", 3);
                        return types::Function::Error;
                    }
                    else
                    {
                        bool bEndWrite = false;
                        iRows = 1;
                        types::String* pS = new types::String(iRows, iCols);
                        char pCt[4096];

                        for (; bEndWrite == false; iRows++)
                        {
                            int siz = 0;
                            C2F(readstring)(pstFormat, pCt, &siz, &error, strlen(pstFormat));
                            pCt[siz] = '\0';

                            if ((siz == 1) && (pCt[0] == ' '))
                            {
                                bEndWrite = true;
                            }
                            else
                            {
                                pS->resize(iRows, iCols);
                                pS->set((iRows - 1), (iCols - 1), pCt);
                            }
                        }

                        if (error == 0)
                        {
                            out.push_back(pS);
                        }
                        else
                        {
                            delete pS;
                        }
                    }
                }
                break;
                default:
                    break;
            }
        }
    }
    else
    {
        if (iID != 5 /*stdin*/)
        {
            switch (itTypeOfData)
            {
                case types::InternalType::ScilabDouble:
                {
                    types::Double* pD = new types::Double(iRows, iCols, false);
                    double* pd = pD->get();

                    if (pstFormat == NULL)
                    {
                        C2F(readdoublefile)(&iID, pd, &iRows, &iCols, &error);
                    }
                    else
                    {
                        C2F(readdoublefileform)(&iID, pstFormat, pd, &iRows, &iCols, &error, strlen(pstFormat));
                    }

                    if (error == 0)
                    {
                        out.push_back(pD);
                    }
                    else
                    {
                        delete pD;
                    }
                }
                break;
                case types::InternalType::ScilabInt32:
                {
                    types::Int32* pI = new types::Int32(iRows, iCols);
                    int* pi = pI->get();

                    C2F(readintfileform)(&iID, pstFormat, pi, &iRows, &iCols, &error, strlen(pstFormat));

                    if (error == 0)
                    {
                        out.push_back(pI);
                    }
                    else
                    {
                        delete pI;
                    }
                }
                break;
                case types::InternalType::ScilabString:
                {
                    types::String* pS = new types::String(iRows, iCols);
                    char pCt[4096];
                    for (int i = 0; i < iCols * iRows; ++i)
                    {
                        int siz = 0;
                        C2F(readstringfile)(&iID, pstFormat, pCt, &siz, &error, strlen(pstFormat));
                        pCt[siz] = '\0';
                        pS->set(i, pCt);
                    }

                    if (error == 0)
                    {
                        out.push_back(pS);
                    }
                    else
                    {
                        delete pS;
                    }
                }
                break;
                default:
                {
                    if (pstFormat)
                    {
                        FREE(pstFormat);
                    }

                    Scierror(999, _("%s: Wrong type for input argument #%d : string expected.\n"), "read", 2);
                    closeFile(in[0], iID);
                    return types::Function::Error;
                }
            }

            //close file
            closeFile(in[0], iID);

            if (error == 2)
            {
                if (pstFormat)
                {
                    FREE(pstFormat);
                }

                Scierror(999, _("Incorrect file or format.\n"));
                return types::Function::Error;
            }
        }
        else//read from the console
        {
            switch (itTypeOfData)
            {
                case types::InternalType::ScilabDouble:
                {
                    types::Double* pD = new types::Double(iRows, iCols, false);

                    char pstString[4] = "(a)";
                    char pCt[4096];

                    for (int i = 0; i < iRows && error == 0; ++i)
                    {
                        int siz = 0;
                        C2F(readstring)(pstString, pCt, &siz, &error, strlen(pstString));
                        pCt[siz] = '\0';

                        char* pch;
                        int iColsTempo = 0;
                        pch = strtok(pCt, " ");
                        while (pch != NULL && error == 0)
                        {
                            if (is_of_type<double>(pch))
                            {
                                pD->set(i, iColsTempo, atof(pch));
                                iColsTempo++;
                            }
                            else
                            {
                                error = 2;
                            }
                            pch = strtok(NULL, " ");
                        }

                        if (iColsTempo != iCols)
                        {
                            delete pD;
                            FREE(pstFormat);
                            Scierror(999, _("%s: Input missing from console.\n"), "read");
                            return types::Function::Error;
                        }
                    }

                    if (error == 0)
                    {
                        out.push_back(pD);
                    }
                    else
                    {
                        delete pD;
                    }
                }
                break;
                case types::InternalType::ScilabInt32:
                {
                    if (pstFormat)
                    {
                        FREE(pstFormat);
                    }

                    Scierror(999, _("Incorrect file or format.\n"));
                    return types::Function::Error;
                }
                case types::InternalType::ScilabString:
                {
                    if (iCols != 1)
                    {
                        if (pstFormat)
                        {
                            FREE(pstFormat);
                        }

                        Scierror(999, _("%s: Wrong input argument %d.\n"), "read", 3);
                        return types::Function::Error;
                    }
                    else
                    {
                        types::String* pS = new types::String(iRows, iCols);
                        char pCt[4096];

                        for (int i = 0; i < (iRows); i++)
                        {
                            int siz = 0;
                            C2F(readstring)(pstFormat, pCt, &siz, &error, strlen(pstFormat));
                            pCt[siz] = '\0';
                            pS->set(i, pCt);
                        }

                        if (error == 0)
                        {
                            out.push_back(pS);
                        }
                        else
                        {
                            delete pS;
                        }
                    }
                }
                break;
                default:
                    break;
            }
        }

        if (error != 0)
        {
            if (pstFormat)
            {
                FREE(pstFormat);
            }
            closeFile(in[0], iID);
            Scierror(999, _("Incorrect file or format.\n"));
            return types::Function::Error;
        }
    }

    if (pstFormat)
    {
        FREE(pstFormat);
    }

    return types::Function::OK;
}
