// Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ???? - 2016 - INRIA - Serge Steer
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.
function [y,x]=csim(u,dt,sl,x0,tol)
    //Syntax:
    //  [y [,x]]=csim(u,dt,sl,[x0])
    // simulation of the controlled linear system sl.
    // sl is assumed to be a continuous-time system.
    // u is the control and x0 the initial state.
    //
    //u can be:
    // - a function
    //    [inputs]=u(t)
    // - a list
    //    list(ut,parameter1,....,parametern) such that
    //    inputs=ut(t,parameter1,....,parametern)
    // - the character string 'impuls' for impulse response calculation
    //    (here sl is assumed SISO without direct feedthrough and x0=0)
    // - the character string 'step' for step response calculation
    //    (here sl is assumed SISO without direct feedthrough and x0=0)
    //dt is a vector of instants with dt(1) = initial time
    //                   that is:           x0=x
    //                                          dt(1)
    //
    //y matrix such that:
    //  y=[y       y  ...  y     ]
    //      dt(1)   dt(2)   dt(n)
    //x matrix such that:
    //  x=[x       x  ...  x     ]
    //      dt(1)   dt(2)   dt(n)
    //
    //See also:
    // dsimul flts ltitr rtitr ode dae
    //!

    [lhs,rhs]=argn(0)
    //
    if rhs<3 then
        msg = _("%s: Wrong number of input arguments: At least %d expected.\n");
        error(msprintf(msg, "csim", 3));
    end
    sltyp=typeof(sl)
    if and(sltyp<>["state-space" "rational" "zpk"]) then
        args=["u","dt","sl","x0","tol"];
        ierr=execstr("[y,x]=%"+typeof(sl,"overload")+"_csim("+strcat(args(1:rhs),",")+")","errcatch")
        if ierr<>0 then
            error(msprintf(_("%s: Wrong type for input argument #%d: Linear dynamical system expected.\n"),"csim",3))
        end
        return
    end
    if sltyp=="rational" then
        sl=tf2ss(sl)
    elseif sltyp=="zpk" then
        sl=zpk2ss(sl),
    end
    if sl.dt<>"c" then
        warning(msprintf(gettext("%s: Input argument #%d is assumed continuous time.\n"),"csim",1));
    end
    //
    [a,b,c,d]=abcd(sl);
    if degree(d)>0 then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: A proper system expected\n"),"csim",1));
    end
    //[ma,mb]=size(b) replaced by  ma=size(a,1);mb=size(d,2);in case of no-state system
    ma=size(a,1);
    mb=size(d,2);
    //
    imp=0;step=0
    text="if t==0 then y=0, else y=1,end"
    //
    select type(u)
    case 10 then //input given by its type (step or impuls)
        if mb<>1 then
            error(msprintf(gettext("%s: Wrong type for input argument #%d: A SIMO expected.\n"),"csim",1));
        end;
        if part(u,1)=="i" then
            //impulse response
            imp=1;
            dt(dt==0)=%eps^2;
        elseif part(u,1)=="s" then
            step=1
            if norm(d,1)<>0 then
                dt(dt==0)=%eps^2;
            end;
        else
            error(msprintf(gettext("%s: Wrong value for input argument #%d: Must be in the set {%s}.\n"),"csim",1,"""step"",""impuls"""))
        end;
        deff("[y]=u(t)",text);
    case 13 then //input given by a function of time
    case 1 then //input given by a vector of data
        [mbu,ntu]=size(u);
        if mbu<>mb | ntu<>size(dt,"*") then
            error(msprintf(gettext("%s: Incompatible input arguments #%d and #%d: Same column dimensions expected.\n"),"csim",1,2))
        end
    case 15 then  //input given by a list: function of time with parameters
        uu=u(1),
    else error(msprintf(gettext("%s: Wrong type for input argument #%d: Function expected"), "csim", 2));
    end;
    //
    if isempty(dt) then
        y = [];
        x = [];
        return
    end
    if rhs==3 then x0=sl(6),end
    if imp==1|step==1 then x0=0*x0,end
    nt=size(dt,"*");x= zeros(ma,nt);
    [a,v]=balanc(a);
    v1=v;//save for backward transformation

    //apply transformation u without matrix inversion
    [k,l]=find(v<>0) //get the permutation

    //apply right transformation
    v=v(k,l);//diagonal matrix
    c=c(:,k)*v;
    //apply left transformation
    v=diag(1 ./diag(v));b=v*b(k,:);x0=v*x0(k)

    [a,v2,bs]=bdiag(a,1);b=v2\b;c=c*v2;x0=v2\x0;
    //form the differential equation function
    if type(u)==1 then
        //form a continuous time interpolation of the given data
        ut=u;
        if min(size(ut))==1 then ut=matrix(ut,1,-1),end
        deff("[y]=u(t)",["ind=find(dt<=t);nn=ind($)"
        "if isempty(nn) then"
        "y=[]"
        "elseif (t==dt(nn)|nn==nt) then "
        "   y=ut(:,nn)"
        "else "
        "   y=ut(:,nn)+(t-dt(nn))/(dt(nn+1)-dt(nn))*(ut(:,nn+1)-ut(:,nn))"
        "end"]);
        deff("[ydot]=%sim2(%tt,%y)","ydot = [];if ~isempty(u(%tt)) then ydot = ak*%y+bk*u(%tt);end");
    elseif type(u)<>15 then
        deff("[ydot]=%sim2(%tt,%y)","ydot=ak*%y+bk*u(%tt)");
        ut=ones(mb,nt);for k=1:nt, ut(:,k)=u(dt(k)),end
    else
        %sim2=u
        tx=" ";for l=2:size(u), tx=tx+",%"+string(l-1);end;
        deff("[ydot]=sk(%tt,%y,u"+tx+")","ydot=ak*%y+bk*u(%tt"+tx+")");
        %sim2(0)=sk;u=u(1)
        deff("[ut]=uu(t)",...
        ["["+part(tx,3:length(tx))+"]=%sim2(3:"+string(size(%sim2))+")";
        "ut=ones(mb,nt);for k=1:nt, ut(:,k)=u(t(k)"+tx+"),end"])
        ut=uu(dt);
    end;

    //simulation
    k=1;
    for n=bs',
        kk=k:k+n-1;
        ak=a(kk,kk);
        bk=b(kk,:);
        nrmu=max([norm(bk*ut,1),norm(x0(kk))]);
        if nrmu > 0 then
            if rhs<5 then
                atol=1.d-12*nrmu;rtol=atol/100;
            else
                atol=tol(1);rtol=tol(2);
            end
            xkk=ode("adams",x0(kk),0,dt,rtol,atol,%sim2);
            if size(xkk,2)<>size(x,2) then
                error(msprintf(_("%s: Simulation failed before final time is reached.\n"),"csim"))
            end
            x(kk,:)=xkk;
            if imp==1 then x(kk,:)=ak*x(kk,:)+bk*ut,end
        end;
        k=k+n
    end;
    if isempty(c)
        y = d*ut;
    else
        y = c*x + d*ut
    end
    if lhs==2 then x=v1*v2*x,end
endfunction
