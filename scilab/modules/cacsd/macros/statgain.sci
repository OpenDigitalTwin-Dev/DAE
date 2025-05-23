// Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA -
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function st = statgain(sl)

    arguments
        sl {mustBeA(sl, ["r", "lss"])}
    end

    select typeof(sl)
    case "rational" then
        dom=sl("dt")
        if dom=="c" then
            st=freq(sl("num"),sl("den"),0)
        else
            st=freq(sl("num"),sl("den"),1)
        end;
    case "state-space" then
        dom=sl.dt;
        [m,p]=size(sl(2));
        if dom=="c" then
            if rank(sl(2)) <> m then
                msg = gettext("%s: Wrong values for input argument #%d: State matrix is singular.\n");
                error(msprintf(msg, "statgain", 1));
            end
            st=sl(5)-sl(4)*inv(sl(2))*sl(3);
        else
            if rank(eye(m,m)-sl(2))<>m then
                msg = gettext("%s: Wrong values for input argument #%d: State matrix - eye is singular.\n");
                error(msprintf(msg, "statgain", 1));
            end
            st = sl(5) + sl(4)*inv(eye(m,m) - sl(2))*sl(3);
        end
    end

endfunction
