// =============================================================================
// Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - DIGITEO - Bruno JOFRET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

// test gcf()

// Check each current figure property
f = gcf();
//assert_checkequal(f.figure_position, [200,200]);
//assert_checkequal(f.figure_size, [614,568]);
assert_checkequal(f.axes_size, [610,460]);
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
assert_checkequal(f.resize, "on");
assert_checkequal(f.toolbar, "figure");
assert_checkequal(f.toolbar_visible, "on");
assert_checkequal(f.menubar, "figure");
assert_checkequal(f.menubar_visible, "on");
assert_checkequal(f.infobar_visible, "on");
assert_checkequal(f.dockable, "on");
assert_checkequal(f.layout, "none");
