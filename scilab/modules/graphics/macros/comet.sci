// Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - INRIA - Serge Steer <serge.steer@inria.fr>
// Copyright (C) 2012 - 2016 - Scilab Enterprises
// Copyright (C) 2018 - Samuel GOUGEON
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function comet(varargin)
    // Comet-like trajectory.
    // comet(y)
    // comet(x, y)
    // comet(x, y, Lf)     // Lf: Leading fraction
    // comet(x, fun)       // fun: function identifier, polynomial, rational
    // comet(x, fun, Lf)
    // comet(..., "colors",c) // c: -1, 4, "orange", "ma", "#RRGGBB", [r g b]
    //
    // History:
    //  2018: colors as "name" "nam" "#RRGGBB" or [r g b] now allowed

    nv = size(varargin)
    if nv >= 3 & varargin(nv-1)=="colors" then
        c = iscolor( varargin(nv))
        if or(isnan(c))
            msg = "%s: Argument #%d: Wrong color specification.\n";
            error(msprintf(msg, "comet", nv))
        end
        if size(c,2)==3
            c = addcolor(c)
        end
        varargin = list(varargin(1:$-2))
    else
        c = []
    end

    select size(varargin)
    case 1 then //y
        y=varargin(1)
        if or(size(y)==1) then
            x=1:size(y,"*")
        else
            x=1:size(y,1)
        end
        p=0.1
    case 2 then  //x,y
        [x,y]=varargin(1:2)
        p=0.1
    case 3 then  //x,y,p
        [x,y,p]=varargin(1:3)
    else
        msg = _("%s: Wrong number of input arguments: %d to %d expected.\n");
        error(msprintf(msg, "comet", 1, 5))
    end
    if type(x)<>1|~isreal(x) then
        msg = _("%s: Wrong type for argument #%d: Real vector expected.\n");
        error(msprintf(msg, "comet", 1))
    end
    if type(y)==13 then //a function
        prot=funcprot();funcprot(0)
        y=feval(x,y)
        funcprot(prot)
    elseif type(y)==2 then //a polynomial
        y=horner(y,x)
    elseif typeof(y)=="rational" then //a rational fraction
        y=horner(y,x)
    end
    if type(y)<>1|~isreal(y) then
        msg = _("%s: Wrong type for argument #%d: Real vector expected.\n");
        error(msprintf(msg, "comet", 2))
    end
    if type(p)<>1|~isreal(p)|size(p,"*")>1 then
        msg = _("%s: Wrong type for argument #%d: Real scalar expected.\n");
        error(msprintf(msg, "comet", 3))
    end
    if p<0|p>=1 then
        msg = _("%s: Wrong value for input argument #%d: Must be in the interval %s.\n");
        error(msprintf(msg, "comet", 3, "[0 1["))
    end

    if or(size(x)==1) then
        x=x(:)
        n=size(x,1)
        if or(size(y)==1) then
            if n<>size(y,"*") then
                msg = _("%s: Incompatible input arguments #%d and #%d: Same sizes expected.\n");
                error(msprintf(msg, "comet", 1, 2))
            end
            y=y(:)
            m=1
        else
            m=size(y,2)
            if size(y,1)<>n then
                msg = _("%s: Incompatible input arguments #%d and #%d: Same number of rows expected.\n")
                error(msprintf(msg, "comet", 1, 2))
            end
            x=x*ones(1,m)
        end
    else
        n=size(x,1)
        m=size(x,2)
        if or(size(y)<>size(x)) then
            msg = _("%s: Incompatible input arguments #%d and #%d: Same sizes expected.\n")
            error(msprintf(msg, "comet", 1, 2))
        end
    end
    fig = gcf();
    if c==[] then
        c = 1:m
    else
        if size(c,"*") <> m then
            msg = _("%s: Wrong size for argument #%d: %d expected.\n")
            error(msprintf(msg, "comet", nv, m))
        end
    end

    axes = gca();
    if axes.children==[] then
        axes.data_bounds=[min(x) min(y);max(x) max(y)];
        axes.axes_visible="on";
    else
        axes.data_bounds=[min(axes.data_bounds(1,:), [min(x) min(y)]);
        max(axes.data_bounds(2,:), [max(x) max(y)])];
    end
    //create the _head, _body and _tail polylines
    drawlater()
    _tail=[];_body=[];_head=[];
    for l=1:m
        _tail(l) = xpoly([],[]);
        _tail(l).foreground=c(l);
 
        _body(l) = xpoly([],[]);
        _body(l).foreground=c(l);
        _body(l).thickness=2;

        _head(l) = xpoly([],[],"marks");
        _head(l).mark_size_unit="point";
        _head(l).mark_size=6;
        _head(l).mark_style=9;
        _head(l).mark_foreground=c(l);
    end
    show_window();


    function anim()
        //animation loop
        k = round(p*n);
        step=ceil(n/200); //used to speed up the drawing
        for i=1:n
            for l=1:m
                _head(l).data=[x(i,l),y(i,l)];
                if i<=k then
                    _body(l).data= [_body(l).data;[x(i,l),y(i,l)]];
                else
                    _body(l).data= [_body(l).data(2:$,:);[x(i,l),y(i,l)]];
                    _tail(l).data=[ _tail(l).data;[x(i-k+1,l),y(i-k+1,l)]];
                end
            end
            if modulo(i,step)==0 then
                fig.immediate_drawing = "on"
                fig.immediate_drawing = "off"
            end
        end
        fig.immediate_drawing = "on"
        fig.immediate_drawing = "off"
        for i=1:k
            for l=1:m
                _body(l).data= _body(l).data(2:$,:);
                _tail(l).data=[_tail(l).data;[x(n-k+i,l),y(n-k+i,l)]];
            end
            if modulo(i,step)==0 then
                fig.immediate_drawing = "on"
                fig.immediate_drawing = "off"
            end
        end
        fig.immediate_drawing = "on"
        delete(_body)
    endfunction
    //not to generate an error message if the window is closed
    exec(anim, "errcatch", -1);
endfunction
