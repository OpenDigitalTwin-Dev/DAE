
/*
 * Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Allan CORNET
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

/*--------------------------------------------------------------------------*/
#include "charEncoding.h"
#include "os_string.h"
#include "pcre_error.h"
#include "pcre_private.h"
#include "sci_malloc.h"
#include "strsubst.h"
#include "sciprint.h"
#include "core_math.h"
/*--------------------------------------------------------------------------*/
char **strsubst(const char **strings_input, int strings_dim, const char *string_to_search, const char *replacement_string)
{
    char **replacedStrings = NULL;

    if ( (strings_input) && (string_to_search) && (replacement_string) )
    {
        int i = 0;
        replacedStrings = (char**)MALLOC(sizeof(char*) * strings_dim);
        for (i = 0; i < strings_dim; i++)
        {
            const char *str = strings_input[i];
            replacedStrings[i] = strsub (str, string_to_search, replacement_string);
        }
    }
    return replacedStrings;
}
/*--------------------------------------------------------------------------*/
char **strsubst_reg(const char **strings_input, int strings_dim, const char *string_to_search, const char *replacement_string, int *ierr)
{
    char **replacedStrings = NULL;

    if ( (strings_input) && (string_to_search) && (replacement_string) )
    {
        int i = 0;
        replacedStrings = (char**)MALLOC(sizeof(char*) * strings_dim);
        for (i = 0; i < strings_dim; i++)
        {
            const char *str = strings_input[i];
            replacedStrings[i] = strsub_reg(str, string_to_search, replacement_string, ierr);
        }
    }
    return replacedStrings;
}
/*-------------------------------------------------------------------------------------*/
char *strsub(const char* input_string, const char* string_to_search, const char* replacement_string)
{
    const char *occurrence_str = NULL;
    char* result_str = NULL;
    char *replacedString = NULL;
    int count = 0, len = 0;

    if (input_string == NULL)
    {
        return NULL;
    }

    if (string_to_search == NULL || replacement_string == NULL)
    {
        return os_strdup(input_string);
    }

    occurrence_str = strstr (input_string, string_to_search);
    if (occurrence_str == NULL)
    {
        return os_strdup(input_string);
    }

    if (strlen (replacement_string) > strlen (string_to_search))
    {
        count = 0;
        len = (int)strlen (string_to_search);
        if (len)
        {
            occurrence_str = input_string;
            while (occurrence_str != NULL && *occurrence_str != '\0')
            {
                occurrence_str = strstr (occurrence_str, string_to_search);
                if (occurrence_str != NULL)
                {
                    occurrence_str += len;
                    count++;
                }
            }
        }
        len = count * ((int)strlen(replacement_string) - (int)strlen(string_to_search)) + (int)strlen(input_string);
    }
    else
    {
        len = (int)strlen(input_string);
    }

    replacedString = (char*)MALLOC (sizeof(char) * (len + 1));
    if (replacedString == NULL)
    {
        return NULL;
    }

    occurrence_str = input_string;
    result_str = replacedString;
    len = (int)strlen (string_to_search);
    while (*occurrence_str != '\0')
    {
        if (*occurrence_str == string_to_search[0] && strncmp (occurrence_str, string_to_search, len) == 0)
        {
            const char *N = NULL;
            N = replacement_string;
            while (*N != '\0')
            {
                *result_str++ = *N++;
            }
            occurrence_str += len;
        }
        else
        {
            *result_str++ = *occurrence_str++;
        }
    }
    *result_str = '\0';

    return replacedString;
}/*-------------------------------------------------------------------------------------*/
char *strsub_reg(const char* input_string, const char* string_to_search, const char* replacement_string, int *ierr)
{
    pcre_error_code w = PCRE_FINISHED_OK;

    int Output_Start = 0;
    int Output_End = 0;

    char *replacedString = NULL;
    wchar_t *wcreplacedString = NULL;

    wchar_t *wcreplacement_string = NULL;
    wchar_t *wcinput_string = NULL;

    int len = 0;

    *ierr = (int)PCRE_FINISHED_OK;

    if (input_string == NULL)
    {
        return NULL;
    }

    if (string_to_search == NULL || replacement_string == NULL)
    {
        return os_strdup(input_string);
    }

    w = pcre_private((char*)input_string, (char*)string_to_search, &Output_Start, &Output_End, NULL, NULL);
    if (w != PCRE_FINISHED_OK)
    {
        *ierr = (int)w;
        return os_strdup(input_string);
    }

    wcreplacement_string = to_wide_string((char*)replacement_string);
    wcinput_string = to_wide_string((char*)input_string);

    if (wcreplacement_string == NULL)
    {
        FREE(wcinput_string);
        *ierr = (int)NOT_ENOUGH_MEMORY_FOR_VECTOR;
        return os_strdup(input_string);
    }

    len = (int)wcslen(wcreplacement_string) + (int)wcslen(wcinput_string);

    wcreplacedString = (wchar_t*)MALLOC (sizeof(wchar_t) * (len + 1));
    if (wcreplacedString == NULL)
    {
        FREE(wcreplacement_string);
        FREE(wcinput_string);
        return NULL;
    }

    {
        /* converts to wide characters */

        wchar_t *wctail = NULL;

        int wcOutput_Start = 0;
        int wcOutput_End = 0;

        char *	strOutput_Start = os_strdup(input_string);
        char *  strOutput_End =  os_strdup(input_string);

        wchar_t *wcstrOutput_Start = NULL;
        wchar_t *wcstrOutput_End = NULL;

        /* calculates positions with wide characters */
        strOutput_Start[Output_Start] = '\0';
        strOutput_End[Output_End] = '\0';

        wcstrOutput_Start = to_wide_string(strOutput_Start);
        wcstrOutput_End = to_wide_string(strOutput_End);

        FREE(strOutput_Start);
        FREE(strOutput_End);

        if (wcstrOutput_Start)
        {
            wcOutput_Start = (int)wcslen(wcstrOutput_Start);
            FREE(wcstrOutput_Start);
        }
        else
        {
            wcOutput_Start = 0;
        }

        if (wcstrOutput_End)
        {
            wcOutput_End = (int)wcslen(wcstrOutput_End);
            FREE(wcstrOutput_End);
        }
        else
        {
            wcOutput_End = 0;
        }

        wcsncpy(wcreplacedString, wcinput_string, wcOutput_Start);
        wcreplacedString[wcOutput_Start] = L'\0';
        wcscat(wcreplacedString, wcreplacement_string);
        wctail = wcinput_string + wcOutput_End;
        wcscat(wcreplacedString, wctail);
        replacedString = wide_string_to_UTF8(wcreplacedString);

        FREE(wcreplacedString);
    }

    FREE(wcinput_string);
    FREE(wcreplacement_string);

    return replacedString;
}
/*-------------------------------------------------------------------------------------*/
wchar_t *wcssub_reg(const wchar_t* _pwstInput, const wchar_t* _pwstSearch, const wchar_t* _pwstReplace, int* _piErr)
{
    pcre_error_code iPcreStatus = PCRE_FINISHED_OK;
    int iStart = 0;
    int iEnd = 0;
    int len = 0;
    int* arriStart = NULL;
    int* arriEnd = NULL;
    int iOccurs = 0;
    int iJump = 0;

    wchar_t* result = NULL;

    if (_pwstInput == NULL)
    {
        return NULL;
    }

    // early return, NULL might be given as unspecified
    if (_pwstSearch == NULL || _pwstReplace == NULL)
    {
        return os_wcsdup(_pwstInput);
    }

    // early return, empty strings will not be modified
    if (_pwstSearch[0] == L'\0' || _pwstInput[0] == L'\0')
    {
        return os_wcsdup(_pwstInput);
    }

    len = (int)wcslen(_pwstInput);
    arriStart = (int*)MALLOC(sizeof(int) * len);
    arriEnd = (int*)MALLOC(sizeof(int) * len);

    //check replacement
    int* replacement = (int*)MALLOC(sizeof(int) * wcslen(_pwstReplace));
    int replacement_len = 0;
    const wchar_t* p = _pwstReplace;
    while ((p = wcsstr(p, L"$")) != NULL)
    {
        if (*(p + 1) == L'$')
        {
            replacement[replacement_len++] = -1;
            p += 2;
        }
        else
        {
            wchar_t* end;
            wchar_t digit[3] = {(p + 1)[0], (p + 1)[1]};
            double d = wcstod(digit, &end);
            if (ISNAN(d) || end == digit)
            {
                FREE(arriStart);
                FREE(arriEnd);
                FREE(replacement);
                *_piErr = TOO_BIG_FOR_OFFSET_SIZE;
                return NULL;
            }

            end = p + (end - digit) + 1;
            replacement[replacement_len++] = (int)d;
            p = end;
        }
    }

    wchar_t*** captured = (wchar_t***)MALLOC(sizeof(wchar_t**) * len);
    int* captured_count = (int*)CALLOC(len, sizeof(int));
    do
    {
        iPcreStatus = wide_pcre_private(_pwstInput + iJump, _pwstSearch, &iStart, &iEnd, &captured[iOccurs], &captured_count[iOccurs]);
        if (iPcreStatus == PCRE_FINISHED_OK)
        {
            if (iEnd != iStart)
            {
                arriStart[iOccurs] = iStart + iJump;
                arriEnd[iOccurs++] = iEnd + iJump;
                iJump += iEnd;
            }
            else if (iEnd == 0 && _pwstInput[iJump] != L'\0')
            {
                //avoid infinite loop
                iJump++;
            }
        }
        else if (iPcreStatus != NO_MATCH)
        {
            pcre_error("strsubst", iPcreStatus);
            FREE(arriStart);
            FREE(arriEnd);
            FREE(replacement);
            FREE(captured);
            FREE(captured_count);
            *_piErr = iPcreStatus;
            return NULL;
        }
    }
    while (iOccurs < len && iPcreStatus == PCRE_FINISHED_OK && iStart != iEnd);

    if (iOccurs)
    {
        int i = 0;
        int replaceLen = (int)wcslen(_pwstReplace);
        int finalSize = len;

        //compute final size
        for (i = 0; i < iOccurs; ++i)
        {
            finalSize -= (arriEnd[i] - arriStart[i]);
            finalSize += replaceLen;
        }

        result = (wchar_t*)MALLOC(sizeof(wchar_t) * (finalSize + 1));
        result[0] = '\0';

        //from start to first occurence
        wcsncat(result, _pwstInput, arriStart[0]);
        result[arriStart[0]] = '\0';

        for (i = 0; i < iOccurs - 1; ++i)
        {
            int curLen = (int)wcslen(result);
            int partLen = arriStart[i + 1] - arriEnd[i];
            //insert replace string
            wcscat(result, _pwstReplace);
            //copy part between 2 occurences
            wcsncat(result, _pwstInput + arriEnd[i], partLen);
            result[curLen + replaceLen + partLen] = '\0';
        }

        wcscat(result, _pwstReplace);
        //copy part after last occurence
        wcscat(result, _pwstInput + arriEnd[iOccurs - 1]);

        /*group substitution*/
        wchar_t* p = result;
        int iDollar = 0;
        while ((p = wcsstr(p, L"$")) != NULL && iDollar < replacement_len)
        {
            BOOL sfree = FALSE;
            wchar_t* s = NULL;
            int replace_len = 0;
            if (replacement[iDollar] > captured_count[0])
            {
                FREE(arriStart);
                FREE(arriEnd);
                FREE(replacement);
                FREE(captured);
                FREE(captured_count);
                *_piErr = TOO_BIG_FOR_OFFSET_SIZE;
                return NULL;
            }

            switch (replacement[iDollar])
            {
                case -1: //replace $$ by $
                    s = L"$";
                    replace_len = 2;
                    break;
                case 0: //replace by whole matching string
                {
                    int len = arriEnd[0] - arriStart[0];
                    s = (wchar_t*)MALLOC(sizeof(wchar_t) * (len + 1));
                    wcsncpy(s, _pwstInput + arriStart[0], len);
                    s[len] = L'\0';
                    sfree = TRUE;
                    replace_len = 2;
                    break;
                }
                default: //replace by capturing group string
                    s = captured[0][replacement[iDollar] - 1];
                    replace_len = replacement[iDollar] <= 9 ? 2 : 3; //$x or $xx
                    break;
            }

            if (s != NULL)
            {
                int slen = (int)wcslen(s);
                finalSize = (int)wcslen(result) - replace_len + slen + 1;
                wchar_t* res = (wchar_t*)MALLOC(sizeof(wchar_t) * finalSize);
                wcsncpy(res, result, p - result);
                int offset = (int)(p - result);
                wcsncpy(res + offset, s, slen);
                offset += slen;
                wcscpy(res + offset, p + replace_len);
                p = res + offset;
                FREE(result);
                result = res;
                if (sfree)
                {
                    FREE(s);
                }
            }

            iDollar++;
        }
    }
    else
    {
        *_piErr = iPcreStatus;
        result = os_wcsdup(_pwstInput);
    }

    FREE(arriStart);
    FREE(arriEnd);
    FREE(replacement);
    FREE(captured);
    FREE(captured_count);

    return result;
}
/*-------------------------------------------------------------------------------------*/
wchar_t **wcssubst_reg(const wchar_t** _pwstInput, int _iInputSize, const wchar_t* _pwstSearch, const wchar_t* _pwstReplace, int* _piErr)
{
    wchar_t** pwstOutput = NULL;

    if (_pwstInput != NULL && _pwstSearch != NULL && _pwstReplace != NULL)
    {
        int i = 0;
        pwstOutput = (wchar_t**)MALLOC(sizeof(wchar_t*) * _iInputSize);
        for (i = 0 ; i < _iInputSize ; i++)
        {
            const wchar_t* pwst = _pwstInput[i];
            pwstOutput[i] = wcssub_reg(pwst, _pwstSearch, _pwstReplace, _piErr);
        }
    }
    return pwstOutput;
}
/*-------------------------------------------------------------------------------------*/
wchar_t **wcssubst(const wchar_t** _pwstInput, int _iInputSize, const wchar_t* _pwstSearch, const wchar_t* _pwstReplace)
{
    wchar_t** pwstOutput = NULL;

    if (_pwstInput != NULL && _pwstSearch != NULL && _pwstReplace != NULL)
    {
        int i = 0;
        pwstOutput = (wchar_t**)MALLOC(sizeof(wchar_t*) * _iInputSize);
        for (i = 0 ; i < _iInputSize ; i++)
        {
            const wchar_t* pwst = _pwstInput[i];
            if (wcslen(pwst) == 0)
            {
                if (wcslen(_pwstSearch) == 0)
                {
                    pwstOutput[i] = os_wcsdup(_pwstReplace);
                }
                else
                {
                    pwstOutput[i] = os_wcsdup(L"");
                }
            }
            else
            {
                pwstOutput[i] = wcssub(pwst, _pwstSearch, _pwstReplace);
            }
        }
    }
    return pwstOutput;
}
/*-------------------------------------------------------------------------------------*/
wchar_t *wcssub(const wchar_t* _pwstInput, const wchar_t* _pwstSearch, const wchar_t* _pwstReplace)
{
    int i               = 0;
    int iOccurs         = 0;
    size_t iReplace     = 0;
    size_t iSearch      = 0;
    size_t iOffset      = 0;

    size_t* piStart     = NULL;

    const wchar_t* pwstPos  = NULL;
    wchar_t* pwstOutput     = NULL;

    if (_pwstInput == NULL)
    {
        return NULL;
    }

    if (_pwstSearch == NULL || _pwstReplace == NULL)
    {
        return os_wcsdup(_pwstInput);
    }

    //no needle
    if (_pwstSearch[0] == L'\0')
    {
        //no input
        if (_pwstInput[0] == L'\0')
        {
            return os_wcsdup(_pwstReplace);
        }
        else
        {
            return os_wcsdup(_pwstInput);
        }
    }

    //no input
    if (_pwstInput[0] == L'\0')
    {
        return os_wcsdup(_pwstInput);
    }

    iSearch     = wcslen(_pwstSearch);
    iReplace    = wcslen(_pwstReplace);
    piStart     = (size_t*)MALLOC(sizeof(size_t) * wcslen(_pwstInput));
    pwstPos     = _pwstInput;

    while (pwstPos)
    {
        pwstPos = wcsstr(pwstPos, _pwstSearch);
        if (pwstPos)
        {
            piStart[iOccurs++]  = pwstPos - _pwstInput;
            iOffset             += iReplace - iSearch;
            pwstPos             += iSearch;
        }
    }

    pwstOutput = (wchar_t*)MALLOC(sizeof(wchar_t) * (wcslen(_pwstInput) + iOffset + 1));
    memset(pwstOutput, 0x00, sizeof(wchar_t) * (wcslen(_pwstInput) + iOffset + 1));

    if (iOccurs == 0)
    {
        wcscpy(pwstOutput, _pwstInput);
    }
    else
    {
        for (i = 0 ; i < iOccurs ; i++)
        {
            if (i == 0)
            {
                //copy start of original string
                wcsncpy(pwstOutput, _pwstInput, piStart[i]);
            }
            else
            {
                //copy start of original string
                wcsncpy(pwstOutput + wcslen(pwstOutput), _pwstInput + piStart[i - 1] + iSearch, piStart[i] - (iSearch + piStart[i - 1]));
            }
            //copy replace string
            wcscpy(pwstOutput + wcslen(pwstOutput), _pwstReplace);
        }
        //copy end of original string
        wcscpy(pwstOutput + wcslen(pwstOutput), _pwstInput + piStart[iOccurs - 1] + iSearch);
    }

    FREE(piStart);
    return pwstOutput;
}
/*-------------------------------------------------------------------------------------*/
