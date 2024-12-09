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

function out = %calendarDuration_a_calendarDuration(a, b)
    if size(a, "*") <> 1 && size(b, "*") <> 1 && or(size(a) <> size(b)) then
        error(msprintf(_("%s: Wrong size for input arguments #%d and #%d: scalar or matrix of same size expected.\n"), "%calendarDuration_a_calendarDuration", 1, 2))
    end

    f = [];
    if a.format <> [] then
        f = a.format;
    elseif b.format <> [] then
        f = b.format;
    end

    out_m = (a.y*12) + a.m + ((b.y*12) + b.m);
    out_y = floor(out_m / 12);
    out_m = modulo(out_m, 12);
    out_d = a.d + b.d;

    out_t = a.t + b.t;

    out = mlist(["calendarDuration", "y", "m" "d", "t", "format"], out_y, out_m, out_d, out_t, f);
endfunction
