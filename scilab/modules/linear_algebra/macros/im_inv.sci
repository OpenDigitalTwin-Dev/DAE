
// Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA - François DELEBECQUE
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function [X,dim,Y]=im_inv(A,B,tol)
    //[X,dim]=im_inv(A,B [,tol]) computes (A^-1)(B) i.e vectors whose
    // image through A are in range(B).
    // The dim first columns de X span (A^-1) (B)
    // tol is a threshold to test if a  subspace is included in another
    // default value tol = 100*%eps;

    arguments
        A {mustBeA(A, "double")}
        B {mustBeA(B, "double")}
        tol (1,1) {mustBeA(tol, "double")} = 100 * %eps * size(A,"*") * size(B,"*")
    end

    [nA,mA]=size(A);[nB,mB]=size(B);

    if nA<>nB then
        error(msprintf(gettext("%s: Incompatible input arguments #%d and #%d: Same sizes expected.\n"),"im_inv",1,2))
        return
    end
    // basis for im(B)
    [Y,rB]=rowcomp(B);//u=Y'

    //Trivial cases
    if rB >= nA then X=eye(mA,mA);dim=mA,return;end;
    if rB ==0 then [X,k1]=colcomp(A);dim=mA-k1,return,end
    //
    up=1:rB;low=rB+1:nA;
    A=Y*A;   //update

    //vectors with image in B
    [X,r1]=colcomp(A(low,:))
    A1=A*X;    //update
    Aup=A1(up,:);
    Alow=A1(low,:);    //Alow(:,1:ma-r1)=0 by construction
    if norm(Alow,1) <= tol*norm(Aup,1) then dim=mA,return,end
    dim=mA-r1;
endfunction



