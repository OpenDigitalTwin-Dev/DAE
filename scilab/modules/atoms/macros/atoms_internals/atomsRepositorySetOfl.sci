// Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Pierre MARECHAL <pierre.marechal@scilab.org>
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

// Internal function

// Add an URL to the list of repositories, and returns

function atomsRepositorySetOfl(url, update)

    // Check number of input arguments
    // =========================================================================
    rhs = argn(2);
    if rhs < 1 || 2 < rhs  then
        error(msprintf(gettext("%s: Wrong number of input argument: %d to %d expected.\n"),"atomsRepositorySetOfl",1,2));
    end

    // Check URLs specified as first input argument
    // =========================================================================

    if type(url) <> 10 then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: string expected.\n"),"atomsRepositorySetOfl",1));
    end

    if size(url,"*") <> 1 then
        error(msprintf(gettext("%s: Wrong size for input argument #%d: string expected.\n"),"atomsRepositorySetOfl",1));
    end

    valid_url_pattern    = "/^((((H|h)(T|t)|(F|f))(T|t)(P|p)((S|s)?))\:\/\/)?(www.|[a-zA-Z0-9].)[a-zA-Z0-9\-\.]+\.[a-zA-Z]{2,6}(\:[0-9]{1,5})*(\/($|[a-zA-Z0-9\.\,\;\?\''\\\+&amp;%\$#\=~_\-]+))*$/";

    if ~ regexp(url,valid_url_pattern,"o") == 1 then
        error(msprintf(gettext("%s: Wrong value for input argument #%d: ''%s'' is not a valid URL.\n"),"atomsRepositorySetOfl",1,url));
    end

    // Check update boolean flag specified as second input argument
    // =========================================================================

    if rhs < 2 then
        update = %t;
    else
        if type(update) <> 4 then
            error(msprintf(gettext("%s: Wrong type for input argument #%d: boolean expected.\n"),"atomsRepositorySetOfl",2));
        end
        if size(update,"*") <> 1 then
            error(msprintf(gettext("%s: Wrong size for input argument #%d: boolean expected.\n"),"atomsRepositorySetOfl",2));
        end
    end

    // Overwrite "repositories" file
    // =========================================================================
    ATOMSALLUSERSWRITEACCESS = atomsAUWriteAccess();
    if(ATOMSALLUSERSWRITEACCESS) then
        mputl(url,pathconvert(SCI+"/modules/atoms/etc/repositories",%F));
    else
        error(msprintf(gettext("%s: Admin rights are needed to change ATOMS official repository.\n"), "atomsRepositorySetOfl"));
    end

    // Update the toolbox list
    // =========================================================================
    if update then
        atomsSystemUpdate();
    end
endfunction
