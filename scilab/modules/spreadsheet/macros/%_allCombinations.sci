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

function [val, b] = %_allCombinations(uniqueVal, vindex, km)
    val = list();
    b = [];

    s = []
    for k = 1:size(uniqueVal)
        s = [s size(uniqueVal(k), "*")];
    end

    for k = 1:size(uniqueVal)
        tmp = uniqueVal(k)
        index = [];
        // from ndgrid function
        ind = (ones(1, prod(s(1:k-1))) .*. (1:size(tmp, "*")) .*. ones(1, prod(s(k+1:$))))';
        v = tmp(ind);
        kdx = km(k)<>0;
        index(kdx) = vindex(k)(km(k)(kdx));
        b = [b, index(ind)];
        val(k) = v
    end
endfunction
