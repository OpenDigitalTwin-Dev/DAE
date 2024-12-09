// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2022 - 3DS - Adeline CARNIS
// Copyright (C) 2022 - 3DS - Antoine ELIAS
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function out = %datetime_1_datetime(dt1, dt2)
    //check dimensions compatibilities
    if size(dt1, "*") <> 1 && size(dt2, "*") <> 1 && or(size(dt1) <> size(dt2)) then
        error(msprintf(_("%s: Wrong size for input arguments #%d and #%d: Same size expected.\n"), "%datetime_1_datetime", 1, 2))
    end

    date_bool1 = dt1.date > dt2.date; //exclution
    date_bool2 = dt1.date < dt2.date; //equal date
    time_bool = dt1.time < dt2.time; //time compare

    out = ~date_bool1 & (date_bool2 | time_bool);
endfunction
