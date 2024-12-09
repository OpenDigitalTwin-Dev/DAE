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

function out = %duration_i_s(varargin)
    //do it with double matrix to get final size
    d = varargin($-1);
    if and(size(d) <> size(varargin(1:$-2))) then
        error(msprintf(gettext("%s: Wrong size for input arguments.\n"), "%duration_i_s"))
    end
    a = [];
    [r, c] = size(d);
    x = ones(r, c);
    a(varargin(1:$-2)) = x;
    out = duration(zeros(x), 0, 0);
    out(a <> 0) = d;
    out.format = d.format;
endfunction
