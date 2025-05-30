
// Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function F=randpencil(eps,infi,fin,eta)
    //returns a random pencil with given kronecker structure
    //eps=[eps1,...,epsk]; epsilon blocks (size eps1x(eps1+1),....)
    //fin=[l1,...,ln]  finite eigenvalues (assumed real)  (possibly [])
    //infi=[k1,...,kp] size of J blocks, ki>=1  (infi=[] if no J blocks)
    //eta=[eta1,...,etap];   eta blocks size (eta1+1)xeta1,...)
    // epsi's should be >=0
    // etai's should be >=0
    // infi's should be >=1
    // If called with eps=[0,...,0], infi=[], fin=[], eta=[]
    // randpencil returns F=[];
    // this should be an empty matrix with zero rows and coldim(eps) columns
    // If called with eps=[], infi=[], fin=[], eta=[0,..,0]
    // randpencil returns F=[];
    // this should be an empty matrix with coldim(eta) rows and 0 columns.
    // (bad behavior of the empty matrix!!!!!)

    arguments
        eps {mustBeA(eps, "double")}
        infi {mustBeA(infi, "double")}
        fin {mustBeA(fin, ["double", "polynomial"])}
        eta {mustBeA(eta, "double")}
    end

    select type(fin)
    case 1
        if diag(fin)<>[] then 
            d = diag(fin);
            Fin = %s * eye(d) -d;
        else 
            Fin = [];
        end
    case 2
        c = companion(fin);
        Fin = %s * eye(c) - c;
    end
    deff("j=%jdrn(n)","j=zeros(n,n);for k=1:n-1;j(k,k+1)=1;end")
    deff("Lk=%eta(k)","Lk=zeros(k+1,k);if k==0 then return;end;for j=1:k;Lk(j,j)=%s;Lk(j+1,j)=-1;end");
    deff("Lk=%epsilon(k)","Lk=zeros(k,k+1);if k==0 then return;end;for j=1:k;Lk(j,j)=%s;Lk(j,j+1)=-1;end");

    J=[];
    for kk=infi;
        J=blockdiag(J,%jdrn(kk));
    end
    if J==[] then Infin=[],else Infin=%s*J-eye();end

    flageps=%f;
    Eps=[];
    seps=gsort(eps);
    if seps(1)==0 then flageps=%t;end
    if ~flageps then
        for k=seps;
            if k==0 then [p,q]=size(Eps); Eps=[Eps,zeros(p,1)];end
            if k<>0 then Eps=blockdiag(Eps,%epsilon(k));end
        end
    end

    flageta=%f;
    Eta=[];
    seta=gsort(eta);
    if seta(1)==0 then flageta=%t;end
    if ~flageta then
        for k=seta;
            if k==0 then [p,q]=size(Eta); Eta=[Eta;zeros(1,q)];end
            if k<>0 then Eta=blockdiag(Eta,%eta(k));end
        end
    end

    F=blockdiag(Eps,Infin,Fin,Eta);

    [p,q]=size(F);ncols=q;

    if flageps then
        F=[zeros(p,prod(size(eps))),F];
        if F==[] then ncols=prod(size(eps));end
    end

    if flageta then
        [p,q]=size(F);
        if F~=[] then
            F=[F;zeros(prod(size(eta)),q)];
        else
            F=[F;zeros(prod(size(eta)),ncols)];
        end
    end
    // This can be uncommented for a seemingly more random pencil!
    //[p,q]=size(F);
    //rand('seed',0);
    //rand('normal')
    //Q=rand(p,p);
    //Z=rand(q,q);
    //F=Q*F*Z;
endfunction

