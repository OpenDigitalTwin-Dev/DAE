/*
 *  Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2011-2011 - DIGITEO - Bruno JOFRET
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

#include "ScilabView.hxx"

extern "C"
{
#include "HandleManagement.h"
#include "graphicObjectProperties.h"
}

long getHandle(int UID)
{
    return ScilabView::getObjectHandle(UID);
}

int getObjectFromHandle(long handle)
{
    return ScilabView::getObjectFromHandle(handle);
}

char const* getHandleTypeStr(int _iType)
{
    switch (_iType)
    {
        case __GO_ARC__:
            return "Arc";
        case __GO_AXES__:
            return "Axes";
        case __GO_AXESMODEL__:
            return "AxesModel";
        case __GO_AXIS__:
            return "Axis";
        case __GO_CHAMP__:
            return "Champ";
        case __GO_COMPOUND__:
            return "Compound";
        case __GO_FAC3D__:
            return "Fac3d";
        case __GO_FEC__:
            return "Fec";
        case __GO_FIGURE__:
            return "Figure";
        case __GO_FIGUREMODEL__:
            return "FigureModel";
        case __GO_GRAYPLOT__:
            return "Grayplot";
        case __GO_LABEL__:
            return "Label";
        case __GO_LEGEND__:
            return "Legend";
        case __GO_MATPLOT__:
            return "Matplot";
        case __GO_PLOT3D__:
            return "Plot3d";
        case __GO_POLYLINE__:
            return "Polyline";
        case __GO_RECTANGLE__:
            return "Rectangle";
        case __GO_SEGS__:
            return "Segs";
        case __GO_TEXT__:
            return "Text";
        case __GO_UICONTROL__:
            return "uicontrol";
        case __GO_UIMENU__:
            return "uimenu";
        case __GO_UICONTEXTMENU__:
            return "uicontextmenu";
        case __GO_CONSOLE__:
            return "Console";
        case __GO_SHOWHIDDENHANDLES__:
            return "ShowHiddenHandles";
        case __GO_WAITBAR__:
            return "Waitbar";
        case __GO_PROGRESSIONBAR__:
            return "Progressionbar";
        case __GO_DATATIP__:
            return "Datatip";
        case __GO_LIGHT__:
            return "Light";
        default:
            return "????";
    }
}
