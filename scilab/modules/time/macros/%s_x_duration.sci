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

function out = %s_x_duration(coef, dura)
    if size(dura, "*") <> 1 && size(coef, "*") <> 1 && or(size(coef) <> size(dura)) then
        error(msprintf(gettext("%s: Inconsistent row/column dimensions.\n"), "%s_x_duration"))
    end

    out = mlist(["duration", "duration", "format"], dura.duration .* coef, dura.format);
endfunction
