/*
 *  Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2015 - Scilab Enterprises - Calixte DENIZET
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

#ifndef __SLINT_XML_CONFIG_HXX__
#define __SLINT_XML_CONFIG_HXX__

#include "XMLtools.hxx"
#include "checkers/AllCheckers.hxx"
#include "SLintXMLException.hxx"

extern "C"
{
#include "dynlib_slint.h"
}

namespace types
{
class String;
}

namespace slint
{

class SLintOptions;

class SLINT_IMPEXP XMLConfig
{

    typedef SLintChecker * (*CBType)(xmlNode *);

    static std::unordered_map<std::string, CBType> callbacks;

public:

    static void getOptions(const std::wstring & path, SLintOptions & options);
    static void getOptions(types::String & str, SLintOptions & options);

private:

    template<typename T>
    static SLintChecker * createFromXmlNode(xmlNode * node)
    {
        bool enable = true;
        XMLtools::getBool(node, "enable", enable);
        if (enable)
        {
            std::wstring id;
            XMLtools::getWString(node, "id", id);

            return new T(id);
        }

        return nullptr;
    }

    static std::unordered_map<std::string, CBType> initCallbacks();

};

} // namespace slint

#endif // __SLINT_XML_CONFIG_HXX__
