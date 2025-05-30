// =============================================================================
// Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2015 - Scilab Enterprises - Charlotte HECQUET - Calixte DENIZET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- TEST WITH GRAPHIC -->
// <-- NO CHECK REF -->
//
// <-- Short Description -->
// Unit test for fec function
//

// First test: quadrangles+mesh
a = [1 1;
     2 1;
     2 2;
     1 2];
x = a(:,1);
y = a(:,2);
triangle = [1 1 2 3 4 0];
clf();
f=gcf();
f.color_map=jet(12);
func = rand(4,1);
fec(x,y,triangle,func,mesh=%t);
assert_checkequal(f.children(1).children.data, [a func]);
assert_checkequal(f.children(1).children.triangles, triangle);
assert_checkequal(f.children(1).children.line_mode , "on");
assert_checkequal(f.children(1).children.z_bounds, [0 0]);
assert_checkequal(f.children(1).children.color_range, [0 0]);
assert_checkequal(f.children(1).children.outside_colors, [0 0]);

// Second test: quadrangles without mesh
clf();
f=gcf();
func = rand(4,1);
f.color_map=jet(12);
fec(x,y,triangle,func,mesh=%f);
assert_checkequal(f.children(1).children.data, [a func]);
assert_checkequal(f.children(1).children.triangles, triangle);
assert_checkequal(f.children(1).children.line_mode , "off");
assert_checkequal(f.children(1).children.z_bounds, [0 0]);
assert_checkequal(f.children(1).children.color_range, [0 0]);
assert_checkequal(f.children(1).children.outside_colors, [0 0]);

// Third test: quadrangles+[zmin zmax]
clf();
f=gcf();
func = rand(4,1);
f.color_map=jet(12);
fec(x,y,triangle,func,zminmax=[0.25 0.5]);
assert_checkequal(f.children(1).children.data, [a func]);
assert_checkequal(f.children(1).children.triangles, triangle);
assert_checkequal(f.children(1).children.line_mode , "off");
assert_checkequal(f.children(1).children.z_bounds, [0.25 0.5]);
assert_checkequal(f.children(1).children.color_range, [0 0]);
assert_checkequal(f.children(1).children.outside_colors, [0 0]);

// Forth test: quadrangles+colminmax
clf();
f=gcf();
func = rand(4,1);
f.color_map=jet(12);
fec(x,y,triangle,func,colminmax=[1 6]);
assert_checkequal(f.children(1).children.data, [a func]);
assert_checkequal(f.children(1).children.triangles, triangle);
assert_checkequal(f.children(1).children.line_mode , "off");
assert_checkequal(f.children(1).children.z_bounds, [0 0]);
assert_checkequal(f.children(1).children.color_range, [1 6]);
assert_checkequal(f.children(1).children.outside_colors, [0 0]);

// Fifth test: quadrangles+zminmax+colout
clf();
f=gcf();
func = rand(4,1);
f.color_map=jet(12);
fec(x,y,triangle,func,zminmax=[0.25 0.75], colout=[0 0]);
assert_checkequal(f.children(1).children.data, [a func]);
assert_checkequal(f.children(1).children.triangles, triangle);
assert_checkequal(f.children(1).children.line_mode , "off");
assert_checkequal(f.children(1).children.z_bounds, [0.25 0.75]);
assert_checkequal(f.children(1).children.color_range, [0 0]);
assert_checkequal(f.children(1).children.outside_colors, [-1 -1]);
close();
