// Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
// This file is released under the 3-clause BSD license. See COPYING-BSD.

demopath = get_absolute_file_path("2d_3d_plots.dem.gateway.sce");

subdemolist = [
"plot2d"         , "plot2d.dem.sce"          ; ..
"plotyy"         , "plotyy.dem.sce"          ; ..
"plotyyy"        , "plotyyy.dem.sce"         ; ..
"param3d (1)"    , "param3d_1.dem.sce"       ; ..
"param3d (2)"    , "param3d_2.dem.sce"       ; ..
"plot3d"         , "plot3d.dem.sce"          ; ..
"plot3d (2)"     , "plot3d_2.dem.sce"        ; ..
"fplot3d"        , "fplot3d.dem.sce"         ; ..
"plot3d1"        , "plot3d1.dem.sce"         ; ..
"fplot3d1"       , "fplot3d1.dem.sce"        ; ..
"contour"        , "contour.dem.sce"         ; ..
"contourf"       , "contourf.dem.sce"        ; ..
"contourf 2"     , "contourf2.dem.sce"       ; ..
"champ"          , "champ.dem.sce"           ; ..
"fchamp"         , "fchamp.dem.sce"          ; ..
"grayplot"       , "grayplot.dem.sce"        ; ..
"fgrayplot"      , "fgrayplot.dem.sce"       ; ..
"polarplot"      , "polarplot.dem.sce"       ; ..
"errbar"         , "errbar.dem.sce"          ; ..
"geom3d"         , "geom3d.dem.sce"          ; ..
"contour (3d) 2" , "contour_3d_2.dem.sce"    ; ..
"contour (3d) 3" , "contour_3d_3.dem.sce"    ; ..
"contour (3d) 4" , "contour_3d_4.dem.sce"    ; ..
"subplot"        , "subplot.dem.sce"         ; ..
"surf"           , "surf.dem.sce"            ;
];

subdemolist(:,2) = demopath + subdemolist(:,2);
clear demopath;
