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

function out = varfun(varargin)
    fname = "varfun";
    groupingVariables = "";
    inputVariables = "";

    rhs = nargin
    if rhs > 2 then
        for i = nargin-1:-2:3
            if type(varargin(i)) <> 10 || (type(varargin(i)) == 10 && ~isscalar(varargin(i))) then
                break;
            end
            select varargin(i)
            case "GroupingVariables" 
                groupingVariables = varargin(i + 1);
                if and(type(groupingVariables) <> [1 10]) then
                    error(msprintf(_("%s: Wrong type for input argument #%d: string or double vector expected.\n"), fname, i));
                end
            case "InputVariables"
                inputVariables = varargin(i + 1);
                if and(type(inputVariables) <> [1 10]) then
                    error(msprintf(_("%s: Wrong type for input argument #%d: string or double vector expected.\n"), fname, i));
                end
            else
                error(msprintf(_("%s: Wrong value for input argument #%d: ''%s'' not allowed.\n"), fname, i, varargin(i)));
            end
        end
    end

    method = varargin(1);
    if and(typeof(method) <> ["function", "fptr"]) then
        error(msprintf(_("%s: Wrong type for input argument #%d: function expected.\n"), fname, 1));
    end

    t = varargin(2);
    if ~istable(t) & ~istimeseries(t) then
        error(msprintf(_("%s: Wrong type for input argument #%d: table or timeseries expected.\n"), fname, 2));
    end

    [t, varnames, groupingVariables] = %_checkinputVariable(t, inputVariables, groupingVariables)


    if groupingVariables <> "" then
        method = list(method);
        opts = struct("includeEmpty", %f, ...
                "method", method, ...
                "nameMethod", "fun");
        out = %_rowvarfun(fname, t, groupingVariables, opts)
    else
        // without groupvars
        // varfun(func, A) or varfun(func, A, "InputVariables", varnames)
        
        out = t;
        for i = 1:size(t.vars, "*")
            data = out.vars(i).data;
            if isdatetime(data) then
                newdt = data;
                newdt.date = method(data.date);
                newdt.time = (newdt.date - int(newdt.date))*(24*60*60) + data.time;
                out.vars(i).data = newdt;
            elseif isduration(data) then
                newdt = data;
                newdt.duration = method(data.duration);
                out.vars(i).data = newdt;
            else
                out.vars(i).data = method(data);
            end
        end
        if istimeseries(t) then
            out.props.variableNames(2:$) = "fun_" + varnames(2:$);
            out.vars(1).data = t.vars(1).data(1:length(out.vars(1).data));
        else
            out.props.variableNames = "fun_" + varnames;
        end
    end

endfunction
