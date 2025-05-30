// Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 1999 - INRIA - Carlos Klimann
// Copyright (C) 2010, 2016 - Samuel GOUGEON
// Copyright (C) 2016 - Michael Baudin
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.
//

function gm = geomean(x, orien)

    arguments
        x {mustBeA(x, ["double", "sparse"])}
        orien (1, 1) {mustBeA(orien, ["double", "string"])} = "*"
    end

    if type(orien)==1
        orien = int(orien)
    end
    if ~((type(orien)==10 && or(convstr(orien)==["r" "c" "*"])) | ..
        (type(orien)==1 && orien>0 && orien<=ndims(x)) )
        msg = _("%s: Argument #%d: Must be in the set {%s}.\n")
        tmp = msprintf("""r"",""c"",1..%d", ndims(x))
        error(msprintf(msg, "geomean", 2, tmp))
    end

    sp = typeof(x)=="sparse"
    if sp then
        x = full(x)
    end

    if (x == []) then
        gm = %nan
        return
    end

    mod = ieee()
    ieee(2)     // To avoid procedural warning or error on log(0)
    if orien == "*" then
        gm = exp(mean(log(x)))
    else
        gm = exp(mean(log(x),orien))
        if sp then
            gm = sparse(gm)
        end
    end
    ieee(mod)   // Restoring initial mode
endfunction
