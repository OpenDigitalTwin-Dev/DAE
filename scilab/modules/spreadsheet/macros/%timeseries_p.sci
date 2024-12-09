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

function %timeseries_p(ts)
    if ts.vars == [] then
        return;
    end

    res = [];
    current_len = 0;
    l = lines();
    l(2) = 50;
    col_s = 1

    nb_rows = size(ts, 1);
    if nb_rows > l(2) then
        output = string(ts([1:3, nb_rows-2:nb_rows], :));
    else
        output = string(ts);
    end

    for c = 1:size(ts.vars, "*")
        name = ts.props.variableNames(c);
        len = length(name);
        max_len = max(length(output(: , c)));
        max_len = max(max_len, len, 3);

        current_len = current_len + max_len + 3;
        if current_len >= l(1) then
            printf("         column %d to %d\n", col_s, c - 1);
            if mode() > 1
                printf("\n");
            end
            res = strcat(res, "", "c");
            mprintf("%s\n", res);
            if mode() > 1
                printf("\n");
            end

            res = [];
            col_s = c;
            current_len = max_len + 3;
        end
    
        shift = floor((max_len - len) / 2);
        right = sprintf("%%%ds", len + shift);
        left = sprintf("%%-%ds", max_len);
        header = sprintf(left, sprintf(right, name));
        separator = strcat(["_"](ones(1, max_len)));

        if c == 1 then
            f = sprintf("   %%%ds\\n", max_len);
        else
            f = sprintf("   %%-%ds\\n", max_len);
        end

        if l(2) <> 0 && nb_rows > l(2) then
            left = floor((max_len-3)/2);
            subres = sprintf(f, [header ; separator ; "" ; output(1:3, c)]);
            subres($+1) = "   " + sprintf("%*s", -max_len, sprintf("%*s", left+3, "..."));
            subres = [subres;sprintf(f, output($-2:$, c))];
            res = [res subres];
        else
            res = [res sprintf(f, [header ; separator ; "" ; output(:, c)])];
        end
    end

    if col_s <> 1 then
        printf("         column %d to %d\n", col_s, c);
        if mode() > 1
            printf("\n");
        end
    end

    res = strcat(res, "", "c");
    mprintf("%s\n", res);
    
endfunction
