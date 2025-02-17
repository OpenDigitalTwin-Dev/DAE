/*
*  Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2009 - DIGITEO - Antoine ELIAS
*  Copyright (C) 2018 - 2019 - UTC - Stéphane MOTTELET
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

#include <cmath>
#include <algorithm>
#include <iostream>
#include <iomanip>
#include "tostring_common.hxx"
#include "configvariable.hxx"

extern "C"
{
#include "sci_malloc.h"
#include "elem_common.h"
#include "os_string.h"
#include "dtoa.h"
#include "charEncoding.h"
}

#define POINT_SIZE 1
#define EXPOSANT_SIZE 2         //exposant symbol + exposant sign

//template <typename T>
//void GetIntFormat(T _TVal, int *_piWidth)
//{
// *_piWidth = (int)(log10((double)_abs64(_TVal)) + 1);
//}

//template <typename T>
//void AddIntValue(ostringstream *_postr, T _TVal, int _iWidth, bool bPrintPlusSign, bool bPrintOne)
//{
// if(bPrintPlusSign == true)
// {
//  *_postr << (_TVal < 0 ? MINUS_STRING_INT : PLUS_STRING);
// }
// else
// {
//  *_postr << (_TVal < 0 ? MINUS_STRING_INT : NO_SIGN);
// }
//
// configureStream(_postr, _iWidth, 0, ' ');
//
// if(bPrintOne == true || _TVal != 1)
// {
//  *_postr << right << _abs64(_TVal);
// }
//}

void addSign(std::wostringstream * _postr, double _dblVal, bool _bPrintPlusSign, bool _bPaddSign)
{
    if (_bPrintPlusSign == true)
    {
        *_postr << (_dblVal < 0 ? MINUS_STRING : PLUS_STRING);
    }
    else
    {
        if (_bPaddSign)
        {
            *_postr << (_dblVal < 0 ? MINUS_STRING : NO_SIGN);
        }
        else
        {
            *_postr << (_dblVal < 0 ? MINUS_STRING : L"");
        }
    }
}

//void getDoubleFormat(double _dblVal, int *_piWidth, int *_piPrec, /*in/out*/bool* _pExp)
void getDoubleFormat(double _dblVal, DoubleFormat * _pDF)
{
    double dblDec = 0;
    double dblEnt = 0;
    double dblAbs = std::fabs(_dblVal);
    int iNbDigit = 0;
    int iNbDec = 0;
    int iBlankSize = _pDF->bPrintBlank ? BLANK_SIZE : 0;
    _pDF->iSignLen = _pDF->bPrintBlank ? SIGN_LENGTH + BLANK_SIZE : _pDF->iSignLen;

    _pDF->bExp |= ConfigVariable::getFormatMode() == 0;
    int iTotalLen = 0;
    int iPrecNeeded = ConfigVariable::getFormatSize();

    if (ISNAN(_dblVal) || !finite(_dblVal))
    {
        _pDF->iWidth = 4;      //" nan" or " inf"
        _pDF->iPrec = 0;
        return;
    }
    //get integer part and fractionnal part
    dblDec = std::modf(dblAbs, &dblEnt);

    //compute len of entire part
    if (dblEnt == 0)
    {
        //[-1, 1]
        if (dblAbs == 0)
        {
            iNbDigit = -1;
        }
        else
        {
            iNbDigit = (int)std::fabs(std::floor(std::log10(dblAbs)));
        }

        if (iNbDigit >= (iPrecNeeded - 2) || _pDF->bExp)
        {
            //exponant
            _pDF->bExp = true;
            iTotalLen = iBlankSize + 1 /*integer before dot */  + POINT_SIZE + EXPOSANT_SIZE + std::max(((int)log10((double)iNbDigit)) + 1, 2);
            _pDF->iWidth = iPrecNeeded;
            _pDF->iPrec = iPrecNeeded - iTotalLen;
            return;
        }
        else
        {
            iTotalLen = iBlankSize + 1 /*integer before dot */  + POINT_SIZE;
        }
    }
    else
    {
        double dblTemp = log10(dblEnt);
        if (dblTemp > (iPrecNeeded - 2) || _pDF->bExp)
        {
            //exponant mode
            if (dblTemp == 0)
            {
                dblTemp = 1;    //no incidence on value, just to allow log10(dblTemp)
            }

            _pDF->bExp = true;
            iTotalLen = iBlankSize + 1 /*integer before dot */  + POINT_SIZE + EXPOSANT_SIZE + std::max(((int)log10(dblTemp)) + 1, 2);
            _pDF->iWidth = iPrecNeeded;
            _pDF->iPrec = iPrecNeeded - iTotalLen;
            return;
        }
        else
        {
            //number of digit in integer part
            iTotalLen = iBlankSize + ((int)dblTemp + 1) + POINT_SIZE;
        }
    }

    //prepare fractionnal part to precision asked
    double dblScale = pow(10., iPrecNeeded - iTotalLen);
    while (iTotalLen <= iPrecNeeded)
    {
        dblDec = dblDec * dblScale;
        dblDec = floor(dblDec + 0.5);
        dblDec = dblDec / dblScale;

        // when dblAbs = 1.9999999..., modf function
        // return a decimal part equal to 1.0
        if (dblDec == 0. || dblDec == 1.)
        {
            break;
        }

        dblDec = dblDec * 10;
        int iFloor = (int)floor(dblDec);

        dblDec -= iFloor;
        iTotalLen++;
        iNbDec++;
        dblScale /= 10;
    }

    _pDF->iWidth = iTotalLen;
    _pDF->iPrec = iNbDec;
}

//void getComplexFormat(double _dblR, double _dblI, int *_piTotalWidth, int *_piWidthR, int *_piWidthI, int *_piPrecR, int *_piPrecI, bool* _pExpR, bool* _pExpI)
void getComplexFormat(double _dblR, double _dblI, int *_piTotalWidth, DoubleFormat * _pDFR, DoubleFormat * _pDFI)
{
    getDoubleFormat(_dblR, _pDFR);
    getDoubleFormat(_dblI, _pDFI);

    *_piTotalWidth = _pDFR->iWidth + _pDFI->iWidth + 2 * (_pDFR->bPrintBlank ? BLANK_SIZE : 0) + BLANK_SIZE + SIZE_SYMBOL_I;
}

void addDoubleValue(std::wostringstream * _postr, double _dblVal, DoubleFormat * _pDF)
{
    wchar_t pwstOutput[32] = {0};     // > @ format max
    wchar_t pwstSign[32] = {0};
    
    if (_pDF == NULL)
    {
        return;
    }

    const wchar_t* pBlank = L"";
    if (_pDF->bPrintBlank)
    {
        pBlank = L" ";
    }

    const wchar_t* pSign = MINUS_STRING;
    if (_dblVal >= 0 || ISNAN(_dblVal))
    {
        if (_pDF->bPrintPlusSign)
        {
            pSign = PLUS_STRING;
        }
        else if (_pDF->bPaddSign)
        {
            pSign = NO_SIGN;
        }
        else
        {
            // PLUS_SIGN "+" is not written by default
            pSign = L"";
        }
    }

    // Step 1: BLANK and SIGN in pwstSign
    if (_pDF->bPrintComplexPlusSpace)
    {
        os_swprintf(pwstSign, 32, L"%ls%ls%ls", pBlank, pSign, pBlank);
    }
    else
    {
        os_swprintf(pwstSign, 32, L"%ls%ls", pBlank, pSign);
    }


    if (ISNAN(_dblVal))
    {
        //NaN
        os_swprintf(pwstOutput, 32, L"%ls%-*ls", pwstSign, _pDF->iWidth - 1, L"Nan");
    }
    else if (!finite(_dblVal))
    {
        //Inf
        os_swprintf(pwstOutput, 32, L"%ls%-*ls", pwstSign, _pDF->iWidth - 1, L"Inf");
    }
    else if (_pDF->bExp)
    {
        // Prints the exponent part 1.543D+03 for example
        int iDecpt = 0;
        int iSign = 0;
        char *pRve = nullptr;
        char *pDtoaStr = dtoa(fabs(_dblVal), 2, _pDF->iPrec + 1, &iDecpt, &iSign, &pRve);

        std::string str(pDtoaStr);
        freedtoa(pDtoaStr);

        // in format("e") omiting the decimal point has a sense
        // only if fabs(_dblVal) is an integer in {1...9}
        if (_pDF->bPrintPoint || str.length() > 1)
        {
            /* add trailing zeros */
            str.append(std::max(0, _pDF->iPrec + 1 - (int)str.length()), '0');
            str.insert(1, ".");
        }

        wchar_t* pwstData = to_wide_string(str.data());
        os_swprintf(pwstOutput, 32, L"%ls%lsD%+.02d", pwstSign, pwstData, iDecpt - 1);
        FREE(pwstData);
    }
    else if ((_pDF->bPrintOne == true) || (isEqual(fabs(_dblVal), 1)) == false)
    {
        //do not print if _bPrintOne == false && _dblVal == 1
        int iDecpt = 0;
        int iSign = 0;
        int iWidth = _pDF->iWidth;
        char *pRve = nullptr;
        char *pDtoaStr = dtoa(fabs(_dblVal), 2, std::floor(log10(fabs(_dblVal))) + _pDF->iPrec + 1, &iDecpt, &iSign, &pRve);

        std::string str(pDtoaStr);
        freedtoa(pDtoaStr);

        /* add leading 0.000..., if applicable */
        if (iDecpt <= 0)
        {
            str.insert(str.begin(), 1 - iDecpt, '0');
            str.insert(1, ".");
        }
        else if (_pDF->bPrintPoint || iDecpt < (int)str.length())
        {
            str.append(std::max(0, (iDecpt - (int)str.length())), '0');
            str.insert(iDecpt, ".");
        }
        else
        {
            str.append(std::max(0, (iDecpt - (int)str.length())), '0');
            iWidth--;
        }

        // trim or append trailing zeros, if applicable
        if (_pDF->bPrintPoint == false)
        {
            iWidth = 1 + str.length();
        }
        else if (std::atof(str.data()) != fabs(_dblVal) && _pDF->bPrintTrailingZeros == true)
        {
            str.append(std::max(0, (ConfigVariable::getFormatSize() - (int)str.length()) - 1), '0');
        }

        wchar_t* pwstData = to_wide_string(str.data());
        os_swprintf(pwstOutput, 32, L"%ls%-*ls", pwstSign, iWidth - 1, pwstData);
        FREE(pwstData);
    }
    else if (wcslen(pwstSign) != 0)
    {
        os_swprintf(pwstOutput, 32, L"%ls", pwstSign);
    }

    *_postr << pwstOutput;
}

void DoubleComplexMatrix2String(std::wostringstream* _postr, double _dblR, double _dblI)
{
    /*
    if R && !C -> R
    if R && C -> R + Ci
    if !R && !C -> 0
    if(!R aa C	-> Ci
    */
    DoubleFormat dfR, dfI;
    dfR.bPrintBlank = false;
    dfI.bPrintBlank = false;

    getDoubleFormat(_dblR, &dfR);
    getDoubleFormat(_dblI, &dfI);

    dfR.bPrintPoint = dfR.bExp;
    dfR.bPaddSign = false;

    dfI.bPrintPoint = dfI.bExp;
    dfI.bPaddSign = false;

    // decrease precision when the real number will be rounded
    // format(10) with number 1.12345678 should return 1.1234568
    if (dfR.iWidth == ConfigVariable::getFormatSize())
    {
        if (dfR.iPrec != 0)
        {
            dfR.iPrec--;
        }

        dfR.iWidth--;
    }

    if (dfI.iWidth == ConfigVariable::getFormatSize())
    {
        if (dfI.iPrec != 0)
        {
            dfI.iPrec--;
        }

        dfI.iWidth--;
    }

    if (_dblR == 0)
    {
        // no real part
        if (_dblI == 0)
        {
            // no imaginary part

            // 0
            addDoubleValue(_postr, 0, &dfR);
        }
        else
        {
            // imaginary part

            // I
            *_postr << (_dblI < 0 ? L"-" : L"");
            *_postr << L"%i";
            if (fabs(_dblI) != 1 || dfI.bExp)
            {
                // specail case if I == 1 write only %i and not %i*1
                *_postr << L"*";
                addDoubleValue(_postr, fabs(_dblI), &dfI);
            }
        }
    }
    else
    {
        // real part
        if (_dblI == 0)
        {
            // no imaginary part

            // R
            addDoubleValue(_postr, _dblR, &dfR);
        }
        else
        {
            // imaginary part

            // R
            addDoubleValue(_postr, _dblR, &dfR);
            // I
            *_postr << (_dblI < 0 ? L"-%i" : L"+%i");
            if (fabs(_dblI) != 1 || dfI.bExp)
            {
                // special case if I == 1 write only %i and not %i*1
                *_postr << L"*";
                addDoubleValue(_postr, fabs(_dblI), &dfI);
            }
        }
    }
}

/*
void addDoubleValue(std::wostringstream *_postr, double _dblVal, int _iWidth, int _iPrec, bool bPrintPlusSign, bool bPrintOne, bool bPaddSign)
{
    addSign(_postr, _dblVal, bPrintPlusSign, bPaddSign);
    configureStream(_postr, _iWidth, _iPrec, ' ');

    if(bPrintOne == true || isEqual(_dblVal, 1) == false)
    {
        NEWprintDoubleVar(_postr, _dblVal, _iWidth, _iPrec);
    }
}
*/
void addDoubleComplexValue(std::wostringstream * _postr, double _dblR, double _dblI, int _iTotalWidth, DoubleFormat * _pDFR, DoubleFormat * _pDFI)
{
    std::wostringstream ostemp;

    /*
     * if R && !C -> R
     * if R && C -> R + Ci
     * if !R && !C -> 0
     * if(!R && C -> Ci
     */

    // *_postr << "|%" << _iTotalWitdh << "%|";

    //R
    DoubleFormat df;

    df.iPrec = _pDFR->iPrec;
    df.bExp = _pDFR->bExp;

    if (ISNAN(_dblR) || !finite(_dblR))
    {
        ostemp << std::left << std::setw(_pDFR->iWidth + 1);
        addDoubleValue(&ostemp, _dblR, &df);
    }
    else
    {
        // following line allows to align sign of imaginary part
        addDoubleValue(&ostemp, _dblR, &df);
        addSpaces(&ostemp, _pDFR->iWidth - ostemp.tellp() + 1);
    }
    //I
    df.iPrec = _pDFI->iPrec;
    df.bExp = _pDFI->bExp;
    df.bPrintPlusSign = true;
    df.bPrintComplexPlusSpace = true;
    df.bPrintOne = false;

    addDoubleValue(&ostemp, _dblI, &df);
    ostemp << std::left << SYMBOL_I;
    if (_dblI == 1 && _pDFI->bExp == false)
    {
        addSpaces(&ostemp, 2);
    }

    configureStream(_postr, _iTotalWidth, 0, ' ');
    *_postr << std::left << ostemp.str() << std::resetiosflags(std::ios::adjustfield);
}

void addSpaces(std::wostringstream * _postr, int _iSpace)
{
    for (int i = 0; i < _iSpace; i++)
    {
        *_postr << L" ";
    }
}

void configureStream(std::wostringstream * _postr, int _iWidth, int _iPrec, char _cFill)
{
    _postr->setf(std::ios::showpoint);
    _postr->width(_iWidth);
    _postr->precision(_iPrec);
    _postr->fill(_cFill);
}

void addColumnString(std::wostringstream& ostr, int _iFrom, int _iTo)
{
    if (ConfigVariable::isPrintCompact() == false && _iFrom != 1)
    {
        ostr << std::endl;
    }
    if (_iFrom == _iTo)
    {
        ostr << L"         column " << _iFrom << std::endl;
    }
    else
    {
        ostr << L"         column " << _iFrom << L" to " << _iTo << std::endl;
    }
    if (ConfigVariable::isPrintCompact() == false)
    {
        ostr << std::endl;
    }
}

void printEmptyString(std::wostringstream& ostr)
{
    ostr << L"    []";
}

void printDoubleValue(std::wostringstream& ostr, double val)
{
    DoubleFormat df;
    df.bPrintTrailingZeros = true;
    getDoubleFormat(val, &df);
    ostr << SPACE_BETWEEN_TWO_VALUES;
    addDoubleValue(&ostr, val, &df);
}

void printComplexValue(std::wostringstream& ostr, double val_r, double val_i)
{
    DoubleFormat dfR, dfI;
    dfR.bPrintTrailingZeros = true;
    dfI.bPrintTrailingZeros = true;

    int iTotalWidth;
    getComplexFormat(val_r, val_i, &iTotalWidth, &dfR, &dfI);
    ostr << SPACE_BETWEEN_TWO_VALUES;
    addDoubleComplexValue(&ostr, val_r, val_i, iTotalWidth, &dfR, &dfI);
}
