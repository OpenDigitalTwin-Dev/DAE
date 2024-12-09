// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2023 - 3DS - Adeline CARNIS adeline.carnis@3ds.com
// Copyright (C) 2023 - 3DS - Antoine ELIAS antoine.elias@3ds.com
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->

// unit tests for groupcounts function
// =============================================================================

// First test
// -----------------------------------------------------------------------------
rand("seed", 0)
x1 = floor(rand(5,1)*5)-1.5;
x2 = -floor(rand(5,1)*5)+0.5;
A = table(["a"; "b"; "b"; "c"; "a"], x1, x2);

G = groupcounts(A, "Var1");
expected = ["a" "2"; "b" "2"; "c" "1"];
assert_checkequal(G.Properties.VariableNames, ["Var1", "GroupCount"]);
assert_checkequal(string(G), expected);

G = groupcounts(A, 1);
assert_checkequal(G.Properties.VariableNames, ["Var1", "GroupCount"]);
assert_checkequal(string(G), expected);


G = groupcounts(A, "Var1", "IncludePercentGroups", %t);
expected = ["a" "2" "40"; "b" "2" "40"; "c" "1" "20"];
assert_checkequal(G.Properties.VariableNames, ["Var1", "GroupCount", "Percent"]);
assert_checkequal(string(G), expected);

G = groupcounts(A, "Var2", [-3 -1 1 3]);
expected = ["[-3, -1)" "1"; "[-1, 1)" "2"; "[1, 3]" "2"];
assert_checkequal(G.Properties.VariableNames, ["disc_Var2", "GroupCount"]);
assert_checkequal(string(G), expected);

G = groupcounts(A, 2, [-3 -1 1 3]);
assert_checkequal(G.Properties.VariableNames, ["disc_Var2", "GroupCount"]);
assert_checkequal(string(G), expected);


G = groupcounts(A, ["Var2", "Var3"], [-5 0 5]);
expected = ["[-5, 0)" "[-5, 0)" "3"; "[0, 5]" "[-5, 0)" "1"; "[0, 5]" "[0, 5]" "1"];
assert_checkequal(G.Properties.VariableNames, ["disc_Var2", "disc_Var3", "GroupCount"]);
assert_checkequal(string(G), expected);

G = groupcounts(A, ["Var2", "Var3"], [-5 0 5], "IncludePercentGroups", %t);
expected = ["[-5, 0)" "[-5, 0)" "3" "60"; "[0, 5]" "[-5, 0)" "1" "20"; "[0, 5]" "[0, 5]" "1" "20"];
assert_checkequal(G.Properties.VariableNames, ["disc_Var2", "disc_Var3", "GroupCount", "Percent"]);
assert_checkequal(string(G), expected);

G = groupcounts(A, ["Var2", "Var3"], [-5 0 5], "IncludeEmptyGroups", %t);
expected = ["[-5, 0)" "[-5, 0)" "3"; "[-5, 0)" "[0, 5]" "0"; "[0, 5]" "[-5, 0)" "1"; "[0, 5]" "[0, 5]" "1"];
assert_checkequal(G.Properties.VariableNames, ["disc_Var2", "disc_Var3", "GroupCount"]);
assert_checkequal(string(G), expected);

G = groupcounts(A, ["Var2", "Var3"], [-5 0 5], "IncludeEmptyGroups", %t, "IncludePercentGroups", %t);
expected = ["[-5, 0)" "[-5, 0)" "3" "60"; "[-5, 0)" "[0, 5]" "0" "0"; "[0, 5]" "[-5, 0)" "1" "20"; "[0, 5]" "[0, 5]" "1" "20"];
assert_checkequal(G.Properties.VariableNames, ["disc_Var2", "disc_Var3", "GroupCount", "Percent"]);
assert_checkequal(string(G), expected);

G = groupcounts(A, [2, 3], [-5 0 5], "IncludeEmptyGroups", %t, "IncludePercentGroups", %t);
assert_checkequal(G.Properties.VariableNames, ["disc_Var2", "disc_Var3", "GroupCount", "Percent"]);
assert_checkequal(string(G), expected);

G = groupcounts(A, ["Var1", "Var3"], {"none", [-5 0 5]});
expected = ["a" "[-5, 0)" "1"; "a" "[0, 5]" "1"; "b" "[-5, 0)" "2"; "c" "[-5, 0)" "1"];
assert_checkequal(G.Properties.VariableNames, ["Var1", "disc_Var3", "GroupCount"]);
assert_checkequal(string(G), expected);

G = groupcounts(A, ["Var2", "Var3"], {[-5 0 5], [-4 -2 0]});
expected = ["[-5, 0)" "[-4, -2)" "3"; "[0, 5]" "[-4, -2)" "1"; "[0, 5]" "<undefined>" "1"];
assert_checkequal(G.Properties.VariableNames, ["disc_Var2", "disc_Var3", "GroupCount"]);
assert_checkequal(string(G), expected);

// With IncludedEdge
// -----------------------------------------------------------------------------
G = groupcounts(A, "Var2", [-1.5 -0.5 0.5 1.5]);
expected = ["[-1.5, -0.5)" "1"; "[-0.5, 0.5)" "2"; "[0.5, 1.5]" "2"];
assert_checkequal(G.Properties.VariableNames, ["disc_Var2", "GroupCount"]);
assert_checkequal(string(G), expected);

G = groupcounts(A, "Var2", [-1.5 -0.5 0.5 1.5], "IncludedEdge", "left");
assert_checkequal(G.Properties.VariableNames, ["disc_Var2", "GroupCount"]);
assert_checkequal(string(G), expected);

G = groupcounts(A, "Var2", [-1.5 -0.5 0.5 1.5], "IncludedEdge", "right");
expected = ["[-1.5, -0.5]" "3"; "(0.5, 1.5]" "2"];
assert_checkequal(G.Properties.VariableNames, ["disc_Var2", "GroupCount"]);
assert_checkequal(string(G), expected);

G = groupcounts(A, ["Var2", "Var3"], [-4 -1.5 0 1.5]);
expected = ["[-1.5, 0)" "[-4, -1.5)" "3"; "[0, 1.5]" "[-4, -1.5)" "1"; "[0, 1.5]" "[0, 1.5]" "1"];
assert_checkequal(G.Properties.VariableNames, ["disc_Var2", "disc_Var3", "GroupCount"]);
assert_checkequal(string(G), expected);

G = groupcounts(A, ["Var2", "Var3"], [-4 -1.5 0 1.5], "IncludedEdge", "left");
assert_checkequal(G.Properties.VariableNames, ["disc_Var2", "disc_Var3", "GroupCount"]);
assert_checkequal(string(G), expected);

G = groupcounts(A, ["Var2", "Var3"], [-4 -1.5 0 1.5], "IncludedEdge", "right");
expected = ["[-4, -1.5]" "[-4, -1.5]" "1"; "(-1.5, 0]" "[-4, -1.5]" "2"; "(0, 1.5]" "[-4, -1.5]" "1"; "(0, 1.5]" "(0, 1.5]" "1"];
assert_checkequal(G.Properties.VariableNames, ["disc_Var2", "disc_Var3", "GroupCount"]);
assert_checkequal(string(G), expected);

G = groupcounts(A, ["Var2", "Var3"],{[-1.5 -0.5 0.5 1.5], [-4 -1.5 0 1.5]});
expected = ["[-1.5, -0.5)" "[-4, -1.5)" "1"; "[-0.5, 0.5)" "[-4, -1.5)" "2"; "[0.5, 1.5]" "[-4, -1.5)" "1"; "[0.5, 1.5]" "[0, 1.5]" "1"];
assert_checkequal(G.Properties.VariableNames, ["disc_Var2", "disc_Var3", "GroupCount"]);
assert_checkequal(string(G), expected);

G = groupcounts(A, ["Var2", "Var3"],{[-1.5 -0.5 0.5 1.5], [-4 -1.5 0 1.5]}, "IncludedEdge", "left");
assert_checkequal(G.Properties.VariableNames, ["disc_Var2", "disc_Var3", "GroupCount"]);
assert_checkequal(string(G), expected);

G = groupcounts(A, ["Var2", "Var3"],{[-1.5 -0.5 0.5 1.5], [-4 -1.5 0 1.5]}, "IncludedEdge", "right");
expected = ["[-1.5, -0.5]" "[-4, -1.5]" "3"; "(0.5, 1.5]" "[-4, -1.5]" "1"; "(0.5, 1.5]" "(0, 1.5]" "1"];
assert_checkequal(G.Properties.VariableNames, ["disc_Var2", "disc_Var3", "GroupCount"]);
assert_checkequal(string(G), expected);

// Second test
// -----------------------------------------------------------------------------
// Data from www.historique-meteo.net
T = readtable(fullfile(SCI, "modules", "spreadsheet", "tests", "unit_tests","meteo_data_bordeaux.csv"));

G = groupcounts(T, "OPINION");
expected = ["météo correcte" "7"; "météo défavorable" "3"; "météo favorable" "10"; "météo idéale" "10"];
assert_checkequal(G.Properties.VariableNames, ["OPINION", "GroupCount"]);
assert_checkequal(string(G), expected);

G = groupcounts(T, "OPINION", "IncludePercentGroups", %t);
expected = ["météo correcte" "7" "23.333333"; "météo défavorable" "3" "10"; "météo favorable" "10" "33.333333"; "météo idéale" "10" "33.333333"];
assert_checkequal(G.Properties.VariableNames, ["OPINION", "GroupCount" "Percent"]);
assert_checkequal(string(G), expected);

G = groupcounts(T, ["OPINION", "UV_INDEX"]);
expected = ["météo correcte" "4" "3"; "météo correcte" "5" "4"; ...
"météo défavorable" "4" "2"; "météo défavorable" "5" "1"; ...
"météo favorable" "4" "1"; "météo favorable" "5" "4"; ; "météo favorable" "6" "5";...
"météo idéale" "5" "1"; "météo idéale" "6" "9"];
assert_checkequal(G.Properties.VariableNames, ["OPINION", "UV_INDEX", "GroupCount"]);
assert_checkequal(string(G), expected);

G = groupcounts(T, ["OPINION" "UV_INDEX"], "IncludePercentGroups", %t, "IncludeEmptyGroups", %t);
expected = ["météo correcte" "4" "3" "10"; "météo correcte" "5" "4" "13.333333"; "météo correcte" "6" "0" "0";...
"météo défavorable" "4" "2" "6.6666667"; "météo défavorable" "5" "1" "3.3333333"; "météo défavorable" "6" "0" "0";...
"météo favorable" "4" "1" "3.3333333"; "météo favorable" "5" "4" "13.333333"; ; "météo favorable" "6" "5" "16.666667";...
"météo idéale" "4" "0" "0"; "météo idéale" "5" "1" "3.3333333"; "météo idéale" "6" "9" "30"];
assert_checkequal(G.Properties.VariableNames, ["OPINION", "UV_INDEX", "GroupCount" "Percent"]);
assert_checkequal(string(G), expected);

TS = table2timeseries(T);
G = groupcounts(T, "OPINION");
expected = ["météo correcte" "7"; "météo défavorable" "3"; "météo favorable" "10"; "météo idéale" "10"];
assert_checkequal(G.Properties.VariableNames, ["OPINION", "GroupCount"]);
assert_checkequal(string(G), expected);

G = groupcounts(T, "OPINION", "IncludePercentGroups", %t);
expected = ["météo correcte" "7" "23.333333"; "météo défavorable" "3" "10"; "météo favorable" "10" "33.333333"; "météo idéale" "10" "33.333333"];
assert_checkequal(G.Properties.VariableNames, ["OPINION", "GroupCount" "Percent"]);
assert_checkequal(string(G), expected);

G = groupcounts(T, ["OPINION", "UV_INDEX"]);
expected = ["météo correcte" "4" "3"; "météo correcte" "5" "4"; ...
"météo défavorable" "4" "2"; "météo défavorable" "5" "1"; ...
"météo favorable" "4" "1"; "météo favorable" "5" "4"; "météo favorable" "6" "5";...
"météo idéale" "5" "1"; "météo idéale" "6" "9"];
assert_checkequal(G.Properties.VariableNames, ["OPINION", "UV_INDEX", "GroupCount"]);
assert_checkequal(string(G), expected);

G = groupcounts(T, ["OPINION" "UV_INDEX"], "IncludePercentGroups", %t, "IncludeEmptyGroups", %t);
expected = ["météo correcte" "4" "3" "10"; "météo correcte" "5" "4" "13.333333"; "météo correcte" "6" "0" "0";...
"météo défavorable" "4" "2" "6.6666667"; "météo défavorable" "5" "1" "3.3333333"; "météo défavorable" "6" "0" "0";...
"météo favorable" "4" "1" "3.3333333"; "météo favorable" "5" "4" "13.333333"; "météo favorable" "6" "5" "16.666667";...
"météo idéale" "4" "0" "0"; "météo idéale" "5" "1" "3.3333333"; "météo idéale" "6" "9" "30"];
assert_checkequal(G.Properties.VariableNames, ["OPINION", "UV_INDEX", "GroupCount" "Percent"]);
assert_checkequal(string(G), expected);

G = groupcounts(TS, "DATE", "dayname");
expected = ["Sunday" "4"; "Monday" "4"; "Tuesday" "4"; "Wednesday" "4"; "Thursday" "5"; "Friday" "5"; "Saturday" "4"];
assert_checkequal(G.Properties.VariableNames, ["dayname_DATE" "GroupCount"]);
assert_checkequal(string(G), expected);


// Third test
// -----------------------------------------------------------------------------
// Data from https://data.oecd.org/agrland/agricultural-land.htm
T = readtable(fullfile(SCI, "modules", "spreadsheet", "tests", "unit_tests","data_agri.csv"));

G = groupcounts(T, "SUBJECT");
expected = ["MAIZE", "4674"; "RICE", "4654"; "SOYBEAN" "4674"; "WHEAT" "4674"];
assert_checkequal(G.Properties.VariableNames, ["SUBJECT", "GroupCount"]);
assert_checkequal(string(G), expected);

G = groupcounts(T, ["LOCATION", "SUBJECT"]);
str = string(123 * ones(152, 1));
str(22) = "103";
expected = [gsort(unique(T.LOCATION)(ones(4,1).*.matrix(1:38, [38 1])), 'g', 'i'), repmat(unique(T.SUBJECT), 38, 1), str];
assert_checkequal(G.Properties.VariableNames, ["LOCATION", "SUBJECT", "GroupCount"]);
assert_checkequal(string(G), expected);

rand("seed", 0)
// with datetime
dt = datetime(2023,[5 3:2:10]', 1);
x = ["a"; "b"; "b"; "c"; "a"];
A = timeseries(dt, x, x1, "VariableNames", ["dt", "x", "x1"]);

G = groupcounts(A, "dt", "monthname");
m = ["March"; "May"; "July"; "September"];
values = [1; 2; 1; 1];
expected = table(m, values, "VariableNames", ["monthname_dt", "GroupCount"]);
assert_checkequal(G, expected);

// With IncludeEmptyGroups
G = groupcounts(A, "dt", "monthname", "IncludeEmptyGroups", %t);
m = ["January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"]';
values = [0 0 1 0 2 0 1 0 1 0 0 0]';
expected = table(m, values, "VariableNames", ["monthname_dt", "GroupCount"]);
assert_checkequal(G, expected);

// groupbins contains datetime or duration
rand("seed", 0);
time = datetime(2024, 4, floor(30*rand(10,1)+1));
x = ["a"; "b"; "a"; "a"; "b"; "c"; "b"; "c"; "c"; "a"];
y = floor(15 * rand(10, 1));
t = timeseries(time, x, y, "VariableNames", ["time", "x", "y"]);

G = groupcounts(t, "time", datetime(2024, 4, 20:2:30));
str = ["[ 2024-04-20, 2024-04-22 )"; "[ 2024-04-22, 2024-04-24 )"; "[ 2024-04-26, 2024-04-28 )"];
expected = table(str, [2; 1; 2], "VariableNames", ["time", "GroupCount"]);
assert_checkequal(G, expected);

G = groupcounts(t, ["time", "y"], {datetime(2024, 4, 20:2:30), [5 7 10 13]});
str = ["[ 2024-04-20, 2024-04-22 )"; "[ 2024-04-20, 2024-04-22 )"; "[ 2024-04-22, 2024-04-24 )"; "[ 2024-04-26, 2024-04-28 )"; "[ 2024-04-26, 2024-04-28 )"; "<undefined>"; "<undefined>"; "<undefined>"];
str2 = ["[7, 10)"; "<undefined>";  "[7, 10)"; "[10, 13]"; "<undefined>"; "[7, 10)"; "[10, 13]"; "<undefined>"];
expected = table(str, str2, [1;1;1;1;1;2;1;2], "VariableNames", ["time", "disc_y", "GroupCount"]);
assert_checkequal(G, expected);

G = groupcounts(t, ["time", "x"], {datetime(2024, 4, 1:10:30), "none"});
str = ["[ 2024-04-01, 2024-04-11 )"; "[ 2024-04-11, 2024-04-21 ]"; "[ 2024-04-11, 2024-04-21 ]"; "<undefined>"; "<undefined>"];
str2 = ["a"; "b"; "c"; "b"; "c"];
expected = table(str, str2, [4; 1; 2; 2; 1], "VariableNames", ["time", "x", "GroupCount"]);
assert_checkequal(G, expected);

G = groupcounts(t, "time", caldays(10));
str = ["[ 2024-04-01, 2024-04-11 )"; "[ 2024-04-11, 2024-04-21 )"; "[ 2024-04-21, 2024-05-01 ]"];
expected = table(str, [4; 2; 4], "VariableNames", ["time", "GroupCount"]);
assert_checkequal(G, expected);

G = groupcounts(t, "time", hours(72));
str = ["[ 2024-04-01, 2024-04-04 )"; "[ 2024-04-07, 2024-04-10 )"; "[ 2024-04-10, 2024-04-13 )"; ...
"[ 2024-04-19, 2024-04-22 )"; "[ 2024-04-22, 2024-04-25 )"; "[ 2024-04-25, 2024-04-28 ]"];
expected = table(str, [2; 1; 1; 3; 1; 2], "VariableNames", ["time", "GroupCount"]);
assert_checkequal(G, expected);
