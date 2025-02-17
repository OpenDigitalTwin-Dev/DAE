// Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA -
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function [K,Y,err]=leqe(P21,Qx)
    arguments
        P21 {mustBeA(P21, "lss")}
        Qx {mustBeA(Qx, "double")}
    end

    [A,B1,C2,D21,xo,dom]=P21(2:7)
    [KT,Y,err]=leqr(syslin(dom,A',C2',B1',D21'),Qx);
    K=KT';
endfunction
