// Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2024 - 3DS - Antoine ELIAS

function s = %datetime_outline(dt, verbose)
    if isscalar(dt) then
        s = %type_dims_outline(dt, typeStr=typeof(dt));
    else
        s = %type_dims_outline(dt);
    end
endfunction