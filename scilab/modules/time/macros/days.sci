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

function out = days(x)
    fname = "days";
    if nargin <> 1 then
        error(msprintf(gettext("%s: Wrong number of input argument: %d expected.\n"), fname, 1));
    end

    if type(x) == 1 then
        if x == [] then
            out = duration([]);
        else
            out = duration(x * 24, 0, 0);
        end
    elseif typeof(x) == "duration" then
        out = x.duration / (24 * 60 * 60 * 1000);
    else
        error(msprintf(gettext("%s: Wrong type for input argument #%d: real or duration expected.\n"), fname, 1));
    end
endfunction
