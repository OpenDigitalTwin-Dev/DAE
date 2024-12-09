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

function out = %calendarDuration_3_calendarDuration(cd1, cd2)
    if size(cd1, "*") <> 1 && size(cd2, "*") <> 1 && or(size(cd1) <> size(cd2)) then
        error(msprintf(_("%s: Wrong size for input arguments #%d and #%d: scalar or matrix of same size expected.\n"), "%calendarDuration_3_calendarDuration", 1, 2))
    end

    y1 = cd1.y > cd2.y; //exclution
    y2 = cd1.y < cd2.y; //equal y

    m1 = cd1.m > cd2.m; //exclution
    m2 = cd1.m < cd2.m; //equal m

    d1 = cd1.d > cd2.d; //exclution
    d2 = cd1.d < cd2.d; //equal d

    t = cd1.t <= cd2.t; //time compare

    out = ~y1 && (y2 || (~m1 && (m2 || ~d1 && (d2 || t))));
endfunction
