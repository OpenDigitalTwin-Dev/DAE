// =============================================================================
// Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Clément DAVID
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- XCOS TEST -->
// <-- ENGLISH IMPOSED -->
// <-- NO CHECK REF -->
//
// <-- Non-regression test for bug 8674 -->
//
// <-- GitLab URL -->
// https://gitlab.com/scilab/scilab/-/issues/8674
//
// <-- Short Description -->
// Check that an erroneous "From workspace" block does not produce an error.
//

result = importXcosDiagram(SCI + "/modules/xcos/tests/nonreg_tests/bug_8674.zcos");
if (result <> %T) then pause,end

// overwrtie dialogs
prot = funcprot();
funcprot(0);

function result = dialog(labels,valueini)
    result = valueini;
endfunction

funcprot(prot);


%cpr = xcos_simulate(scs_m, 4);

