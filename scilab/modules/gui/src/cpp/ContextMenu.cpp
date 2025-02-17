/*
 * Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Vincent COUVERT
 * Copyright (C) 2012 - Scilab Enterprises - Vincent COUVERT
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */

#include "ContextMenu.hxx"

using namespace org_scilab_modules_gui_bridge;

char *uiWaitContextMenu(int iObjUID)
{
    /* TO DO : create a copy or move sci_uiwait to cpp to do a delete */
    return CallScilabBridge::displayAndWaitContextMenu(getScilabJavaVM(), iObjUID);
}

void releaseUiWaitContextMenuResult(char* res)
{
    if (res == nullptr)
    {
        return;
    }
    delete[] res;
}
