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

function out = %calendarDuration_a_duration(cd1, dura)
    if size(cd1, "*") <> 1 && size(dura, "*") <> 1 && or(size(cd1) <> size(dura)) then
        error(msprintf(_("%s: Wrong size for input arguments #%d and #%d: scalar or matrix of same size expected.\n"), "%calendarDuration_a_duration", 1, 2))
    end

    ref_size = [1 1];
    if size(cd1.t, "*") <> [0 1] then
        ref_size = size(cd1.t);
    else
        ref_size = size(dura);
    end

    out_y = cd1.y .* ones(ref_size(1), ref_size(2));
    out_m = cd1.m .* ones(ref_size(1), ref_size(2));
    out_d = cd1.d .* ones(ref_size(1), ref_size(2));

    out_t = cd1.t + dura;

    out = mlist(["calendarDuration", "y", "m" "d", "t", "format"], out_y, out_m, out_d, out_t, cd1.format);
endfunction
