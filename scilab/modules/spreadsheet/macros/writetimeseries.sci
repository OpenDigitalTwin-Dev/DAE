// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2022 - 3DS - Adeline CARNIS
// Copyright (C) 2022 - 3DS - Antoine ELIAS
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function writetimeseries(ts, filename, varargin)
    rhs = nargin;
    delim = ",";
    if rhs > 2 then
        for i = nargin-1:-2:3
            if type(varargin(i)) <> 10 then
                break;
            end

            select varargin(i)
            case "Delimiter"
                delim = varargin(i + 1);
                if type(delim) <> 10 then
                    error(msprintf(_("%s: Wrong type for %s argument: string expected.\n"), "writetimeseries", varargin(i)));
                end
                if ~isscalar(delim) then
                    error(msprintf(_("%s: Wrong size for %s argument: scalar expected.\n"), "writetimeseries", varargin(i)));
                end
            else
                error(msprintf(_("%s: Wrong value for input argument #%d: ''%s'' not allowed.\n"), "writetimeseries", i, varargin(i)));
            end

            rhs = rhs - 2;
        end
    end

    // arg #1
    if ~istimeseries(ts) then
        error(msprintf(_("%s: Wrong type for input argument #%d: timeseries expected.\n"), "writetimeseries", 1));
    end

    // arg #2
    if type(filename) <> 10 then
        error(msprintf(_("%s: Wrong type for input argument #%d: file name expected.\n"), "writetimeseries", 1));
    end

    tss = string(ts);
    tss = [ts.props.variableNames; tss];

    csvWrite(tss, filename, delim)

endfunction
