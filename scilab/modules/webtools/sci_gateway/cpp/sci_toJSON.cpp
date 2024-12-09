/*
* Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2017 - ESI-Group - Antoine ELIAS
*
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

#include <sstream>
#include <string>
#include <fstream>
#include <iomanip>
#include <vector>
#include <regex>
#include <chrono>

#define __API_SCILAB_UNSAFE__
#include "json.hxx"

//#define SHOW_TIMER

extern "C"
{
#include "gw_webtools.h"
#include "api_scilab.h"
#include "Scierror.h"
#include "sciprint.h"
#include "sci_malloc.h"
#include <localization.h>
}

bool exportJSON(scilabEnv env, scilabVar var, int indent, const std::wstring &file, scilabVar* const  out);
bool export_data(scilabEnv env, scilabVar var, int indent, std::wostringstream &os);

const std::string name("toJSON");

int sci_toJSON(scilabEnv env, int nin, scilabVar *in, int nopt, scilabOpt opt, int nout, scilabVar *out)
{
#ifdef SHOW_TIMER
    std::chrono::steady_clock::time_point ttotal = std::chrono::steady_clock::now();
    std::chrono::steady_clock::time_point tinput = std::chrono::steady_clock::now();
#endif

    int indent = 0;
    std::wstring file;

    if (1 > nin || nin > 3)
    {
        Scierror(999, _("%s: Wrong number of input arguments: %d to %d expected.\n"), name.data(), 1, 3);
        return STATUS_ERROR;
    }

    switch (nin)
    {
        case 1:
            //OK
            break;

        case 2:
        {
            //must be a scalar double or single string
            if ((!scilab_isDouble(env, in[1]) && !scilab_isString(env, in[1])) || !scilab_isScalar(env, in[1]))
            {
                Scierror(999, "%s: Wrong type for input argument #%d: double or string expected.\n", name.data(), 2);
                return STATUS_ERROR;
            }

            if (scilab_isDouble(env, in[1]))
            {
                double dbl = 0;
                scilab_getDouble(env, in[1], &dbl);
                indent = (int)dbl;
            }
            else
            {
                //string
                wchar_t *wf = nullptr;
                scilab_getString(env, in[1], &wf);
                file = wf;
            }
            break;
        }
        case 3:
        {
            // toJSON(var, file, indent)
            int indent_input = 2;
            int file_input = 1;
            if (scilab_isDouble(env, in[1])) // toJSON(var, indent, file)
            {
                indent_input = 1;
                file_input = 2;
            }

            //must be a scalar double
            if (!scilab_isDouble(env, in[indent_input]) || !scilab_isScalar(env, in[indent_input]))
            {
                Scierror(999, "%s: Wrong type for input argument #%d: A real scalar expected.\n", name.data(), indent_input + 1);
                return STATUS_ERROR;
            }

            double dbl = 0;
            scilab_getDouble(env, in[indent_input], &dbl);
            indent = (int)dbl;

            //must be a single string
            if (!scilab_isString(env, in[file_input]) || !scilab_isScalar(env, in[file_input]))
            {
                Scierror(999, "%s: Wrong type for input argument #%d: single string expected.\n", name.data(), file_input + 1);
                return STATUS_ERROR;
            }

            wchar_t *wf = nullptr;
            scilab_getString(env, in[file_input], &wf);
            file = wf;
            break;
        }
        default:
        {
            Scierror(999, "%s: bad input arguments.\n", name.data());
            return STATUS_ERROR;
        }
    }

#ifdef SHOW_TIMER
    std::chrono::steady_clock::time_point tinputend = std::chrono::steady_clock::now();
    sciprint("Input\t: %d ms\n", std::chrono::duration_cast<std::chrono::milliseconds>(tinputend - tinput).count());
#endif

    exportJSON(env, in[0], indent, file, out);

#ifdef SHOW_TIMER
    std::chrono::steady_clock::time_point ttotalend = std::chrono::steady_clock::now();
    sciprint("Total\t: %d ms\n", std::chrono::duration_cast<std::chrono::milliseconds>(ttotalend - ttotal).count());
#endif

    return 0;
}

bool exportJSON(scilabEnv env, scilabVar var, int indent, const std::wstring& file, scilabVar* const out)
{
#ifdef SHOW_TIMER
    std::chrono::steady_clock::time_point tprocess = std::chrono::steady_clock::now();
#endif

    std::string s = toJSON(env, var, indent);

#ifdef SHOW_TIMER
    std::chrono::steady_clock::time_point tprocessend = std::chrono::steady_clock::now();
    sciprint("Process\t: %d ms\n", std::chrono::duration_cast<std::chrono::milliseconds>(tprocessend - tprocess).count());

    std::chrono::steady_clock::time_point toutput = std::chrono::steady_clock::now();
#endif

    if (file.empty() == false)
    {
        char *_filename = wide_string_to_UTF8(file.c_str());
        if (_filename == NULL)
        {
            return false;
        }

        std::ofstream outfile(_filename);
        outfile << s.data();
        outfile.close();
        FREE(_filename);
    }
    else
    {
        wchar_t* w = to_wide_string(s.data());
        out[0] = scilab_createString(env, w);
        FREE(w);
    }

#ifdef SHOW_TIMER
    std::chrono::steady_clock::time_point toutputend = std::chrono::steady_clock::now();
    sciprint("Output\t: %d ms\n", std::chrono::duration_cast<std::chrono::milliseconds>(toutputend - toutput).count());
#endif

    return true;
}
