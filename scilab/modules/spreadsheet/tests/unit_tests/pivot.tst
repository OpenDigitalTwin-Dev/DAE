// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2023 - 3DS - Adeline CARNIS adeline.carnis@3ds.com
// Copyright (C) 2023 - 3DS - Antoine ELIAS antoine.elias@3ds.com
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->

// unit tests for pivot function
// =============================================================================

rand("seed", 0)
x1 = floor(rand(5,1)*5)-1.5;
x2 = -floor(rand(5,1)*5)+0.5;
x = ["a"; "b"; "b"; "c"; "a"];
y = ["x"; "x"; "x"; "y"; "y"];

A = table(x, y, x1, x2, "VariableNames", ["x", "y", "v1", "v2"]);

P = pivot(A, Rows="x", Columns="y");

expected = ["a" "1" "1"; "b" "2" "0"; "c" "0" "1"];
varnames = ["Var_x", "x", "y"];
assert_checkequal(P.Properties.VariableNames, varnames);
assert_checkequal(string(P), expected);

P = pivot(A, Rows="x", Columns="y", DataVariable="v2");
expected = ["a" "-2.5" "0.5"; "b" "-6" "0"; "c" "0" "-3.5"];
assert_checkequal(string(P), expected);

P = pivot(A, Rows="x", Columns="y", Method="sum", DataVariable="v1");
expected = ["a" "-0.5" "1.5"; "b" "0" "0"; "c" "0" "-0.5"];
assert_checkequal(string(P), expected);

clear t;
t.FirstName = ["Anna"; "Marius"; "Judith"; "Maria"; "David"; "Chris"; "William"; "George"; "Emmy"; "Liam"];
t.Gender = ["Female"; "Male"; "Female"; "Female"; "Male"; "Male"; "Male"; "Male"; "Female"; "Male"];
t.Age = [7;7;8;7;7;8;8;7;7;8];
t.Sport = ["Tennis"; "Rugby"; "Athelics"; "Athelics"; "Tennis"; "Rugby"; "Rugby"; "Tennis"; "Rugby"; "Rugby"];
t = struct2table(t);

P = pivot(t, Rows="Gender", Columns="Sport");
expected = ["Female" "2" "1" "1"; "Male" "0" "4" "2"];
varnames = ["Gender", "Athelics", "Rugby", "Tennis"];
assert_checkequal(P.Properties.VariableNames, varnames);
assert_checkequal(string(P), expected);

P = pivot(t, Rows="Gender", Columns="Sport", IncludeTotals=%t);
expected = ["Female" "2" "1" "1" "4"; "Male" "0" "4" "2" "6"; "Total" "2" "5" "3" "10"];
varnames = ["Gender", "Athelics", "Rugby", "Tennis", "Total"];
assert_checkequal(P.Properties.VariableNames, varnames);
assert_checkequal(string(P), expected);

P = pivot(t, Rows="Gender", Columns="Sport", Method="mean", DataVariable="Age");
expected = ["Female" "7.5" "7" "7"; "Male" "0" "7.75" "7"];
varnames = ["Gender", "Athelics", "Rugby", "Tennis"];
assert_checkequal(P.Properties.VariableNames, varnames);
assert_checkequal(string(P), expected);

P = pivot(t, Columns="Sport", IncludeTotals=%t);
expected = ["2" "5" "3" "10"];
varnames = ["Athelics", "Rugby", "Tennis", "Total"];
assert_checkequal(P.Properties.VariableNames, varnames);
assert_checkequal(string(P), expected);

P = pivot(t, Rows="Sport", IncludeTotals=%t);
expected = ["Athelics" "2"; "Rugby" "5"; "Tennis" "3"; "Total" "10"];
varnames = ["Sport", "GroupCount"];
assert_checkequal(P.Properties.VariableNames, varnames);
assert_checkequal(string(P), expected);

P = pivot(t, Rows="Age", Columns="Sport");
expected = ["7" "1" "2" "3"; "8" "1" "3" "0"];
varnames = ["Age" "Athelics" "Rugby" "Tennis"];
assert_checkequal(P.Properties.VariableNames, varnames);
assert_checkequal(string(P), expected);

P = pivot(t, Rows="Age", Columns="Sport", Method="percentage");
expected = ["7" "10" "20" "30"; "8" "10" "30" "0"];
assert_checkequal(P.Properties.VariableNames, varnames);
assert_checkequal(string(P), expected);

// With IncludedEdge
// -----------------------------------------------------------------
P = pivot(A, Rows="x", Columns="v1", ColumnsBinMethod=[-1.5 0 1.5]);
expected = ["a" "1" "1"; "b" "1" "1"; "c" "1" "0"];
varnames = ["x", "[-1.5, 0)", "[0, 1.5]"];
assert_checkequal(P.Properties.VariableNames, varnames);
assert_checkequal(string(P), expected);

P = pivot(A, Rows="x", Columns="v1", ColumnsBinMethod=[-1.5 0 1.5], IncludedEdge="right");
expected = ["a" "1" "1"; "b" "1" "1"; "c" "1" "0"];
varnames = ["x", "[-1.5, 0]", "(0, 1.5]"];
assert_checkequal(P.Properties.VariableNames, varnames);
assert_checkequal(string(P), expected);

P = pivot(A, Rows="v1", Columns="x", RowsBinMethod=[-1.5 0 1.5]);
expected = ["[-1.5, 0)" "1" "1" "1"; "[0, 1.5]" "1" "1" "0"];
varnames = ["v1", "a", "b", "c"];
assert_checkequal(P.Properties.VariableNames, varnames);
assert_checkequal(string(P), expected);

P = pivot(A, Rows="v1", Columns="x", RowsBinMethod=[-1.5 0 1.5], IncludedEdge="right");
expected = ["[-1.5, 0]" "1" "1" "1"; "(0, 1.5]" "1" "1" "0"];
varnames = ["v1", "a", "b", "c"];
assert_checkequal(P.Properties.VariableNames, varnames);
assert_checkequal(string(P), expected);

// -----------------------------------------------------------------
clear t;
t.time = [1;1;2;2];
t.location = ["indoors"; "outdoors"; "indoors"; "outdoors"];
t.data = [24;18;25;19];
T = struct2table(t);

P = pivot(T, Rows="time", Columns="location");
assert_checkequal(string(P), ["1" "1" "1"; "2" "1" "1"]);

P = pivot(T, Rows="time", Columns="location", DataVariable="data");
assert_checkequal(string(P), ["1" "24" "18"; "2" "25" "19"]);
