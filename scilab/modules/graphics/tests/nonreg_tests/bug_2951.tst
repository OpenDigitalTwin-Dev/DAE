// =============================================================================
// Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008-2008 - INRIA - Jean-Baptiste Silvy
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

// <-- Non-regression test for bug 2951 -->
//
// <-- GitLab URL -->
// https://gitlab.com/scilab/scilab/-/issues/2951
//
// <-- Short Description -->
// problem to refresh display with example of graypolarplot

rho=1:0.1:4;theta=(0:0.02:1)*2*%pi;
z=30+round(theta'*(1+rho.^2));
f=gcf();
f.color_map= hot(128);
clf();graypolarplot(theta,rho,z)

// should be fast
a = gca();
a.view = "3d";
for i=1:360,
  a.rotation_angles(2) = a.rotation_angles(2) + 1;
end



