// =============================================================================
// Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 3992 -->
//
// <-- GitLab URL -->
// https://gitlab.com/scilab/scilab/-/issues/3992
//
// <-- Short Description -->
// fix some invalid dependencies on external libraries

// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->

// checks that we don't have a dependency on libmmd.dll

allDLLs = ls("SCI/bin/*.dll");
fortran = grep(allDLLs, ["libmmd", "libifcore", "lapack", "arpack"]);
nonFortran = setdiff(1:size(allDLLs, 1), fortran);

for dll = allDLLs(nonFortran)'
    stdout = unix_g("dumpbin /IMPORTS "+dll);
    found = grep(stdout, "libmmd.dll");
    if found then pause, end
end

// checks that we don't have a dependency on user32.dll

windowsOnly = grep(allDLLs, ["tk85", "tcl85", "sound", "scilocalization", "sciconsole", "noconsole", "newt", "nativewindow", "libjvm", "Windows", "windows", "core", "ast"]);
genericCode = setdiff(1:size(allDLLs, 1), windowsOnly);

for dll = allDLLs(genericCode)'
    stdout = unix_g("dumpbin /IMPORTS "+dll);
    found = grep(stdout, "USER32.dll");
    if found then pause, end
end
