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

function out = %datetime_i_timeseries(i, val, ts)
    //disp("_i_4", i, val, ts);
    out = ts;
    select i
    case "StartTime"
    case "TimeStep"
    case "Properties"
    case "SampleRate"
    else
        idx = find(ts.props.variableNames == i);
        if and(size(val) > [1 1]) then
            error(msprintf(_("%s: Wrong size for input argument #%d: Must be a vector.\n"), "%datetime_i_timeseries", 2));
        end
        val = val(:);
        if idx == [] then
            varnames = [out.props.variableNames(1) i];
            out = [out timeseries(out.vars(1).data, val, "VariableNames", varnames)]
        else
            out.vars(idx).data = val;
        end
    end
endfunction
