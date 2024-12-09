/*
 *  Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2014-2016 - Scilab Enterprises - Clement DAVID
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

#include <algorithm>
#include <cwchar>
#include <cstdarg>
#include <cstdio>
#include <string>
#include <iostream>
#include <sstream>
#include <string>

#include "scilabWrite.hxx"
extern "C"
{
#include "Scierror.h"
#include "Sciwarning.h"
}

#include "LoggerView.hxx"
#include "Controller.hxx"

namespace org_scilab_modules_scicos
{

static const bool USE_SCILAB_WRITE = true;

LoggerView::LoggerView() :
    View(), m_level(LOG_WARNING)
{
}

LoggerView::~LoggerView()
{
}

static std::wstring levelTable[] =
{
    L"TRACE",
    L"DEBUG",
    L"INFO",
    L"WARNING",
    L"ERROR",
    L"FATAL",
};

static std::string displayTable[] =
{
    "Xcos trace:   ",
    "Xcos debug:   ",
    "Xcos info:    ",
    "Xcos warning: ",
    "Xcos error:   ",
    "Xcos fatal:   ",
};

enum LogLevel LoggerView::indexOf(const wchar_t* name)
{
    for (int i = LOG_TRACE; i <= LOG_FATAL; i++)
    {
        if (!wcscmp(name, levelTable[i].data()))
        {
            return static_cast<enum LogLevel>(i);
        }
    }
    return LOG_UNDEF;
}

const wchar_t* LoggerView::toString(enum LogLevel level)
{
    if (LOG_TRACE <= level && level <= LOG_FATAL)
    {
        return levelTable[level].data();
    }
    return L"";
}

const std::string LoggerView::toDisplay(enum LogLevel level)
{
    if (LOG_TRACE <= level && level <= LOG_FATAL)
    {
        return displayTable[level];
    }
    return "";
}

void LoggerView::log(enum LogLevel level, const std::stringstream& msg)
{
    if (level >= this->m_level)
    {
        std::string str = msg.str();
        if (USE_SCILAB_WRITE)
        {
            scilabForcedWrite((LoggerView::toDisplay(level) + str).data());
        }
        else
        {
            std::cerr << LoggerView::toDisplay(level) << str;
        }
    }
}

void LoggerView::log(enum LogLevel level, const std::string& msg)
{
    if (level >= this->m_level)
    {
        if (USE_SCILAB_WRITE)
        {
            scilabForcedWrite((LoggerView::toDisplay(level) + msg).data());
        }
        else
        {
            std::cerr << LoggerView::toDisplay(level) << msg;
        }
    }
}

void LoggerView::log(enum LogLevel level, const char* msg, ...)
{
    if (level >= this->m_level)
    {
        const int N = 1024;
        std::vector<char> buffer(N);
        char* str = buffer.data();
        
        va_list opts;
        va_start(opts, msg);
        vsnprintf(str, N, msg, opts);
        va_end(opts);

        if (USE_SCILAB_WRITE)
        {
            std::string msg = LoggerView::toDisplay(level) + str;
            if (level == LOG_WARNING)
            {
                // map to a Scilab warning
                Sciwarning(msg.data());
            }
            else if (level >= LOG_ERROR)
            {
                // map to a Scilab error
                Scierror(-1, msg.data());
            }
            else
            {
                // report to the console
                scilabForcedWrite(msg.data());
            }

        }
        else
        {
            std::cerr << LoggerView::toDisplay(level) << str;
        }
    }
}

void LoggerView::log(enum LogLevel level, const wchar_t* msg, ...)
{
    if (level >= this->m_level)
    {
        const int N = 1024;
        std::vector<wchar_t> buffer(N);
        wchar_t* str = buffer.data();

        va_list opts;
        va_start(opts, msg);
        vswprintf(str, N, msg, opts);
        va_end(opts);

        if (USE_SCILAB_WRITE)
        {
            scilabForcedWrite(LoggerView::toDisplay(level).data());
            scilabForcedWriteW(str);
        }
        else
        {
            std::cerr << LoggerView::toDisplay(level);
            std::wcerr << str;
        }

        delete[] str;
    }
}

// generated with :
// awk ' $2 == "//!<" {sub(",","", $1); print "case " $1 ":\n    os << \"" $1 "\";\n    break;" }' ~/work/branches/master/scilab/modules/scicos/includes/utilities.hxx

std::ostream& operator<<(std::ostream& os, update_status_t u)
{
    switch (u)
    {
        case SUCCESS:
            os << "SUCCESS";
            break;
        case NO_CHANGES:
            os << "NO_CHANGES";
            break;
        case FAIL:
            os << "FAIL";
            break;
    }
    return os;
}

std::ostream& operator<<(std::ostream& os, kind_t k)
{
    switch (k)
    {
        case ANNOTATION:
            os << "ANNOTATION";
            break;
        case BLOCK:
            os << "BLOCK";
            break;
        case DIAGRAM:
            os << "DIAGRAM";
            break;
        case LINK:
            os << "LINK";
            break;
        case PORT:
            os << "PORT";
            break;
    }
    return os;
}


std::ostream& operator<<(std::ostream& os, object_properties_t p)
{
    switch (p)
    {
        case PARENT_DIAGRAM:
            os << "PARENT_DIAGRAM";
            break;
        case GEOMETRY:
            os << "GEOMETRY";
            break;
        case DESCRIPTION:
            os << "DESCRIPTION";
            break;
        case FONT:
            os << "FONT";
            break;
        case FONT_SIZE:
            os << "FONT_SIZE";
            break;
        case RELATED_TO:
            os << "RELATED_TO";
            break;
        case INTERFACE_FUNCTION:
            os << "INTERFACE_FUNCTION";
            break;
        case SIM_FUNCTION_NAME:
            os << "SIM_FUNCTION_NAME";
            break;
        case SIM_FUNCTION_API:
            os << "SIM_FUNCTION_API";
            break;
        case SIM_SCHEDULE:
            os << "SIM_SCHEDULE";
            break;
        case SIM_BLOCKTYPE:
            os << "SIM_BLOCKTYPE";
            break;
        case SIM_DEP_UT:
            os << "SIM_DEP_UT";
            break;
        case EXPRS:
            os << "EXPRS";
            break;
        case INPUTS:
            os << "INPUTS";
            break;
        case OUTPUTS:
            os << "OUTPUTS";
            break;
        case EVENT_INPUTS:
            os << "EVENT_INPUTS";
            break;
        case EVENT_OUTPUTS:
            os << "EVENT_OUTPUTS";
            break;
        case STATE:
            os << "STATE";
            break;
        case DSTATE:
            os << "DSTATE";
            break;
        case ODSTATE:
            os << "ODSTATE";
            break;
        case NZCROSS:
            os << "NZCROSS";
            break;
        case NMODE:
            os << "NMODE";
            break;
        case PARAMETER_NAME:
            os << "PARAMETER_NAME";
            break;
        case PARAMETER_DESCRIPTION:
            os << "PARAMETER_DESCRIPTION";
            break;
        case PARAMETER_UNIT:
            os << "PARAMETER_UNIT";
            break;
        case PARAMETER_TYPE:
            os << "PARAMETER_TYPE";
            break;
        case PARAMETER_ENCODING:
            os << "PARAMETER_ENCODING";
            break;
        case PARAMETER_VALUE:
            os << "PARAMETER_VALUE";
            break;
        case RPAR:
            os << "RPAR";
            break;
        case IPAR:
            os << "IPAR";
            break;
        case OPAR:
            os << "OPAR";
            break;
        case EQUATIONS:
            os << "EQUATIONS";
            break;
        case UID:
            os << "UID";
            break;
        case PARENT_BLOCK:
            os << "PARENT_BLOCK";
            break;
        case CHILDREN:
            os << "CHILDREN";
            break;
        case PORT_REFERENCE:
            os << "PORT_REFERENCE";
            break;
        case STYLE:
            os << "STYLE";
            break;
        case LABEL:
            os << "LABEL";
            break;
        case DESTINATION_PORT:
            os << "DESTINATION_PORT";
            break;
        case SOURCE_PORT:
            os << "SOURCE_PORT";
            break;
        case CONTROL_POINTS:
            os << "CONTROL_POINTS";
            break;
        case THICK:
            os << "THICK";
            break;
        case COLOR:
            os << "COLOR";
            break;
        case KIND:
            os << "KIND";
            break;
        case DATATYPE:
            os << "DATATYPE";
            break;
        case DATATYPE_ROWS:
            os << "DATATYPE_ROWS";
            break;
        case DATATYPE_COLS:
            os << "DATATYPE_COLS";
            break;
        case DATATYPE_TYPE:
            os << "DATATYPE_TYPE";
            break;
        case FIRING:
            os << "FIRING";
            break;
        case SOURCE_BLOCK:
            os << "SOURCE_BLOCK";
            break;
        case PORT_KIND:
            os << "PORT_KIND";
            break;
        case IMPLICIT:
            os << "IMPLICIT";
            break;
        case PORT_NUMBER:
            os << "PORT_NUMBER";
            break;
        case CONNECTED_SIGNALS:
            os << "CONNECTED_SIGNALS";
            break;
        case NAME:
            os << "NAME";
            break;
        case PATH:
            os << "PATH";
            break;
        case AUTHOR:
            os << "AUTHOR";
            break;
        case FILE_VERSION:
            os << "FILE_VERSION";
            break;
        case COPYRIGHT:
            os << "COPYRIGHT";
            break;
        case LICENSE:
            os << "LICENSE";
            break;
        case GENERATION_TOOL:
            os << "GENERATION_TOOL";
            break;
        case GENERATION_DATE:
            os << "GENERATION_DATE";
            break;
        case PROPERTIES:
            os << "PROPERTIES";
            break;
        case DEBUG_LEVEL:
            os << "DEBUG_LEVEL";
            break;
        case DIAGRAM_CONTEXT:
            os << "CONTEXT";
            break;
        case VERSION_NUMBER:
            os << "VERSION_NUMBER";
            break;
        default:
            os << "";
            break;
    }
    return os;
}

void LoggerView::objectCreated(const ScicosID& uid, kind_t k)
{
    std::stringstream ss;
    ss << "objectCreated" << "( " << uid << " , " << k << " )" << '\n';
    log(LOG_INFO, ss);
}

void LoggerView::objectReferenced(const ScicosID& uid, kind_t k, unsigned refCount)
{
    std::stringstream ss;
    ss << "objectReferenced" << "( " << uid << " , " << k << " ) : " << refCount << '\n';
    log(LOG_TRACE, ss);
}

void LoggerView::objectUnreferenced(const ScicosID& uid, kind_t k, unsigned refCount)
{
    std::stringstream ss;
    ss << "objectUnreferenced" << "( " << uid << " , " << k << " ) : " << refCount << '\n';
    log(LOG_TRACE, ss);
}

void LoggerView::objectDeleted(const ScicosID& uid, kind_t k)
{
    std::stringstream ss;
    ss << "objectDeleted" << "( " << uid << " , " << k << " )" << '\n';
    log(LOG_INFO, ss);
}

void LoggerView::objectCloned(const ScicosID& uid, const ScicosID& cloned, kind_t k)
{
    std::stringstream ss;
    ss << "objectCloned" << "( " << uid << " , " << cloned << " , " << k << " )" << '\n';
    log(LOG_INFO, ss);
}

//
// used to debug a link connection, will be compiled out
//

/* 0-based index of id in content, -1 if not found */
inline int indexOf(ScicosID id, const std::vector<ScicosID>& content)
{
    const auto& it = std::find(content.begin(), content.end(), id);
    if (it == content.end())
        return -1;
    return (int) std::distance(content.begin(), it);
};

/* Scilab-like connected port  */
static inline std::string to_string_port(Controller& controller, ScicosID uid, kind_t k, object_properties_t p)
{
    if (k != LINK)
        return "";
    if (p != SOURCE_PORT && p != DESTINATION_PORT)
        return "";

    ScicosID endID = ScicosID();
    controller.getObjectProperty(uid, k, p, endID);
    if (endID == ScicosID())
    {
        return "";
    }

    ScicosID sourceBlock = ScicosID();
    controller.getObjectProperty(endID, PORT, SOURCE_BLOCK, sourceBlock);
    if (sourceBlock == ScicosID())
    {
        return "";
    }
    
    ScicosID parent = ScicosID();
    kind_t parentKind = BLOCK;
    controller.getObjectProperty(uid, k, PARENT_BLOCK, parent);
    std::vector<ScicosID> children;
    // Added to a superblock
    if (parent == ScicosID())
    {
        // Added to a diagram
        controller.getObjectProperty(uid, k, PARENT_DIAGRAM, parent);
        parentKind = DIAGRAM;
    }
    if (parent == ScicosID())
    {
        return "";
    }
    controller.getObjectProperty(parent, parentKind, CHILDREN, children);

    std::vector<ScicosID> sourceBlockPorts;
    int portIndex;
    object_properties_t port;
    for (object_properties_t ports : { INPUTS, OUTPUTS, EVENT_INPUTS, EVENT_OUTPUTS })
    {
        controller.getObjectProperty(sourceBlock, BLOCK, ports, sourceBlockPorts);
        portIndex = indexOf(endID, sourceBlockPorts) + 1;
        port = ports;
        if (portIndex > 0)
            break;
    }

    int startOrEnd = 0;
    if (port == INPUTS && p == SOURCE_PORT)
        startOrEnd = 1;
    else if (port == EVENT_INPUTS && p == SOURCE_PORT)
        startOrEnd = 1;
    else if (port == OUTPUTS && p == SOURCE_PORT)
        startOrEnd = 0;
    else if (port == EVENT_OUTPUTS && p == SOURCE_PORT)
        startOrEnd = 0;
    else if (port == INPUTS && p == DESTINATION_PORT)
        startOrEnd = 1;
    else if (port == EVENT_INPUTS && p == DESTINATION_PORT)
        startOrEnd = 1;
    else if (port == OUTPUTS && p == DESTINATION_PORT)
        startOrEnd = 0;
    else if (port == EVENT_OUTPUTS && p == DESTINATION_PORT)
        startOrEnd = 0;

    return "[" + std::to_string(indexOf(sourceBlock, children)+1) + " " + std::to_string(portIndex) + " " + std::to_string(startOrEnd) + "]";
};

/* Scilab-like connected link  */
static inline std::string to_string_link(Controller& controller, ScicosID uid, kind_t k)
{
    if (k != LINK)
        return "";

    std::vector<ScicosID> path;
    path.push_back(ScicosID());
    ScicosID last = path.back();
    controller.getObjectProperty(uid, k, PARENT_BLOCK, path.back());
    while (path.back() != ScicosID())
    {
        last = path.back();
        path.push_back(ScicosID());
        controller.getObjectProperty(last, BLOCK, PARENT_BLOCK, path.back());
    }
    path.pop_back();

    ScicosID diagram = ScicosID();
    controller.getObjectProperty(last, BLOCK, PARENT_DIAGRAM, diagram);

    // display path
    std::vector<ScicosID> children;
    controller.getObjectProperty(diagram, DIAGRAM, CHILDREN, children);
    
    std::stringstream ss;
    ss << "LoggerView " << uid;
    ss << " scs_m.objs(";
    for (auto it = path.rbegin(); it != path.rend(); ++it)
    {
        ss << indexOf(*it, children) + 1 << ").model.rpar.objs(";
        controller.getObjectProperty(*it, BLOCK, CHILDREN, children);
    }
    ss << indexOf(uid, children) + 1 << ") ";
    
    // display connection
    ss << " = scicos_link(";
    ss << "from=" << to_string_port(controller, uid, k, SOURCE_PORT);
    ss << ",";
    ss << "to=" << to_string_port(controller, uid, k, DESTINATION_PORT);
    ss << ")";

    return ss.str();
};


void LoggerView::propertyUpdated(const ScicosID& uid, kind_t k, object_properties_t p, update_status_t u)
{
    std::stringstream ss;
    ss << "propertyUpdated" << "( " << uid << " , " << k << " , " << p << " ) : " << u;

    // DEBUG Controller controller;
    // DEBUG
    // DEBUG if (p == CHILDREN || p == INPUTS || p == OUTPUTS || p == EVENT_INPUTS || p == EVENT_OUTPUTS || p == CONNECTED_SIGNALS)
    // DEBUG {
    // DEBUG     std::vector<ScicosID> end;
    // DEBUG     controller.getObjectProperty(uid, k, p, end);
    // DEBUG     if (end.size() > 0)
    // DEBUG     {
    // DEBUG         ss << " [ " << end[0];
    // DEBUG         for (auto it = end.begin() + 1; it != end.end(); ++it)
    // DEBUG             ss << " , " << *it;
// DEBUG 
    // DEBUG         ss << " ]";
    // DEBUG     }
    // DEBUG     else
    // DEBUG     {
    // DEBUG         ss << " []";
    // DEBUG     }
    // DEBUG }
// DEBUG 
    // DEBUG if (p == SOURCE_PORT || p == DESTINATION_PORT || p == SOURCE_BLOCK)
    // DEBUG {
    // DEBUG     ScicosID end;
    // DEBUG     controller.getObjectProperty(uid, k, p, end);
    // DEBUG     ss << " " << end;
    // DEBUG }

    ss << '\n';
    

    if (u == NO_CHANGES)
    {
        log(LOG_TRACE, ss);
    }
    else
    {
        log(LOG_DEBUG, ss);
    }
}

} /* namespace org_scilab_modules_scicos */
