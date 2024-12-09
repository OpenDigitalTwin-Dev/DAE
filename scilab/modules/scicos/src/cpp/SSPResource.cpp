/*
 * Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2023 - Dassault Systemes - Clement DAVID
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 */

#include "SSPResource.hxx"

extern "C" {
#include <libxml/xmlversion.h>
}

namespace org_scilab_modules_scicos
{

SSPResource::SSPResource(ScicosID id) : controller(), root(id), constXcosNames(), xcosNamespaceUri(), xmlnsSSC(), xmlnsSSB(), xmlnsSSD(), xmlnsSSV(), xmlnsSSM(), processed(), annotated(false), unit(), references()
{
    LIBXML_TEST_VERSION;
}

SSPResource::~SSPResource()
{
}

}
