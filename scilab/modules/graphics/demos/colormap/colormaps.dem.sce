// Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Pierre MARECHAL
// Copyright (C) 2016 - Scilab Enterprises - Paul Bignier
// Copyright (C) 2017 - Samuel GOUGEON
//
// This file is released under the 3-clause BSD license. See COPYING-BSD.

prot = funcprot();
funcprot(0);

function demo_colormaps()

    if get("colormap_frame") <> [] then
        h = get("colormap_frame");
        f = h.parent.parent;
        delete(f)
    end

    // Parameters
    // =========================================================================

    frame_w              = 150;     // Frame width
    frame_h              = 330;     // Frame height

    plot_w               = 600;     // Plot width
    plot_h               = 550;     // Plot height

    margin_x             = 15;      // Horizontal margin between each elements
    margin_y             = 15;      // Vertical margin between each elements

    defaultfont          = "arial"; // Default Font
    text_font_size       = 12;      // Text font size

    // Figure creation
    // =========================================================================

    axes_w       = 3*margin_x + frame_w + plot_w;    // axes width
    axes_h       = 2*margin_y + max(frame_h,plot_h); // axes height

    if get("colormaps_figure") <> [] then
        fig_handle = get("colormaps_figure");
        delete(fig_handle.children);
    else
        close(100002)
        // Create a figure
        fig_handle = figure( ...
        "figure_id", 100002, ...
        "infobar_visible", "off", ...
        "toolbar_visible", "off", ...
        "toolbar", "none", ...
        "default_axes", "on", ...
        "layout", "gridbag", ...
        "visible", "off", ...
        "background", -2, ...
        "figure_position", [0 0], ...
        "tag", "colormaps_figure", ...
        "axes_size", [axes_w axes_h]);
    end    

    c = createConstraints("gridbag", [2 1 1 1], [1 1], "vertical", "right");
    // Create an empty panel on the right to occupy the 3/4 of the screen (figure plot)
    empty_frame = uicontrol(fig_handle, "style", "frame","constraints", c);

    c = createConstraints("gridbag", [1 1 1 5], [0 1], "both", "left", [0 0], [90 0]);
    u = uicontrol(fig_handle, ...
    "style", "frame", ...
    "backgroundcolor", [1 1 1], ...
    "layout", "border", ...
    "constraints", c);

    // Add top and bottom empty panels to pad the upcoming colormap panel
    c = createConstraints("border", "top", [0 80]);
    top = uicontrol(u, "style", "frame", "backgroundcolor", [1 1 1], "constraints", c);
    c = createConstraints("border", "bottom", [0 80]);
    bottom = uicontrol(u, "style", "frame", "backgroundcolor", [1 1 1], "constraints", c);

    // Border for the colormap frame
    b_f_colormap = createBorderFont("", 15);
    b_l_colormap = createBorder("line", "navy", 2);
    b_colormap   = createBorder("titled", b_l_colormap, "Colormap", "center", "top", b_f_colormap, "navy");

    colormap_frame = uicontrol(u, ...
    "style", "frame", ...
    "backgroundcolor", [1 1 1], ...
    "border", b_colormap, ...
    "layout", "gridbag", ...
    "tag", "colormap_frame");

    demo_viewCode("colormaps.dem.sce");

    fig_handle.color_map = jet(128);

    // Radio buttons
    // =========================================================================
    colormaps = ["Jet" "Parula" "HSV" "Hot" "Gray" ..
                "Winter" "Spring" "Summer" "Autumn" ..
                "Bone" "Copper" "Pink" "Rainbow" "Ocean" "White"];
    for i = 1:size(colormaps,"*")
        mapname = colormaps(i);
        c = createConstraints("gridbag", [1 i 1 1], [1 1], "both", "upper");
        disk_radio = uicontrol( ...
        "parent"       , colormap_frame, ...
        "style"        , "radiobutton", ...
        "string"       , mapname, ...
        "fontname"     , defaultfont, ...
        "fontunits"    , "points", ...
        "fontsize"     , text_font_size, ...
        "value"        , 1*(i==1), ...
        "background"   , [1 1 1], ...
        "callback"     , "demo_update_misc", ...
        "groupname"    , "colormap_demo", ..
        "constraints"  , c, ...
        "tag"          , mapname+"_radio");
    end

    // Plots creation
    // =========================================================================

    drawlater();

    my_plot_region_pos_x = ((2*margin_x+frame_w)/axes_w);
    my_plot_region_pos_y = 0;
    my_plot_region_w     = 1-my_plot_region_pos_x;
    my_plot_region_h     = 1;

    // First plot : grayplot();  top - left

    my_plot_1_axes  = newaxes();
    my_plot_1_pos_x = ((2*margin_x+frame_w)/axes_w);
    my_plot_1_pos_y = 0.05;
    my_plot_1_w     = (1-my_plot_region_pos_x)/2.3;
    my_plot_1_h     = 1/2.3;
    my_plot_1_axes.axes_bounds = [ my_plot_1_pos_x my_plot_1_pos_y my_plot_1_w my_plot_1_h ];
    grayplot();
    my_plot_1_axes.tight_limits = "on";
    e = my_plot_1_axes.children(1);
    z = e.data.z
    e.data.z(1,1) = min(z)+(max(z)-min(z))/127*128;

    // Second plot : plot3d1(); top right

    my_plot_2_axes  = newaxes();
    // my_plot_2_pos_x = ((2*margin_x+frame_w)/axes_w) + my_plot_region_w/2;
    my_plot_2_pos_x = my_plot_1_pos_x + my_plot_1_w;
    my_plot_2_pos_y = 0;
    my_plot_2_w     = (1-my_plot_region_pos_x)/2 + 0.03;
    my_plot_2_h     = 1/2;
    my_plot_2_axes.axes_bounds = [ my_plot_2_pos_x my_plot_2_pos_y my_plot_2_w my_plot_2_h ];
    plot3d1();
    my_plot_2_axes.rotation_angles = [80 45];

    // Third plot : grayplot();  bottom - left

    my_plot_3_axes  = newaxes();
    my_plot_3_pos_x = ((2*margin_x+frame_w)/axes_w);
    my_plot_3_pos_y = 1/2;
    my_plot_3_w     = (1-my_plot_region_pos_x)/2;
    my_plot_3_h     = 1/2;
    my_plot_3_axes.axes_bounds = [ my_plot_3_pos_x my_plot_3_pos_y my_plot_3_w my_plot_3_h ];
    Matplot()
    isoview
    my_plot_3_axes.axes_bounds(3:4) = [0.33 0.45];
    xtitle("Matplot","","")

    // Fourth plot : knot();  bottom - right

    my_plot_4_axes  = newaxes();
    // my_plot_4_pos_x = ((2*margin_x+frame_w)/axes_w) + my_plot_region_w/2;
    my_plot_4_pos_x = my_plot_3_pos_x +  my_plot_3_w - 0.05;
    my_plot_4_pos_y = 1/2;
    my_plot_4_w     = 0.5;
    my_plot_4_h     = 1/2;
    my_plot_4_axes.axes_bounds = [ my_plot_4_pos_x my_plot_4_pos_y my_plot_4_w my_plot_4_h ];

    deff("[x,y,z] = knot(u,v)",..
    ["vv = ones(v)";
     "uu = ones(u);";
     "x = (5.*cos(u)+cos(u).*cos(v))";
     "y = (5.*sin(u)+sin(u).*cos(v))";
     "z = (uu.*sin(v))";])

    nx=60;
    Nx=(0:nx)/nx;
    ny=20;
    Ny=(0:ny)/ny;

    [xx,yy,zz]=eval3dp(knot,2*%pi*Nx,2*%pi*Ny);
    XXX=[-xx xx];
    YYY=[-yy zz];
    ZZZ=[-zz yy];
    kk1=[1:size(zz,2)];

    kk1=modulo(kk1,60)*2;
    kk2=kk1;
    KKK=list(ZZZ,[kk1 kk2]);
    plot3d(XXX,YYY,KKK,35,70," @ @ ",[2,1,4],[-6,6,-6,6,-6,6]);

    // define colormap
    f = gcf();
    f.color_map = jet(128);

    my_plot_2_axes.background = color(240,240,240);
    my_plot_4_axes.background = color(240,240,240);

    // Color bar
    colorbar(0,128, [0 128], "%d");
    gcf().children(1).axes_bounds = [0.14 0.1 0.1 0.8];
    gcf().figure_name = "Jet Colormap";
    drawnow();

    fig_handle.visible = "on";
endfunction


function demo_update_misc()
    my_figure    = gcf();
    my_figure.immediate_drawing = "off";
    set(gcbo, "value", 1);
    colormap_name = strtok(get(gcbo,"tag"),"_");
    execstr(["my_figure.color_map = "+convstr(colormap_name,'l')+"(128);"
             "my_figure.figure_name = """+colormap_name+""""]);

    // Gray background for 3d axes
    for i = 1:size(my_figure.children,"*"),
        child = my_figure.children(i);
        if child.type=="Axes" then
            if child.view=="3d" then
                child.background = color(240,240,240);
            end
        end
    end

    // Plot
    my_figure.immediate_drawing = "on";
endfunction

funcprot(prot);
demo_colormaps();
clear demo_colormaps;
