// =============================================================================
// Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008-2008 - INRIA - Serge Steer
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->
// <-- NO CHECK REF -->

// <-- Non-regression test for bug 3289 -->
//
// <-- GitLab URL -->
// https://gitlab.com/scilab/scilab/-/issues/3289
//
// <-- Short Description -->
// When a legended curve is destroyed, legend is not updated and Scilab becomes unstable

t=linspace(0,6,100)';
plot2d(t,[sin(t),cos(t),abs(cos(t))],[1 2 3])
e = gce();
c = legend(e.children($:-1:1),string(1:3));
l = c.links;
c.legend_location = "in_lower_left";

delete(e.children(2));
if or(c.links<>l([1 3])) then pause,end
if or(c.text<>["1";"3"]) then pause,end

delete(e.children(2));
if or(c.links<>l([3]))   then pause,end
if or(c.text<>["3"])     then pause,end
