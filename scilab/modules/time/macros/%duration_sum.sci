// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2024 - 3DS - Adeline CARNIS
//
// For more information, see the COPYING file which you should have received
// along with this program.

function out = %duration_sum(d, orient, typ)
    arguments
        d {mustBeA(d, "duration")}
        orient (1, 1) {mustBeA(orient, ["double", "string"]), mustBeMember(orient, {1, 2, "r", "c", "*"})} = "*"
        typ (1, 1) {mustBeA(typ, "string"), mustBeMember(typ, ["native", "double"])} = "native"
    end

    s = sum(d.duration, orient, typ);
    out = mlist(["duration", "duration", "format"], s, d.format);
endfunction
