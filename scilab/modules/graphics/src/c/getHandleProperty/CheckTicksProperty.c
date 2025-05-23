/*
 * Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 * Copyright (C) 2024 - UTC - Stéphane MOTTELET
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
/* file: CheckTicksProperty.h                                             */
/* desc : generic function to retrieve the assigned x_ticks, y_ticks or   */
/*        z_ticks property                                                */
/*------------------------------------------------------------------------*/

#include "CheckTicksProperty.h"
#include "localization.h"
#include "Scierror.h"
/*---------------------------------------------------------------------------*/

AssignedList * createTlistForTicks(void* _pvCtx)
{
    AssignedList* tlist = NULL;
    int nbRowLoc  = 0;
    int nbColLoc  = 0;
    int nbRowLab  = 0;
    int nbColLab  = 0;
    int nbRowInt  = 0;
    int nbColInt  = 0;
    char** ret;

    tlist = createAssignedList(_pvCtx, 3, 3);

    if (tlist == NULL)
    {
        // test legacy ticks (no interpreter field)
        tlist = createAssignedList(_pvCtx, 3, 2);
        if (tlist == NULL)
        {
            Scierror(999, _("Tlist could not be created, check ticks location and label vectors.\n"));
            return NULL;            
        }
    }

    if (!isListCurrentElementDoubleMatrix(_pvCtx, tlist))
    {
        Scierror(999, _("%s should be a vector of double.\n"), "locations");
        destroyAssignedList(tlist);
        return NULL;
    }

    getCurrentDoubleMatrixFromList(_pvCtx, tlist, &nbRowLoc, &nbColLoc);

    if (nbRowLoc * nbColLoc == 0)
    {
        /* labels should also be an empty matrix */
        if (!isListCurrentElementEmptyMatrix(_pvCtx, tlist))
        {
            Scierror(999, _("Ticks location and label vectors must have the same size.\n"));
            destroyAssignedList(tlist);
            return NULL;
        }
    }
    else
    {
        if (!isListCurrentElementStringMatrix(_pvCtx, tlist))
        {
            Scierror(999, _("%s should be a string vector.\n"), "labels");
            destroyAssignedList(tlist);
            return NULL;
        }

        ret = getCurrentStringMatrixFromList(_pvCtx, tlist, &nbRowLab, &nbColLab);
        if (ret != NULL)
        {
            freeAllocatedMatrixOfString(nbRowLab, nbColLab, ret);
        }

        if (nbRowLoc != nbRowLab || nbColLoc != nbColLab)
        {
            Scierror(999, _("Ticks location and label vectors must have the same size.\n"));
            destroyAssignedList(tlist);
            return NULL;
        }

        if (tlist->iNbItem == 4)
        {
            if (!isListCurrentElementStringMatrix(_pvCtx, tlist))
            {
                Scierror(999, _("%s should be a string vector.\n"), "interpreter");
                destroyAssignedList(tlist);
                return NULL;
            }

            ret = getCurrentStringMatrixFromList(_pvCtx, tlist, &nbRowInt, &nbColInt);
            if (ret != NULL)
            {
                freeAllocatedMatrixOfString(nbRowInt, nbColInt, ret);
            }

            if (nbRowLoc != nbRowInt || nbColLoc != nbColInt)
            {
                Scierror(999, _("Ticks location and interpreter vectors must have the same size.\n"));
                destroyAssignedList(tlist);
                return NULL;
            }
        }
    }

    rewindAssignedList(tlist);

    return tlist;

}
/*---------------------------------------------------------------------------*/
