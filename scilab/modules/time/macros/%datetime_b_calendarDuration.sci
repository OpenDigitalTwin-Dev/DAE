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

function out = %datetime_b_calendarDuration(dt1, dura, dt2)
    if size(dt1, "*") <> 1 || size(dura, "*") <> 1 || size(dt2, "*") <> 1 then
        error(msprintf(_("%s: Wrong size for input arguments: scalars expected.\n"), "%datetime_b_calendarDuration"));
    end
    
    out = datetime([], "OutputFormat", dt1.format);
    if dt1 <= dt2 && dura > caldays(0) then
        if dura.y == 0 && dura.m == 0 && dura.t == 0 then
            // dura == caldays
            s = dt2.date - dt1.date;
            vec = 0:dura.d:s;
            out = dt1 + caldays(vec);
        elseif dura.y == 0 && dura.m == 0 && dura.d == 0 && dura.t <> 0 then
            out = dt1:dura.t:dt2;
        else
            out = dt1;
            x = dt1 + dura;
            while x <= dt2
                out(1, $+1) = x;
                x = x + dura;
            end
        end
    end
endfunction
