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

function out = calmonths(x)
    fname = "calmonths";
    if nargin <> 1 then
        error(msprintf(_("%s: Wrong number of input argument: %d expected.\n"), fname, 1));
    end
    if type(x) <> 1 then
        error(msprintf(_("%s: Wrong type for input argument #%d: real expected.\n"), fname, 1));
    end

    out = calendarDuration(0, x, 0);
endfunction
