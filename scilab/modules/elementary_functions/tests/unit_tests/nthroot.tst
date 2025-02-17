// =============================================================================
// Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - DIGITEO - Adeline CARNIS
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- ENGLISH IMPOSED -->

// <-- CLI SHELL MODE -->

// unit tests for nthroot() function
// =============================================================================
// Run with test_run('elementary_functions', 'nthroot', ['no_check_error_output'])

// 1. Interface
// ============
assert_checkfalse(execstr("nthroot()",          "errcatch") == 0);
refMsg = msprintf(_("%s: Wrong number of input argument(s): %d expected.\n"), "nthroot", 2);
assert_checkerror("nthroot()", refMsg);
assert_checkfalse(execstr("nthroot(1)",         "errcatch") == 0);
refMsg = msprintf(_("%s: Wrong number of input argument(s): %d expected.\n"), "nthroot", 2);
assert_checkerror("nthroot(1)", refMsg);
assert_checkfalse(execstr("nthroot(%i, 1)",    "errcatch") == 0);
refMsg = msprintf(_("%s: Wrong value for input argument #%d: Real numbers expected.\n"), "nthroot", 1);
assert_checkerror("nthroot(%i, 1)", refMsg);
assert_checkfalse(execstr("nthroot(%t, 1)",    "errcatch") == 0);
refMsg = msprintf(_("%s: Wrong type for input argument #%d: Must be in %s.\n"), "nthroot", 1, sci2exp("double"));
assert_checkerror("nthroot(%t, 1)", refMsg);
assert_checkfalse(execstr("nthroot(-2, %nan)", "errcatch") == 0);
refMsg = msprintf(_("%s: If x is negative, then n must contain odd integers only.\n"), "nthroot");
assert_checkerror("nthroot(-2, %nan)", refMsg);
assert_checkfalse(execstr("nthroot(-2, %eps)", "errcatch") == 0);
refMsg = msprintf(_("%s: If x is negative, then n must contain odd integers only.\n"), "nthroot");
assert_checkerror("nthroot(-2, %eps)", refMsg);
assert_checkfalse(execstr("nthroot(-3, 6)",     "errcatch") == 0);
refMsg = msprintf(_("%s: If x is negative, then n must contain odd integers only.\n"), "nthroot");
assert_checkerror("nthroot(-3, 6)", refMsg);
assert_checkfalse(execstr("nthroot(-3, 0)",     "errcatch") == 0);
refMsg = msprintf(_("%s: If x is negative, then n must contain odd integers only.\n"), "nthroot");
assert_checkerror("nthroot(-3, 0)", refMsg);
assert_checkfalse(execstr("nthroot(-%inf, -2)", "errcatch") == 0);
refMsg = msprintf(_("%s: If x is negative, then n must contain odd integers only.\n"), "nthroot");
assert_checkerror("nthroot(-%inf, -2)", refMsg);
assert_checkfalse(execstr("nthroot(-2, -2)",     "errcatch") == 0);
refMsg = msprintf(_("%s: If x is negative, then n must contain odd integers only.\n"), "nthroot");
assert_checkerror("nthroot(-2, -2)", refMsg);
assert_checkfalse(execstr("nthroot(-2, 2)",      "errcatch") == 0);
refMsg = msprintf(_("%s: If x is negative, then n must contain odd integers only.\n"), "nthroot");
assert_checkerror("nthroot(-2, 2)", refMsg);

assert_checktrue(nthroot(3, []) == 3);

// 2. Singular Values
// ==================
assert_checkalmostequal(nthroot(2   ,-2), 0.7071068, 1.e-5);
assert_checkalmostequal(nthroot(2   , 2), 1.4142136, 1.e-5);
assert_checkalmostequal(nthroot(%eps, 3), 0.0000061, 1.e-2);
assert_checkalmostequal(nthroot([1 2], 2)    , [1 1.4142136], 1.e-5);
assert_checkalmostequal(nthroot([1 2], [2 2]), [1 1.4142136], 1.e-5);
assert_checkequal(nthroot(-2,  %inf), -1);
assert_checkequal(nthroot( 3 , %inf), 1);
assert_checkequal(nthroot( 3 ,-%inf), 1);
assert_checkequal(nthroot(-3,  %inf), -1);
assert_checkequal(nthroot( 3 , %nan), %nan);
assert_checkequal(nthroot( 3 ,-%nan), %nan);
assert_checkequal(nthroot( 3 , %eps), %inf);
assert_checkequal(nthroot( 3 ,-%eps), 0);
assert_checkequal(nthroot( 3 , 0), %inf);
assert_checkequal(nthroot( 1 , 0), %nan);

assert_checkequal(nthroot([1 2], %inf), [1 1]);
assert_checkequal(nthroot([1 2], %nan), [%nan %nan]);
assert_checkequal(nthroot([1 2], %eps), [1 %inf]);
assert_checkequal(nthroot([1 2], 0),    [%nan %inf]);
assert_checkequal(nthroot([1 2], [%inf %inf]), [1 1]);
assert_checkequal(nthroot([1 2], [%nan %nan]), [%nan %nan]);
assert_checkequal(nthroot([1 2], [%eps %eps]), [1 %inf]);
assert_checkequal(nthroot([1 2], [0 0]),       [%nan %inf]);

assert_checkequal(nthroot(0, 0),    0);
assert_checkequal(nthroot(0, 7),    0);
assert_checkequal(nthroot(0,-7),    %inf);
assert_checkequal(nthroot(0, %inf), 1);
assert_checkequal(nthroot(0,-%inf), 1);
assert_checkequal(nthroot(0, %nan), %nan);
assert_checkequal(nthroot(0, %eps), 0);
assert_checkequal(nthroot(0,-%eps), %inf);

assert_checkequal(nthroot([0;-1], %inf), [1; -1]);
assert_checkequal(nthroot([0; 1], 0),    [0; %nan]);
assert_checkequal(nthroot([0;-1], [%inf; %inf]), [1; -1]);
assert_checkequal(nthroot([0; 1], [0; 0]),       [0; %nan]);

// 3. Empty Matrix
//==================
assert_checkequal(nthroot([],-2),    []);
assert_checkequal(nthroot([], 3),    []);
assert_checkequal(nthroot([], %nan), []);
assert_checkequal(nthroot([], %inf), []);
assert_checkequal(nthroot([],-%inf), []);
assert_checkequal(nthroot([], %eps), []);
assert_checkequal(nthroot([],-%eps), []);
assert_checkequal(nthroot([], 0),    []);

// 4. Limit Values
// ================
assert_checkequal(nthroot(-%inf,-7),    0);
assert_checkequal(nthroot(-%inf, 7),   -%inf);
assert_checkequal(nthroot( %inf,-7),    0);
assert_checkequal(nthroot( %inf, 7),    %inf);
assert_checkequal(nthroot( %inf, %inf), 1);
assert_checkequal(nthroot( %inf,-%inf), 1);
assert_checkequal(nthroot(-%inf, %inf), -1);
assert_checkequal(nthroot(-%inf,-%inf), -1);
assert_checkequal(nthroot( %inf, %inf), 1);
assert_checkequal(nthroot( %inf, %nan), %nan);
assert_checkequal(nthroot( %inf, %eps), %inf);
assert_checkequal(nthroot( %inf,-%eps), 0);
assert_checkequal(nthroot( %inf, 0),    %inf);

assert_checkequal(nthroot([%inf; %nan], 3),    [%inf; %nan]);
assert_checkequal(nthroot([%inf; %nan], %nan), [%nan; %nan]);
assert_checkequal(nthroot([%inf; %nan], 0),    [%inf; %nan]);
assert_checkequal(nthroot([%inf; %nan], [3; 3]),       [%inf; %nan]);
assert_checkequal(nthroot([%inf; %nan], [%nan; %nan]), [%nan; %nan]);
assert_checkequal(nthroot([%inf; %nan], [0; 0]),       [%inf; %nan]);

assert_checkequal(nthroot([%inf; %eps], %eps), [%inf; 0]);
assert_checkequal(nthroot([%inf; %eps], %nan), [%nan; %nan]);
assert_checkequal(nthroot([%inf; 1],    0),    [%inf; %nan]);
assert_checkequal(nthroot([%inf; %eps], [%eps; %eps]), [%inf; 0]);
assert_checkequal(nthroot([%inf; %eps], [%nan; %nan]), [%nan; %nan]);
assert_checkequal(nthroot([%inf; 1],    [0; 0]),       [%inf; %nan]);

// 5. Not A Number
// =================
assert_checkequal(nthroot(%nan,-2),   %nan);
assert_checkequal(nthroot(%nan, %inf), %nan);
assert_checkequal(nthroot(%nan,-%inf), %nan);
assert_checkequal(nthroot(%nan,-%eps), %nan);
assert_checkequal(nthroot(%nan, 0),    %nan);

// 6. X = %eps
// ===========
assert_checkequal(nthroot( %eps, %inf), 1);
assert_checkequal(nthroot( %eps,-%inf), 1);
assert_checkequal(nthroot(-%eps, %inf),-1);
assert_checkequal(nthroot(-%eps,-%inf),-1);
assert_checkequal(nthroot( %eps, %inf), 1);
assert_checkequal(nthroot( %eps, %nan), %nan);
assert_checkequal(nthroot( %eps, %eps), 0);
assert_checkequal(nthroot( %eps,-%eps), %inf);
assert_checkequal(nthroot( %eps, 0),    0);
