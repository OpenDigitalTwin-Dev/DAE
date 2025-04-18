/*
* Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2011 - DIGITEO - Cedric DELAMARRE
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
/*--------------------------------------------------------------------------*/
#include "optimizationfunctions.hxx"

extern "C"
{
#include "scioptimfunctions.h"
}

// optim
void costf(int *ind, int *n, double *x, double *f, double *g, int *ti, float *tr, double *td)
{
    OptimizationFunctions* opFunction = NULL;
    opFunction = Optimization::getOptimizationFunctions();

    try
    {
        if (opFunction == NULL)
        {
            throw ast::InternalError(_("An error occurred while getting OptimizationFunctions object.\n"));
        }
        opFunction->execCostf(ind, n, x, f, g, ti, tr, td);
    }
    catch (const ast::InternalError)
    {
        *ind=-1;
    }
}

// fsolve
void fct(int *n, double *x, double *v, int *iflag)
{
    OptimizationFunctions* opFunction = NULL;
    opFunction = Optimization::getOptimizationFunctions();

    try
    {
        if (opFunction == NULL)
        {
            throw ast::InternalError(_("An error occurred while getting OptimizationFunctions object.\n"));
        }
        opFunction->execFsolveFct(n, x, v, iflag);
    }
    catch (const ast::InternalError&)
    {
        *iflag=-1;
    }
}

void jac(int *n, double *x, double *v, double *jac, int *ldjac, int *iflag)
{
    OptimizationFunctions* opFunction = NULL;
    opFunction = Optimization::getOptimizationFunctions();

    try
    {
        if (opFunction == NULL)
        {
            throw ast::InternalError(_("An error occurred while getting OptimizationFunctions object.\n"));
        }
        if (*iflag == 1)
        {
            opFunction->execFsolveFct(n, x, v, iflag);
        }
        else
        {
            opFunction->execFsolveJac(n, x, v, jac, ldjac, iflag);
        }        
    }
    catch (const ast::InternalError&)
    {
        *iflag=-1;
    }        
}

// lsqrsolve
void lsqrfct(int *m, int *n, double *x, double *v, int *iflag)
{
    OptimizationFunctions* opFunction = NULL;
    opFunction = Optimization::getOptimizationFunctions();

    try
    {
        if (opFunction == NULL)
        {
            throw ast::InternalError(_("An error occurred while getting OptimizationFunctions object.\n"));
        }
        opFunction->execLsqrsolveFct(m, n, x, v, iflag);
    }
    catch (const ast::InternalError)
    {
        *iflag=-1;
    }
}

void lsqrjac(int *m, int *n, double *x, double *v, double *jac, int *ldjac, int *iflag)
{
    OptimizationFunctions* opFunction = NULL;
    opFunction = Optimization::getOptimizationFunctions();

    try
    {
        if (opFunction == NULL)
        {
            throw ast::InternalError(_("An error occurred while getting OptimizationFunctions object.\n"));
        }
        if (*iflag == 1)
        {
            opFunction->execLsqrsolveFct(m, n, x, v, iflag);
        }
        else
        {
            opFunction->execLsqrsolveJac(m, n, x, v, jac, ldjac, iflag);
        }        
    }
    catch (const ast::InternalError)
    {
        *iflag=-1;
    }        
}
