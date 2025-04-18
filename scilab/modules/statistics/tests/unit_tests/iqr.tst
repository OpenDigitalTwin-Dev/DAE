// =============================================================================
// Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Adeline CARNIS
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// unit tests for iqr function 
// =============================================================================

assert_checkfalse(execstr("iqr()"   ,"errcatch") == 0);
refMsg = msprintf(_("%s: Wrong number of input arguments: %d to %d expected.\n"), "iqr", 1, 2);
assert_checkerror("iqr()", refMsg);

assert_checkequal(iqr([]), %nan);

assert_checkequal(iqr(%nan), %nan);

assert_checkequal(iqr([%nan %nan]), %nan);

// Vector
x = 1:10;
assert_checkequal(iqr(x), 5);
assert_checkequal(iqr(x'), 5);
assert_checkequal(iqr(x, "*"), 5);
assert_checkequal(iqr(x', "*"), 5);

assert_checkfalse(execstr("iqr(x, ''r'')"   ,"errcatch") == 0);
refMsg = msprintf(_("%s: Wrong dimensions for input argument #%d: A column vector or matrix expected.\n"), "iqr", 1);
assert_checkerror("iqr(x, ''r'')", refMsg);

assert_checkfalse(execstr("iqr(x, 1)"   ,"errcatch") == 0);
refMsg = msprintf(_("%s: Wrong dimensions for input argument #%d: A column vector or matrix expected.\n"), "iqr", 1);
assert_checkerror("iqr(x, 1)", refMsg);

assert_checkfalse(execstr("iqr(x'', ''c'')"   ,"errcatch") == 0);
refMsg = msprintf(_("%s: Wrong dimensions for input argument #%d: A row vector or matrix expected.\n"), "iqr", 1);
assert_checkerror("iqr(x'', ''c'')", refMsg);

assert_checkfalse(execstr("iqr(x'', 2)"   ,"errcatch") == 0);
refMsg = msprintf(_("%s: Wrong dimensions for input argument #%d: A row vector or matrix expected.\n"), "iqr", 1);
assert_checkerror("iqr(x'', 2)", refMsg);

assert_checkequal(iqr(x, "c"), 5);
assert_checkequal(iqr(x, 2), 5);

assert_checkequal(iqr(x', "r"), 5);
assert_checkequal(iqr(x', 1), 5);

assert_checkfalse(execstr("iqr(x, 3)"   ,"errcatch") == 0);
refMsg = msprintf(_("%s: Wrong value for input argument #%d: Must be in %s.\n"), "iqr", 2, sci2exp({1, 2,"r", "c", "*"}));
assert_checkerror("iqr(x, 3)", refMsg);

assert_checkfalse(execstr("iqr(x, ''t'')"   ,"errcatch") == 0);
refMsg = msprintf(_("%s: Wrong value for input argument #%d: Must be in %s.\n"), "iqr", 2, sci2exp({1, 2,"r", "c", "*"}));
assert_checkerror("iqr(x, ''t'')", refMsg);

assert_checkfalse(execstr("iqr(x, [])"   ,"errcatch") == 0);
refMsg = msprintf(_("%s: Wrong size of input argument #%d: %d x %d expected.\n"), "iqr", 2, 1, 1);
assert_checkerror("iqr(x, [])", refMsg);

// Matrix
A=[1,2,10;7,7.1,7.01];

assert_checkequal(iqr(A), 5.1);
assert_checkequal(iqr(A, "*"), 5.1);
assert_checkequal(iqr(A, "r"), [6 5.1 2.99]);
assert_checkalmostequal(iqr(A, "c"), [6.75; 0.075], [], %eps);
assert_checkequal(iqr(A, 1), [6 5.1 2.99]);
assert_checkalmostequal(iqr(A, 2), [6.75; 0.075], [], %eps);
