// =============================================================================
// Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008-2008 - INRIA - Jean-Baptiste Silvy
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

// <-- Non-regression test for bug 1248 -->
//
// <-- GitLab URL -->
// https://gitlab.com/scilab/scilab/-/issues/1248
//
// <-- Short Description -->
// Postscript export export displayed text even if it is ouside the viewing area.


x=1:.5:100;
y=2:200;
scf(0);
clf();
// l,r,t,b
xsetech(arect=[.01,.01,.01,.01]);
plot2d(x,y,frameflag=8,axesflag=1);

fig = gcf();
xs2eps(fig.figure_id, TMPDIR+"/toto.eps");

// check that toto.eps does not display ticks label

