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

function out = %datetime_a_duration(dt, dura)
    if size(dt, "*") <> 1 && size(dura, "*") <> 1 && or(size(dt) <> size(dura)) then
        error(msprintf(_("%s: Wrong size for input arguments #%d and #%d: Same size expected.\n"), "%datetime_a_duration", 1, 2))
    end

    d = floor(dura.duration / 86400000);
    t = modulo(dura.duration, 86400000) / 1000;

    dt.date = dt.date + d;
    dt.time = dt.time + t;
    above_day = find(dt.time >= 86400);

    if above_day <> [] then
        dt.time(above_day) = dt.time(above_day) - 86400;
        dt.date(above_day) = dt.date(above_day) + 1;
    end

    out = dt;
endfunction
