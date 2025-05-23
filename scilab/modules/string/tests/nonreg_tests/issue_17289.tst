// =============================================================================
// Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2024 - Dassault Systèmes - Vincent COUVERT
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
//
// <-- Non-regression test for bug 17289 -->
//
// <-- GitLab URL -->
// https://gitlab.com/scilab/scilab/-/issues/17289
//
// <-- Short Description -->
// Some fields of userType objects are not displayed

doc = xmlReadStr("<root><a att=""foo"" rib=""bar""><b>Hello</b></a></root>")
