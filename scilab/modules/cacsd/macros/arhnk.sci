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

function [slm]=arhnk(a,ordre,tol)

    arguments
        a {mustBeA(a, "lss")}
        ordre {mustBeA(ordre, "double"), mustBeInteger}
        tol {mustBeA(tol, "double") }= 0
    end

    if nargout <> 1 then
        msg = _("%s: Wrong number of output arguments: %d to %d expected.\n");
        error(msprintf(msg, "arhnk", 0, 1));
    end
    if a.dt<>"c" then
        msg = _("%s: Wrong type for input argument #%d: In continuous time expected.\n");
        error(msprintf(msg, "arhnk",1));
    end

    [a,b,c,d,x0,dom]=a(2:7);
    if(max(real(spec(a)))) > 0 then
        msg = _("%s: Wrong values for input argument #%d: Stable system expected.\n");
        error(msprintf(msg, "arhnk", 1));
    end
    domaine="c"
    wc=lyap(a',-b*b',domaine)
    wo=lyap(a,-c'*c,domaine)
    if nargin == 2 then 
        [t,nn]=equil1(wc,wo);
    else
        [t,nn]=equil1(wc,wo,tol);
    end;
    n1=nn(1);
    ti=inv(t);a=t*a*ti;b=t*b;c=c*ti
    wc=t*wc*t';wo=ti'*wo*ti;
    if ordre>n1 then
        ordre=n1
    end;
    if ordre==n1 then
        a=a(1:n1,1:n1);b=b(1:n1,:);c=c(:,1:n1);
        if lhs==1 then a=syslin("c",a,b,c,d, zeros(n1,1)),end
        return,
    end;
    sigma=wc(ordre+1,ordre+1)

    r=max(n1-ordre-1,1)

    n=n1
    sel=[1:ordre ordre+r+1:n];seleq=ordre+1:ordre+r
    b2=b(seleq,:);c2=c(:,seleq);
    u=-c(:,seleq)*pinv(b(seleq,:)')
    a=a(sel,sel);b=b(sel,:);c=c(:,sel);
    wo=wo(sel,sel);wc=wc(sel,sel);

    Gamma=wc*wo-sigma*sigma*eye()
    a=Gamma\(sigma*sigma*a'+wo*a*wc-sigma*c'*u*b')
    b1=Gamma\(wo*b+sigma*c'*u)
    c=c*wc+sigma*u*b';b=b1;
    d=-sigma*u+d
    //
    [n,n]=size(a)
    [u,m]=schur(a,"c")
    a=u'*a*u;b=u'*b;c=c*u;
    if m<n then
        t=sylv(a(1:m,1:m),-a(m+1:n,m+1:n),-a(1:m,m+1:n),"c")
        a=a(1:m,1:m)
        b=b(1:m,:)-t*b(m+1:n,:)
        c=c(:,1:m)
    end;
    //
    slm=syslin("c",a,b,c,d, zeros(m,1));
endfunction
