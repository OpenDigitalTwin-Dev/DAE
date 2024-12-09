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

function out = %timeseries_6(varargin)
    //disp("_6")
    //disp(varargin)
    if varargin(1) == "Properties" then
        props = varargin($).props;
        props.userdata = size(varargin($).vars);
        out = props;
    else
        ts = varargin($);
        idx = find(ts.props.variableNames == varargin(1));
        out = ts.vars(idx).data;
    end

endfunction
