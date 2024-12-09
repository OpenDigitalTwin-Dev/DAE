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

function t = timeseries2table(varargin)
    data = varargin(1);
    l = list();
    for i = 1:size(data, 2)+1
        l($+1) = data.vars(i).data;
    end
    t = table(l(:), varargin(2:$));

    // update des props
    namesProps = fieldnames(t.props);
    if nargin > 1 then
        str = [];
        for i = 2:2:nargin
            v = varargin(i);
            str = [str; convstr(part(v, 1))+ part(v, 2:$)]
        end
    else
        str = "";
    end

    [k, i] = members(namesProps, str);
    namesProps = namesProps(k == 0)';
    namesProps(namesProps == "rowNames") = [];
    for k = namesProps
        t.props(k) = data.props(k);
    end
endfunction
