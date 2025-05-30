// =============================================================================
// Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - DIGITEO - Bruno JOFRET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- TEST WITH GRAPHIC -->
// <-- NO CHECK REF -->
//
// test plot2d()

// Check each default property
plot2d();
f = gcf();
a = gca();
c = a.children(1); // compound
p1 = c.children(1); // polyline 1
p2 = c.children(2); // polyline 2
p3 = c.children(3); // polyline 3

// Check figure
//assert_checkequal(f.figure_position, [200,200]);
//assert_checkequal(f.figure_size, [614,568]);
//assert_checkequal(f.axes_size, [610,460]);
assert_checkequal(f.auto_resize, "on");
assert_checkequal(f.viewport, [0,0]);
assert_checkequal(f.figure_name, gettext("Graphic window number %d"));
assert_checkequal(f.figure_id, 0);
assert_checkequal(f.pixel_drawing_mode, "copy");
assert_checkequal(f.anti_aliasing, "8x");
assert_checkequal(f.immediate_drawing, "on");
assert_checkequal(f.background, -2);
assert_checkequal(f.visible, "on");
assert_checkequal(f.rotation_style, "unary");
assert_checkequal(f.event_handler, "");
assert_checkequal(f.event_handler_enable, "off");
assert_checkequal(f.user_data, []);
assert_checkequal(f.tag, "");

// Check Axes
assert_checkequal(a.visible, "on");
assert_checkequal(a.axes_visible, ["on" "on" "on"]);
assert_checkequal(a.axes_reverse, ["off" "off" "off"]);
assert_checkequal(a.grid, [-1 -1]);
assert_checkequal(a.grid_position, "background");
assert_checkequal(a.x_location, "bottom");
assert_checkequal(a.y_location, "left");
assert_checkequal(a.auto_ticks, ["on" "on" "on"]);
//assert_checkequal(a.x_ticks.locations, [0:0.1:1]');
//assert_checkequal(a.y_ticks.locations, [0:0.1:1]');
//assert_checkequal(a.z_ticks.locations, []);
assert_checkequal(a.box, "on");
assert_checkequal(a.filled, "on");
assert_checkequal(a.sub_ticks, [0, 4]);
assert_checkequal(a.font_style, 6);
assert_checkequal(a.font_size, 1);
assert_checkequal(a.font_color, -1);
assert_checkequal(a.fractional_font, "on");
assert_checkequal(a.isoview, "off");
assert_checkequal(a.cube_scaling, "off");
assert_checkequal(a.view, "2d");
assert_checkequal(a.rotation_angles, [0, 270]);
assert_checkequal(a.log_flags, "nnn");
assert_checkequal(a.tight_limits, ["on", "on", "on"]);
assert_checkequal(a.data_bounds, [0, -2; 2 * %pi, 2]);
assert_checkequal(a.zoom_box, []);
assert_checkequal(a.margins, [0.125, 0.125, 0.125, 0.125]);
assert_checkequal(a.axes_bounds, [0, 0, 1, 1]);
assert_checkequal(a.auto_clear, "off");
assert_checkequal(a.auto_scale, "on");
assert_checkequal(a.hidden_axis_color, 4);
assert_checkequal(a.hiddencolor, 4);
assert_checkequal(a.line_mode, "on");
assert_checkequal(a.line_style, 1);
assert_checkequal(a.thickness, 1);
assert_checkequal(a.mark_mode, "off");
assert_checkequal(a.mark_style, 0);
assert_checkequal(a.mark_size_unit, "tabulated");
assert_checkequal(a.mark_size, 0);
assert_checkequal(a.mark_foreground, -1);
assert_checkequal(a.mark_background, -2);
assert_checkequal(a.foreground, -1);
assert_checkequal(a.background, -2);
assert_checkequal(a.arc_drawing_method, "lines");
assert_checkequal(a.clip_state, "clipgrf");
assert_checkequal(a.clip_box, []);
assert_checkequal(a.user_data, []);

// Check Compound Object
assert_checkequal(c.parent.type, "Axes");
assert_checkequal(c.children(:).type, ["Polyline"; "Polyline"; "Polyline"]);
assert_checkequal(c.visible, "on");
assert_checkequal(c.user_data, []);

// Check Polyline 1
demoData = [0:0.1:2*%pi]';
assert_checkequal(p1.parent.type, "Compound");
assert_checkequal(p1.children, []);
assert_checkequal(p1.visible, "on");
assert_checkequal(p1.data, [demoData, sin(3 * demoData)]);
assert_checkequal(p1.closed, "off");
assert_checkequal(p1.line_mode, "on");
assert_checkequal(p1.fill_mode, "off");
assert_checkequal(p1.line_style, 1);
assert_checkequal(p1.thickness, 1);
assert_checkequal(p1.arrow_size_factor, 1);
assert_checkequal(p1.polyline_style, 1);
assert_checkequal(p1.foreground, 3);
assert_checkequal(p1.background, -2);
assert_checkequal(p1.interp_color_vector, []);
assert_checkequal(p1.interp_color_mode, "off");
assert_checkequal(p1.mark_mode, "off");
assert_checkequal(p1.mark_style, 0);
assert_checkequal(p1.mark_size_unit, "tabulated");
assert_checkequal(p1.mark_size, 0);
assert_checkequal(p1.mark_foreground, -1);
assert_checkequal(p1.mark_background, -2);
assert_checkequal(p1.x_shift, []);
assert_checkequal(p1.y_shift, []);
assert_checkequal(p1.z_shift, []);
assert_checkequal(p1.bar_width, 0);
assert_checkequal(p1.clip_state, "clipgrf");
assert_checkequal(p1.clip_box, []);
assert_checkequal(p1.user_data, []);

// Check Polyline 2
assert_checkequal(p2.parent.type, "Compound");
assert_checkequal(p2.children, []);
assert_checkequal(p2.visible, "on");
assert_checkequal(p2.data, [demoData, sin(2 * demoData)]);
assert_checkequal(p2.closed, "off");
assert_checkequal(p2.line_mode, "off");
assert_checkequal(p2.fill_mode, "off");
assert_checkequal(p2.line_style, 1);
assert_checkequal(p2.thickness, 1);
assert_checkequal(p2.arrow_size_factor, 1);
assert_checkequal(p2.polyline_style, 1);
assert_checkequal(p2.foreground, -1);
assert_checkequal(p2.background, -2);
assert_checkequal(p2.interp_color_vector, []);
assert_checkequal(p2.interp_color_mode, "off");
assert_checkequal(p2.mark_mode, "on");
assert_checkequal(p2.mark_style, 2);
assert_checkequal(p2.mark_size_unit, "tabulated");
assert_checkequal(p2.mark_size, 0);
assert_checkequal(p2.mark_foreground, -1);
assert_checkequal(p2.mark_background, -2);
assert_checkequal(p2.x_shift, []);
assert_checkequal(p2.y_shift, []);
assert_checkequal(p2.z_shift, []);
assert_checkequal(p2.bar_width, 0);
assert_checkequal(p2.clip_state, "clipgrf");
assert_checkequal(p2.clip_box, []);
assert_checkequal(p2.user_data, []);

// Check Polyline 3
assert_checkequal(p3.parent.type, "Compound");
assert_checkequal(p3.children, []);
assert_checkequal(p3.visible, "on");
assert_checkequal(p3.data, [demoData, sin(demoData)]);
assert_checkequal(p3.closed, "off");
assert_checkequal(p3.line_mode, "off");
assert_checkequal(p3.fill_mode, "off");
assert_checkequal(p3.line_style, 1);
assert_checkequal(p3.thickness, 1);
assert_checkequal(p3.arrow_size_factor, 1);
assert_checkequal(p3.polyline_style, 1);
assert_checkequal(p3.foreground, -1);
assert_checkequal(p3.background, -2);
assert_checkequal(p3.interp_color_vector, []);
assert_checkequal(p3.interp_color_mode, "off");
assert_checkequal(p3.mark_mode, "on");
assert_checkequal(p3.mark_style, 1);
assert_checkequal(p3.mark_size_unit, "tabulated");
assert_checkequal(p3.mark_size, 0);
assert_checkequal(p3.mark_foreground, -1);
assert_checkequal(p3.mark_background, -2);
assert_checkequal(p3.x_shift, []);
assert_checkequal(p3.y_shift, []);
assert_checkequal(p3.z_shift, []);
assert_checkequal(p3.bar_width, 0);
assert_checkequal(p3.clip_state, "clipgrf");
assert_checkequal(p3.clip_box, []);
assert_checkequal(p3.user_data, []);
