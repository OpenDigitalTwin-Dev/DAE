// Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C)  2016 - INRIA - Serge Steer
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// For more information, see the COPYING file which you should have received
// along with this program.
function s=zp2tf(Z,P,K,dt)
    //zero pole gain to state space (simo system)
    arguments
        Z {mustBeA(Z, "double")}
        P {mustBeA(P, "double")}
        K {mustBeA(K, "double")} = 1
        dt {mustBeA(dt, ["double", "string"]), mustBeScalarOrEmpty} = []
    end

    siso=and(size(K)==1)
    if siso then Z=Z(:);end
    for k=1:size(Z,2)
        if ~isreal(poly(Z(:,k),'x')) then
            error(msprintf(_("%s: Wrong value for input argument #%d: complex zeros  must appear in complex conjugate pairs.\n"),"zp2tf",1))
        end
    end

    if ~isreal(poly(P,'x')) then
        error(msprintf(_("%s: Wrong value for input argument #%d: complex poles  must appear in complex conjugate pairs.\n"),"zp2tf",2))
    end

    if Z<>[]&(size(K,"*")<>size(Z,2)) then
        error(msprintf(_("%s: Incompatible input arguments #%d and #%d: Same column dimensions ""expected.\n"),"zp2tf",1,3))
    end

    if siso then
        s=zp2tf_siso(Z,P,K,dt)
    else
        s=[];
        for k=1:size(Z,2)
            s=[s;zp2tf_siso(Z(:,k),P,K(k),dt)];
        end
    end
endfunction

function s=zp2tf_siso(Z,P,K,dt)
    //remove poles and zeros at infinity
    Z(Z==%inf)=[];
    P(P==%inf)=[];
    if dt=="c" then var="s"; else var="z";end
    s=syslin(dt,(real(poly(Z,var,"r"))*K)/real(poly(P,var,"r")))
endfunction
