// Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - F. Delebecque
// Copyright (C) 2016 INRIA -S. Steer
//
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function [p1,r1]=lft(p,r,p#,r#)
    //[p1,r1]=lft(p,r,p#,r#)
    //linear fractional transform between two standard plants
    //p and p# in state space form or in transfer form.
    //r= size(p22) r#=size(p22#);
    // lft(p,r, k) is the linear fractional transform between p and
    // a controller k (k may be a gain or a controller in
    // state space form or in transfer form);
    // lft(p,k) is lft(p,r,k) with r=size of k transpose;
    //!

    [lhs,rhs]=argn(0);
    if rhs<2 then
        error(msprintf(_("%s: Wrong number of input argument: At least %d expected.\n"),"lft",2))
    end

    if type(p)==1 then
        //p=syslin([],[],[],[],p);
        dom=[]
    else
        if and(typeof(p)<>["rational","state-space"]) then
            error(msprintf(gettext("%s: Wrong type for input argument #%d: Linear state space or a transfer function expected.\n"),"lft",1))
        end
        dom=p.dt
    end

    if rhs==2 then //lft(p,p#)
        p#=r;
        if and(typeof(p#)<>["constant","rational","state-space"]) then
            error(msprintf(gettext("%s: Wrong type for input argument #%d: Linear state space or a transfer function expected.\n"),"lft",2))
        end

        r=size(p#');
        r#=size(p#)-[r(2),r(1)];
    elseif rhs>=3 then //lft(p,r,p#) or lft(p,r,p#,r#)
        if and(typeof(p#)<>["constant","rational","state-space"]) then
            error(msprintf(gettext("%s: Wrong type for input argument #%d: Linear state space or a transfer function expected.\n"),"lft",3))
        end

        if type(r)<>1 then
            error(msprintf(_("%s: Wrong type for argument %d: Real vector expected.\n"),"lft",2))
        end

        if size(r,"*")<>2 then
            error(msprintf(_("%s: Wrong size for input argument #%d: A %d elements array expected.\n"),"lft",2,2))
        end

        if or(r<=0) then
            error(msprintf(_("%s: Wrong values for input argument #%d: Elements must be positive.\n"),"lft",2))
        end

        if or(r>size(p)) then
            error(msprintf(_("%s: incompatible input arguments %d and %d\n"),"lft",1,2))
        end

        if or(r>size(p#')) then
            error(msprintf(_("%s: incompatible input arguments %d and %d\n"),"lft",2,3))
        end

        if rhs==4 then //Obsolete
            msg = "%s: %s(P, p, R, r) is obsolete. Please use %s(P, p, R) instead.\n"
            msg = msprintf(msg, "lft", "lft", "lft");
            msg = [msg, msprintf(_("This feature will be permanently removed in Scilab %s"), "2026.0.0")]
            warning(msg);

            if type(r#)<>1 then
                error(msprintf(_("%s: Wrong type for argument %d: Real vector expected.\n"),"lft",4))
            end

            r#=matrix(r#,1,-1)
            if size(r#,"*")<>2 then
                error(msprintf(_("%s: Wrong size for input argument #%d: A %d elements array expected.\n"),"lft",4,2))
            end

            if or(r#>size(p#)) then
                error(msprintf(_("%s: incompatible input arguments %d and %d\n"),"lft",3,4))
            end

            if r(2)<>size(p#,1)-r#(1)|r(1)<>size(p#,2)-r#(2) then
                expected=size(p#)-r([2 1])
                error(msprintf(_("%s: Wrong value for input argument #%d: Must be %s.\n"),"lft",4,sci2exp(expected)))
            end
        else
            r#=size(p#)-[r(2),r(1)];
        end
    end

    if type(p#)==1 then
        dom#=dom;
    elseif p#.dt==[] then
        dom#=dom;
    else
        dom#=p#.dt;
        if dom==[] then
            dom=dom#;
        elseif dom<>dom#
            error(msprintf(_("%s: incompatible time domains\n"),"lft"))
        end
    end

    pssdflag=%f;
    if typeof(p)=="state-space" then
        if typeof(p.D)=="polynomial" then
            pssdflag=%t;
        end
    end

    if typeof(p#)=="state-space" then
        if typeof(p#.D)=="polynomial" then
            pssdflag=%t;
        end
    end

    if pssdflag then
        [p1,r1]=lftpssd(p,r,p#,r#);return;
        return
    end

    if typeof(p)=="state-space"&typeof(p#)=="rational" then
        p#=tf2ss(p#)
    elseif typeof(p)=="rational"&typeof(p#)=="state-space" then
        p=tf2ss(p)
    end

    select typeof(p)
    case "constant" //p is a gain
        [nl,nc]=size(p);
        l1=1:nl-r(1);
        l2=(nl-r(1)+1):nl;
        k1=1:nc-r(2);
        k2=(nc-r(2)+1):nc;
        d11=p(l1,k1);d12=p(l1,k2);d21=p(l2,k1);d22=p(l2,k2);

        select typeof(p#)
        case "constant" //p# is a gain
            [nl,nc]=size(p#);
            l1=1:nl-r#(1);
            l2=(nl-r#(1)+1):nl;
            k1=1:nc-r#(2);
            k2=(nc-r#(2)+1):nc;
            d#11=p#(l1,k1);d#12=p#(l1,k2);d#21=p#(l2,k1);d#22=p#(l2,k2);
            g=inv(eye()-d22*d#11);
            g#=inv(eye()-d#11*d22);

            dd11=d11+d12*g#*d#11*d21;
            dd12=d12*g#*d#12;
            dd21=d#21*g*d21;
            dd22=d#22+d#21*g*d22*d#12;

            p1=[dd11,dd12;dd21,dd22];
            r1=size(dd22);

        case "state-space"
            //p# in state form
            [a#,b#1,b#2,c#1,c#2,d#11,d#12,d#21,d#22]=smga(p#,r#);
            g=inv(eye()-d22*d#11);
            g#=inv(eye()-d#11*d22);

            aa=a#+b#1*g*d22*c#1;

            bb1= b#1*g*d21;
            bb2= b#2+b#1*g*d22*d#12;

            cc1= d12*g#*c#1;
            cc2= c#2+d#21*g*d22*c#1;

            dd11=d11+d12*g#*d#11*d21;
            dd12=d12*g#*d#12;
            dd21=d#21*g*d21;
            dd22=d#22+d#21*g*d22*d#12;

            p1=syslin(dom,aa,[bb1,bb2],[cc1;cc2],[dd11,dd12;dd21,dd22]);
            r1=size(dd22);

        case "rational"
            [j11,j12,j21,j22]=smga(p#,r#);
            g=invr(eye()-d22*j11);
            g#=invr(eye()-j11*d22);

            ptfg11=d11+d12*j11*g*d21;
            ptfg12=d12*g#*j12;
            //ptfg21=j21*g*p21;
            ptfg21=j21*(eye()+d22*g#*j11)*d21;
            ptfg22=j22+j21*d22*g#*j12;

            p1=[ptfg11,ptfg12;ptfg21,ptfg22];
            r1=size(ptfg22)
        end

    case "state-space" //P is a state space representation
        //lft(standard plant in ss form,standard plant in ss form)
        [a ,b1,b2,c1,c2,d11,d12,d21,d22]=smga(p,r);
        if typeof(p#)=="constant" then //p# is a gain
            [nl,nc]=size(p#);
            l1=1:nl-r#(1);
            l2=(nl-r#(1)+1):nl;
            k1=1:(nc-r#(2));
            k2=(nc-r#(2)+1):nc;
            d#11=p#(l1,k1);d#12=p#(l1,k2);d#21=p#(l2,k1);d#22=p#(l2,k2);

            g=inv(eye()-d22*d#11);
            g#=inv(eye()-d#11*d22);

            aa=a+b2*g#*d#11*c2;

            bb1=b1+b2*g#*d#11*d21;
            bb2=b2*g#*d#12;

            cc1=c1+d12*g#*d#11*c2;
            cc2=d#21*g*c2;

            dd11=d11+d12*g#*d#11*d21;
            dd12=d12*g#*d#12;
            dd21=d#21*g*d21;
            dd22=d#22+d#21*g*d22*d#12;

            p1=syslin(dom,aa,[bb1,bb2],[cc1;cc2],[dd11,dd12;dd21,dd22]);
            r1=size(dd22);
        else  //p# in state space
            [a#,b#1,b#2,c#1,c#2,d#11,d#12,d#21,d#22]=smga(p#,r#);
            g=inv(eye()-d22*d#11);
            g#=inv(eye()-d#11*d22);
            aa=[a+b2*g#*d#11*c2 b2*g#*c#1;
            b#1*g*c2 a#+b#1*g*d22*c#1];
            bb1=[b1+b2*g#*d#11*d21;
            b#1*g*d21];
            bb2=[b2*g#*d#12;
            b#2+b#1*g*d22*d#12];
            cc1=[c1+d12*g#*d#11*c2 d12*g#*c#1];
            cc2=[d#21*g*c2 c#2+d#21*g*d22*c#1];
            dd11=d11+d12*g#*d#11*d21;
            dd12=d12*g#*d#12;
            dd21=d#21*g*d21;
            dd22=d#22+d#21*g*d22*d#12;
            p1=syslin(dom,aa,[bb1,bb2],[cc1;cc2],[dd11,dd12;dd21,dd22]);
            r1=size(dd22);return;
        end // select typeof(p#)

    case "rational" //P and P# are in rational form
        //lft(standard plant in tf form,standard plant in tf form)
        [p11,p12,p21,p22]=smga(p,r);
        [j11,j12,j21,j22]=smga(p#,r#);
        g=invr(eye()-p22*j11);
        g#=invr(eye()-j11*p22);

        ptfg11=p11+p12*j11*g*p21;
        ptfg12=p12*g#*j12;
        //ptfg21=j21*g*p21;
        ptfg21=j21*(eye()+p22*g#*j11)*p21;
        ptfg22=j22+j21*p22*g#*j12;

        p1=[ptfg11,ptfg12;ptfg21,ptfg22];
        r1=size(ptfg22)
    end
endfunction

function [lf,r1]=lftpssd(p,r,k,r2)
    //lft for pssd (inria report #1827, dec. 1992)
    [lhs,rhs]=argn(0);
    if rhs==2 then
        rhs=3;k=r;r=size(k');
    end

    if rhs==3 then
        [pp,qq]=size(p);
        l1=pp-r(1);k1=qq-r(2);
        l2=r(1);k2=r(2);
        [l3,k4]=size(k);
        l4=k1;k3=l1;
        psi=[p,[-eye(l1,k3),zeros(l1,k4);
        zeros(l2,k3),-eye(l2,k4)];
        zeros(l3,k1),-eye(l3,k2),zeros(l3,k3),k;
        eye(l4,k1),zeros(l4,k2+k3+k4)];
        w=clean(ss2tf(psi));
        lf=[zeros(k3,k1+k2),eye(k3,k3),zeros(k3,k4)]*inv(psi)* ...
            [zeros(l1+l2+l3,l4);eye(l4,l4)];
        r1=[l1,k1];
        return;
    end

    if rhs==4 then
        [pp,qq]=size(p);
        l1=pp-r(1);k1=qq-r(2);
        l2=r(1);k2=r(2);
        [pk,qk]=size(k);
        l3=pk-r2(1);k5=qk-r2(2);
        l4=r2(1);k6=r2(2);
        l5=k1;k3=l4;k4=l1;l6=k6;
        psi=...
            [p,[zeros(l1,k3),-eye(l1,k4);zeros(l2,k3+k4)],[zeros(l1,k5+k6);-eye(l2,k5),zeros(l2,k6)];
            [zeros(l3,k1),-eye(l3,k2);zeros(l4,k1+k2)],[zeros(l3,k3+k4);-eye(l4,k3),zeros(l4,k4)],k;
            [eye(l5,k1),zeros(l5,k2+k3+k4+k5+k6);zeros(l6,k1+k2+k3+k4+k5),eye(l6,k6)]];
        lf=...
            [zeros(k4,k1+k2+k3),eye(k4,k4),zeros(k4,k5+k6);
            zeros(k3,k1+k2),eye(k3,k3),zeros(k3,k4+k5+k6)]*inv(psi)* ...
                [zeros(l1+l2+l3+l4,l5+l6);eye(l5+l6,l5+l6)];
        r1=[k3,l6];
    end
endfunction
