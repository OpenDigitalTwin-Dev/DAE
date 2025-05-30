// Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2004-2006 - INRIA - Farid Belahcene
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function varargout = xlabel(varargin)
    // XLABEL function
    // Label x

    if size(varargin)<1 then
        error(msprintf(gettext("%s: Wrong number of input argument(s): At least %d expected.\n"), "xlabel", 1));
    end

    if argn(1) > 1 then
        msg = gettext("%s: Wrong number of output argument(s): at most %d expected.\n")
        error(msprintf(msg, "xlabel", 1));
    end

    varargin = list("x_label",varargin(1:$));

    h = TitleLabel(varargin(:));

    if argn(1) == 1
        varargout(1) = h
    end

endfunction
