/*
 *  Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2010-2010 - DIGITEO - Antoine ELIAS
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

#include "core_gw.hxx"
#include "function.hxx"
#include "callable.hxx"
#include "context.hxx"
#include "string.hxx"
#include "bool.hxx"

extern "C"
{
#include "Scierror.h"
#include "localization.h"
}

types::Function::ReturnValue sci_isglobal(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    types::typed_list::iterator inIterator;

    if (in.size() != 1)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d expected.\n"), "isglobal", 1);
        return types::Function::Error;
    }

    if (in[0]->isString() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: string expected.\n"), "isglobal", 1);
        return types::Function::Error;
    }

    types::String* pS = in[0]->getAs<types::String>();
    types::Bool *pOut = new types::Bool(pS->getDims(), pS->getDimsArray());

    for (int i = 0; i < pS->getSize(); i++)
    {
        if (symbol::Context::getInstance()->isGlobalVisible(symbol::Symbol(pS->get(i))))
        {
            pOut->set(i,1);
        }
        else
        {
            pOut->set(i,0);
        }
    }

    out.push_back(pOut);
    return types::Function::OK;
}
