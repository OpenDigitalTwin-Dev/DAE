// Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA, Serge Steer
// Copyright (C) - 2010 - DIGITEO - Michael Baudin
// Copyright (C) DIGITEO - 2011 - Allan CORNET
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.
function y = acosd(x)
    //acosd  Inverse cosine, result in degrees.
    arguments
        x {mustBeA(x, "double"), mustBeReal, mustBeInRange(x, -1, 1)}
    end

    y = 180 / %pi * acos(x);
endfunction
