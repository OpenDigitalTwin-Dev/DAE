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

function %calendarDuration_p(dura)
    if dura.y == [] then
        return;
    end

    res = [];
    current_len = 0;
    l = lines();
    l(2) = 50;
    col_s = 1

    nb_rows = size(dura.y, 1);
    if nb_rows > l(2) then
        output = string(dura([1:3, nb_rows-2:nb_rows], :));
    else
        output = string(dura);
    end

    for c = 1:size(dura.y, 2)
        max_len = max(length(output(: , c)));
        current_len = current_len + max_len + 3;
        if current_len >= l(1) then
            printf("         column %d to %d\n\n", col_s, c - 1);

            res = strcat(res, "", "c");
            res = strcat(res, "\n");

            printf(res);
            printf("\n\n");

            if mode() > 1
                printf("\n");
            end

            res = [];
            col_s = c;
            current_len = max_len + 3;
        end

        f = sprintf("   %%%ds\\n", max_len);
        if l(2) <> 0 && nb_rows > l(2) then
            left = floor((max_len-3)/2);
            subres = sprintf(f, output(1:3, c));
            subres($+1) = "   " + sprintf("%*s", -max_len, sprintf("%*s", left+3, "..."));
            subres = [subres;sprintf(f, output($-2:$, c))];
            res = [res subres];
        else
            res = [res sprintf(f, output(:, c))];
        end
    end

    if col_s <> 1 then
        printf("         column %d to %d\n\n", col_s, c);
    end

    res = strcat(res, "", "c");
    res = strcat(res, "\n");

    printf(res);
    printf("\n");
endfunction
