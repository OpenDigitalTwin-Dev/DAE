// Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 1985 - 2016 - INRIA - Serge Steer
// Copyright (C) 2012 - 2016 - Scilab Enterprises
// Copyright (C) 2018 - Samuel GOUGEON
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function bode(varargin)
    rhs = size(varargin)

    if rhs == 0 then
        s = poly(0, "s");
        h1 = syslin("c", (s^2+2*0.9*10*s+100)/(s^2+2*0.3*10.1*s+102.01));
        num = 22801+4406.18*s+382.37*s^2+21.02*s^3+s^4;
        den = 22952.25+4117.77*s+490.63*s^2+33.06*s^3+s^4;
        h2 = syslin("c", num/den);

        // Clears and resets the current axes where to draw the example
        ab = gca().axes_bounds
        delete(gca())
        xsetech(ab)

        // Drawing
        bode([h1; h2], 0.01, 100, ["h1"; "h2"]);
        return;
    end

    rad = %f;
    if type(varargin($)) == 10 then
        if varargin($) == "rad" then
            rad = %t;
            rhs = rhs-1;
            if type(varargin($-1)) == 10 then
                comments = varargin($-1);
                rhs = rhs-1;
            else
                comments = [];
            end
        else
            comments = varargin($);
            rhs = rhs-1;
        end
    else
        comments = [];
    end
    fname = "bode"; // For error messages
    fmax = [];
    discr = %f; // For Shannon limit
    if or(typeof(varargin(1)) == ["state-space" "rational" "zpk"]) then
        // sys, fmin, fmax [,pas] or sys, frq
        refdim = 1; // for error message
        discr = varargin(1).dt<>"c";
        if rhs == 1 then // sys
            [frq, repf] = repfreq(varargin(1), 1d-3, 1d3);
        elseif rhs == 2 then // sys, frq
            if size(varargin(2), 2) < 2 then
                error(msprintf(_("%s: Wrong size for input argument #%d: A row vector with length>%d expected.\n"), ..
                fname, 2, 1))
            end
            [frq, repf] = repfreq(varargin(1:rhs));
        elseif or(rhs == (3:4)) then // sys, fmin, fmax [,pas]
            [frq, repf] = repfreq(varargin(1:rhs));
        else
            error(msprintf(_("%s: Wrong number of input arguments: %d to %d expected.\n"), fname, 1, 5))
        end
        [phi, d] = phasemag(repf);
        if rhs >= 3 then fmax = varargin(3); end
    elseif type(varargin(1)) == 1 then
        // frq, db, phi [,comments] or frq, repf [,comments]
        refdim = 2;
        select rhs
        case 2 then // frq,repf
            frq = varargin(1);
            if size(frq, 2) < 2 then
                error(msprintf(_("%s: Wrong size for input argument #%d: A row vector with length>%d expected.\n"), ..
                fname, 1, 1))
            end
            if size(frq, 2) <> size(varargin(2), 2) then
                error(msprintf(_("%s: Incompatible input arguments #%d and #%d: Same column dimensions expected.\n"), ..
                fname, 1, 2))
            end
            [phi, d] = phasemag(varargin(2));
        case 3 then  // frq, db, phi
            [frq, d, phi] = varargin(1:rhs);
            if size(frq, 2) <> size(d, 2) then
                error(msprintf(_("%s: Incompatible input arguments #%d and #%d: Same column dimensions expected.\n"), ..
                fname, 1, 2))
            end
            if size(frq, 2) <> size(phi, 2) then
                error(msprintf(_("%s: Incompatible input arguments #%d and #%d: Same column dimensions expected.\n"), ..
                fname, 1, 3))
            end
        else
            error(msprintf(_("%s: Wrong number of input arguments: %d to %d expected.\n"), fname, 2, 4))
        end
    else
        ierr=execstr("%"+typeof(varargin(1),"overload")+"_bode(varargin(:))","errcatch")
        if ierr<>0 then
            error(msprintf(_("%s: Wrong type for input argument #%d: Linear dynamical system or row vector of floats expected.\n"),fname,1))
        end
        return
    end
    frq = frq';
    d = d';
    phi = phi';
    [n, mn] = size(d);

    if comments <> [] & size(comments, "*") <> mn then
        msg = _("%s: Incompatible input arguments #%d and #%d: Same number of elements expected.\n")
        error(mprintf(msg, fname, refdim, rhs+1))
    end

    fig = gcf();
    immediate_drawing = fig.immediate_drawing;
    fig.immediate_drawing = "off";

    sciCurAxes = gca();
    axes = sciCurAxes;
    wrect = axes.axes_bounds;

    // Magnitude plot
    // --------------
    axes.axes_bounds = [wrect(1), wrect(2), wrect(3), wrect(4)*0.5];
    axes.data_bounds = [min(frq), min(d); max(frq), max(d)];
    axes.log_flags = "lnn";
    axes.grid = color("lightgrey")*ones(1, 3);
    axes.grid_style = [8 8];
    axes.sub_ticks(1) = 8;
    axes.axes_visible = "on";
    axes.clip_state = "clipgrf";
    if size(d, 2) > 1 & size(frq, 2) == 1 then
        xpolys(frq(:, ones(1, mn)), d, 1:mn);
    else
        xpolys(frq, d, 1:mn);
    end
    // Set datatips info
    e = gce();

    for i=1:size(e.children, "*")
        e.children(i).display_function = "formatBodeMagTip"
    end

    if discr & fmax <> [] & max(frq) < fmax then
        xpoly(max(frq)*[1; 1], axes.y_ticks.locations([1 $]));
        e = gce();
        e.foreground = 5;
    end
    xtitle("", _("Frequency (Hz)"), _("Magnitude (dB)"));
    axesM = axes;

    // Phase plot
    // ----------
    axes = newaxes();
    axes.axes_bounds = [wrect(1), wrect(2)+wrect(4)*0.5, wrect(3), wrect(4)*0.5];
    axes.data_bounds = [min(frq), min(phi); max(frq), max(phi)];
    axes.log_flags = "lnn";
    axes.grid = color("lightgrey")*ones(1, 3);
    axes.grid_style = [8 8];
    axes.sub_ticks(1) = 8;
    axes.axes_visible = "on";
    axes.clip_state = "clipgrf";
    if size(phi, 2) > 1 & size(frq, 2) == 1 then
        xpolys(frq(:, ones(1, mn)), phi, 1:mn);
    else
        xpolys(frq, phi, 1:mn);
    end
    ephi = gce();
    // Set datatips info
    for i=1:size(ephi.children, "*")
        ephi.children(i).display_function = "formatBodePhaseTip";
    end

    if discr & fmax <> [] & max(frq) < fmax then
        xpoly(max(frq)*[1; 1], axes.y_ticks.locations([1 $]));
        e = gce();
        e.foreground = 5;
    end
    xtitle("", _("Frequency (Hz)"), _("Phase (degree)"));

    // Create legend
    // -------------
    if comments <> [] then
        c = %_legend(ephi.children, comments, "lower_caption");
        c.background = get(gcf(), "background");
    end

    // Rendering
    fig.immediate_drawing = immediate_drawing;

    // Post-tuning the vertical distribution of Bode plots
    // ---------------------------------------------------
    // (only after rendering, otherwise the legend position is [0 0] (bug))
    if comments <> [] then
        LgH = (1-c.position(2))*wrect(4)*0.5; // Legend height
        // Ah = Magnitude axes height.
        // Ah + Ah+LgH*Ah/0.5 = wrect(4)  => Ah (1 + 1 + 2*LgH) = wrect(4)
        Ah = wrect(4)/(2+2*LgH);
        axes.axes_bounds([2 4]) = [wrect(2)+Ah, wrect(4)-Ah];
        axesM.axes_bounds(4) = Ah*1.05;  // Magnitude plot
    end

    // Returns to the initial axes
    sca(sciCurAxes);

    // rad/s mode
    // ----------
    if rad == %t then
        // This function modifies the Bode diagrams for a rad/s display instead of Hz.
        // h is a hanlde of a figure containing Bode diagrams.
        // Ref: https://gitlab.com/scilab/forge/cpge/-/tree/master/macros/bode_Hz2rad_2.sci

        labels = [_("Phase (degree)"); _("Magnitude (dB)")];
        pos_h = [9, 5];
        for k=1:2
            for i=1:size(fig.children(k).children, 1)
                if fig.children(k).children(i).type == "Compound"
                    for j=1:size(fig.children(k).children(i).children, 1)
                        fig.children(k).children(i).children(j).data(:, 1) = fig.children(k).children(i).children(j).data(:, 1)*2*%pi;
                    end

                    // h.children(k).title.text = h.children(k).y_label.text;
                    xmin1 = fig.children(k).data_bounds(1)*2*%pi;
                    xmax1 = fig.children(k).data_bounds(2)*2*%pi;
                    ymin1 = fig.children(k).data_bounds(3);
                    ymax1 = fig.children(k).data_bounds(4);

                    rect = [xmin1, ymin1, xmax1, ymax1];
                    nb_dec = log(xmax1/xmin1)/log(10);
                    fig.children(k).x_label.text = _("Frequency (rad/s)");
                    fig.children(k).x_location = "bottom";
                    fig.children(k).y_label.text = labels(k);
                    replot(rect, fig.children(k));
                end
            end
        end
    end
endfunction
