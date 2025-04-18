/*
 * Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2004-2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Allan Cornet
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2011 - DIGITEO - Vincent Couvert
 * Copyright (C) 2012 - Scilab Enterprises - Bruno JOFRET
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

/*------------------------------------------------------------------------*/
/* file: get_tics_interpreters_property.c                                       */
/* desc : function to retrieve in Scilab the tics_labels field of         */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "getHandleProperty.h"
#include "GetProperty.h"
#include "returnProperty.h"
#include "Scierror.h"
#include "localization.h"
#include "Format.h"
#include "sci_malloc.h"

#include "getGraphicObjectProperty.h"
#include "graphicObjectProperties.h"

/*------------------------------------------------------------------------*/
void* get_tics_interpreters_property(void* _pvCtx, int iObjUID)
{
    char** interpreters = NULL;
    int iNumberTicksLabels = 0;
    int* piNumberTicksLabels = &iNumberTicksLabels;

    getGraphicObjectProperty(iObjUID, __GO_NUMBER_TICKS_LABELS__, jni_int, (void **) &piNumberTicksLabels);
    if (piNumberTicksLabels == NULL)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "tics_interpreters");
        return NULL;
    }

    getGraphicObjectProperty(iObjUID, __GO_TICKS_INTERPRETERS__, jni_string_vector, (void **) &interpreters);

    if (interpreters == NULL)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "tics_interpreters");
        return NULL;
    }

    /* User-specified ticks labels */
    return sciReturnRowStringVector(interpreters, iNumberTicksLabels);
}
/*------------------------------------------------------------------------*/
