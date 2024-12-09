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

function varargout = %datetime_size(varargin)
    select nargout
    case {0,1}
        varargout(1) = size(varargin(1).date, varargin(2:$));
    case 2
        [varargout(1), varargout(2)] = size(varargin(1).date, varargin(2:$));
    end
endfunction
