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

function out = %s_s_datetime(dura, dt)
    if size(dt, "*") <> 1 && size(dura, "*") <> 1 && or(size(dt) <> size(dura)) then
        error(msprintf(_("%s: Wrong size for input arguments #%d and #%d: Same size expected.\n"), "%s_s_datetime", 1, 2))
    end

    out = dt - caldays(dura);
endfunction
