// =============================================================================
// Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - Digiteo - Jean-Baptiste Silvy
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- INTERACTIVE TEST -->
// <-- TEST WITH GRAPHIC -->
// <-- ENGLISH IMPOSED -->
// <-- NO CHECK REF -->

// <-- Non-regression test for bug 4108 -->
//
// <-- GitLab URL -->
// https://gitlab.com/scilab/scilab/-/issues/4108
//
// <-- Short Description -->
// Argument location of function caption has no effect.
//

legend_positions = ["in_upper_right", "in_upper_left", "in_lower_right",..
"in_lower_left", "out_upper_right", "out_upper_left",..
"out_lower_right", "out_lower_left", "upper_caption",..
"lower_caption", "by_coordinates"];

x = 1:10;
plot(x,[x;x.^2]);
curves = gce();

// check all cases
for loc = legend_positions,
    legend(curves.children($:-1:1), ["x","x.^2"], loc);
    leg = gce();
    assert_checktrue(leg.legend_location == loc);
end
