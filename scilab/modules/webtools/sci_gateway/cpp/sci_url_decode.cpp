/*
 * Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2023 - 3DS - Antoine ELIAS
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */

#include <regex>
#include <curl/curl.h>
#include "double.hxx"
#include "function.hxx"
#include "string.hxx"
#include "webtools_gw.hxx"

extern "C"
{
#include "Scierror.h"
#include "getFullFilename.h"
#include "localization.h"
#include "sci_malloc.h"
#include "sciprint.h"
}
/*--------------------------------------------------------------------------*/
static const char fname[] = "url_decode";
types::Function::ReturnValue sci_url_decode(types::typed_list& in, int _iRetCount, types::typed_list& out)
{
    if (in.size() != 1)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d expected.\n"), fname, 1);
        return types::Function::Error;
    }

    if (_iRetCount > 1)
    {
        Scierror(78, _("%s: Wrong number of output argument(s): %d expected.\n"), fname, 1);
        return types::Function::Error;
    }

    // []
    if (in[0]->isDouble() && in[0]->getAs<types::Double>()->isEmpty())
    {
        out.push_back(in[0]);
        return types::Function::OK;
    }

    // get URL
    if (in[0]->isString() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: Matrix of strings expected.\n"), fname, 1);
        return types::Function::Error;
    }

    types::String* pIn = in[0]->getAs<types::String>();
    wchar_t** w = pIn->get();

    types::String* pOut = new types::String(pIn->getDims(), pIn->getDimsArray());

    for (int i = 0; i < pOut->getSize(); ++i)
    {
        if (wcslen(w[i]) == 0)
        {
            pOut->set(i, "");
            continue;
        }

        char* c = wide_string_to_UTF8(w[i]);
        std::string s(c);

        //check validity of encoded string
        std::regex r("%(.{2})");
        std::regex_token_iterator<std::string::iterator> rend;

        std::regex_token_iterator<std::string::iterator> a(s.begin(), s.end(), r);
        while (a != rend)
        {
            std::string res(*a);
            std::regex e("%[a-fA-F0-9]{2}");
            std::smatch match;
            std::regex_match(res, match, e);
            if (match.size() == 0)
            {
                Scierror(999, _("%s: Argument #%d(%d): Wrong character encoding.\n"), fname, 1, i + 1);
                return types::Function::Error;
            }
            a++;
        }

        char* o = curl_easy_unescape(NULL, c, static_cast<int>(strlen(c)), NULL);
        FREE(c);
        if (o == NULL)
        {
            Scierror(999, _("%s: Unable to decode URI.\n"), fname);
            return types::Function::Error;
        }

        pOut->set(i, o);
        curl_free(o);
    }

    out.push_back(pOut);
    return types::Function::OK;
}
