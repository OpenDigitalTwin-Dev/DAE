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

function out = %duration_r_s(dura, coef)
    if size(dura, "*") <> 1 && size(coef, "*") <> 1 && or(size(dura) <> size(coef)) then
        error(msprintf(gettext("%s: Wrong size for input arguments #%d and #%d: scalar or matrix of same size expected.\n"), "%duration_r_s", 1, 2))
    end

    out = mlist(["duration", "duration", "format"], dura.duration ./ coef, dura.format);
endfunction
