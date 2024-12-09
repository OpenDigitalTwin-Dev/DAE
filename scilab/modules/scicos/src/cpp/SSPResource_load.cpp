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

#include <algorithm>
#include <cassert>
#include <cmath>   // for fabs
#include <cstdlib> // for atoi and atof
#include <cstring> // for strcmp and strchr
#include <string>
#include <vector>

#include <filesystem>

#include "SSPResource.hxx"
#include "base64.hxx"
#include "utilities.hxx"
#include "expandPathVariable.h"

// units are stored in the model without being mapped on the Controller yet
#include "Model.hxx"
#include "model/Block.hxx"
#include "model/Diagram.hxx"

extern "C"
{
#include <archive.h>
#include <archive_entry.h>

#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/xmlerror.h>
#include <libxml/xmlreader.h>

#include "Sciwarning.h"
#include "sci_malloc.h"
#include "sci_tmpdir.h"
#include "sci_types.h"
#include "sciprint.h"
}

#ifdef _MSC_VER
#undef min // to remove #define from windows header
#endif

namespace org_scilab_modules_scicos
{

/**
 * Display on the Scilab console
 */
static void console_print(void*, const char* msg, ...) LIBXML_ATTR_FORMAT(2, 3);
void console_print(void*, const char* msg, ...)
{
    // print the message
    va_list ap;
    va_start(ap, msg);
    scivprint(msg, ap);
    va_end(ap);
}

namespace
{

/**
 * Helper class to set / reset the XML parser state
 */
struct LibXML2State
{
    LibXML2State()
    {
        xmlGenericErrorFunc f = &console_print;
        initGenericErrorDefaultFunc(&f);
    }
    ~LibXML2State()
    {
        initGenericErrorDefaultFunc(nullptr);
    }
};

/**
 * Helper class to allocate and clean the archive and libxml2 state
 */
struct State
{
    struct archive* ar;
    struct archive* ext;

    struct archive_entry* ac;

    State() : ar(archive_read_new()), ext(archive_write_disk_new()), ac(archive_entry_new())
    { };

    ~State()
    {
        archive_entry_free(ac);
        archive_write_free(ext);
        archive_read_free(ar);
    };

    // input zip file
    struct archive* input() { return ar; };
    // output temp directory
    struct archive* directory() { return ext; };
    // current entry
    struct archive_entry* entry() { return ac; };
};

/**
 * Copy data from ar to aw
 */
int copy_data(struct archive* ar, struct archive* aw)
{
    la_ssize_t r;
    const void* buff;
    size_t size;
    la_int64_t offset;

    for (;;)
    {
        r = archive_read_data_block(ar, &buff, &size, &offset);
        if (r == ARCHIVE_EOF)
            return (ARCHIVE_OK);
        if (r < ARCHIVE_OK)
            return ((int)r);
        r = archive_write_data_block(aw, buff, size, offset);
        if (r < ARCHIVE_OK)
        {
            sciprint("%s\n", archive_error_string(aw));
            return int(r);
        }
    }
};

int ioread(void* context, char* buffer, int len)
{
    struct archive* a = static_cast<struct archive*>(context);
    return (int)archive_read_data(a, buffer, len);
};

int ioclose(void* context)
{
    return 0;
};

}; // namespace

/*
 * Intern strings to speedup comparaison, this table can be generated from the SSP XSD file.
 */
void SSPResource::internPredefinedStrings(xmlTextReaderPtr reader)
{
    constXcosNames[e_A] = xmlTextReaderConstString(reader, BAD_CAST("A"));
    constXcosNames[e_Annotation] = xmlTextReaderConstString(reader, BAD_CAST("Annotation"));
    constXcosNames[e_Annotations] = xmlTextReaderConstString(reader, BAD_CAST("Annotations"));
    constXcosNames[e_acausal] = xmlTextReaderConstString(reader, BAD_CAST("acausal"));
    constXcosNames[e_any] = xmlTextReaderConstString(reader, BAD_CAST("any"));
    constXcosNames[e_application_x_fmu_sharedlibrary] = xmlTextReaderConstString(reader, BAD_CAST("application/x-fmu-sharedlibrary"));
    constXcosNames[e_application_x_scilab_xcos] = xmlTextReaderConstString(reader, BAD_CAST("application/x-scilab-xcos"));
    constXcosNames[e_application_x_ssp_definition] = xmlTextReaderConstString(reader, BAD_CAST("application/x-ssp-definition"));
    constXcosNames[e_application_x_ssp_package] = xmlTextReaderConstString(reader, BAD_CAST("application/x-ssp-package"));
    constXcosNames[e_text_x_modelica] = xmlTextReaderConstString(reader, BAD_CAST("text/x-modelica"));
    constXcosNames[e_author] = xmlTextReaderConstString(reader, BAD_CAST("author"));
    constXcosNames[e_BaseUnit] = xmlTextReaderConstString(reader, BAD_CAST("BaseUnit"));
    constXcosNames[e_Binary] = xmlTextReaderConstString(reader, BAD_CAST("Binary"));
    constXcosNames[e_Boolean] = xmlTextReaderConstString(reader, BAD_CAST("Boolean"));
    constXcosNames[e_BooleanMappingTransformation] = xmlTextReaderConstString(reader, BAD_CAST("BooleanMappingTransformation"));
    constXcosNames[e_calculatedParameter] = xmlTextReaderConstString(reader, BAD_CAST("calculatedParameter"));
    constXcosNames[e_cd] = xmlTextReaderConstString(reader, BAD_CAST("cd"));
    constXcosNames[e_component] = xmlTextReaderConstString(reader, BAD_CAST("component"));
    constXcosNames[e_Component] = xmlTextReaderConstString(reader, BAD_CAST("Component"));
    constXcosNames[e_Connection] = xmlTextReaderConstString(reader, BAD_CAST("Connection"));
    constXcosNames[e_ConnectionGeometry] = xmlTextReaderConstString(reader, BAD_CAST("ConnectionGeometry"));
    constXcosNames[e_Connections] = xmlTextReaderConstString(reader, BAD_CAST("Connections"));
    constXcosNames[e_Connector] = xmlTextReaderConstString(reader, BAD_CAST("Connector"));
    constXcosNames[e_ConnectorGeometry] = xmlTextReaderConstString(reader, BAD_CAST("ConnectorGeometry"));
    constXcosNames[e_Connectors] = xmlTextReaderConstString(reader, BAD_CAST("Connectors"));
    constXcosNames[e_copyright] = xmlTextReaderConstString(reader, BAD_CAST("copyright"));
    constXcosNames[e_CoSimulation] = xmlTextReaderConstString(reader, BAD_CAST("CoSimulation"));
    constXcosNames[e_DefaultExperiment] = xmlTextReaderConstString(reader, BAD_CAST("DefaultExperiment"));
    constXcosNames[e_description] = xmlTextReaderConstString(reader, BAD_CAST("description"));
    constXcosNames[e_dictionary] = xmlTextReaderConstString(reader, BAD_CAST("dictionary"));
    constXcosNames[e_DictionaryEntry] = xmlTextReaderConstString(reader, BAD_CAST("DictionaryEntry"));
    constXcosNames[e_ElementGeometry] = xmlTextReaderConstString(reader, BAD_CAST("ElementGeometry"));
    constXcosNames[e_Elements] = xmlTextReaderConstString(reader, BAD_CAST("Elements"));
    constXcosNames[e_endConnector] = xmlTextReaderConstString(reader, BAD_CAST("endConnector"));
    constXcosNames[e_endElement] = xmlTextReaderConstString(reader, BAD_CAST("endElement"));
    constXcosNames[e_Enumeration] = xmlTextReaderConstString(reader, BAD_CAST("Enumeration"));
    constXcosNames[e_EnumerationMappingTransformation] = xmlTextReaderConstString(reader, BAD_CAST("EnumerationMappingTransformation"));
    constXcosNames[e_Enumerations] = xmlTextReaderConstString(reader, BAD_CAST("Enumerations"));
    constXcosNames[e_factor] = xmlTextReaderConstString(reader, BAD_CAST("factor"));
    constXcosNames[e_fileversion] = xmlTextReaderConstString(reader, BAD_CAST("fileversion"));
    constXcosNames[e_generationDateAndTime] = xmlTextReaderConstString(reader, BAD_CAST("generationDateAndTime"));
    constXcosNames[e_generationTool] = xmlTextReaderConstString(reader, BAD_CAST("generationTool"));
    constXcosNames[e_GraphicalElements] = xmlTextReaderConstString(reader, BAD_CAST("GraphicalElements"));
    constXcosNames[e_iconFixedAspectRatio] = xmlTextReaderConstString(reader, BAD_CAST("iconFixedAspectRatio"));
    constXcosNames[e_iconFlip] = xmlTextReaderConstString(reader, BAD_CAST("iconFlip"));
    constXcosNames[e_iconRotation] = xmlTextReaderConstString(reader, BAD_CAST("iconRotation"));
    constXcosNames[e_iconSource] = xmlTextReaderConstString(reader, BAD_CAST("iconSource"));
    constXcosNames[e_id] = xmlTextReaderConstString(reader, BAD_CAST("id"));
    constXcosNames[e_implementation] = xmlTextReaderConstString(reader, BAD_CAST("implementation"));
    constXcosNames[e_inout] = xmlTextReaderConstString(reader, BAD_CAST("inout"));
    constXcosNames[e_input] = xmlTextReaderConstString(reader, BAD_CAST("input"));
    constXcosNames[e_Integer] = xmlTextReaderConstString(reader, BAD_CAST("Integer"));
    constXcosNames[e_IntegerMappingTransformation] = xmlTextReaderConstString(reader, BAD_CAST("IntegerMappingTransformation"));
    constXcosNames[e_Item] = xmlTextReaderConstString(reader, BAD_CAST("Item"));
    constXcosNames[e_K] = xmlTextReaderConstString(reader, BAD_CAST("K"));
    constXcosNames[e_kg] = xmlTextReaderConstString(reader, BAD_CAST("kg"));
    constXcosNames[e_kind] = xmlTextReaderConstString(reader, BAD_CAST("kind"));
    constXcosNames[e_license] = xmlTextReaderConstString(reader, BAD_CAST("license"));
    constXcosNames[e_LinearTransformation] = xmlTextReaderConstString(reader, BAD_CAST("LinearTransformation"));
    constXcosNames[e_m] = xmlTextReaderConstString(reader, BAD_CAST("m"));
    constXcosNames[e_MapEntry] = xmlTextReaderConstString(reader, BAD_CAST("MapEntry"));
    constXcosNames[e_MappingEntry] = xmlTextReaderConstString(reader, BAD_CAST("MappingEntry"));
    constXcosNames[e_mime_type] = xmlTextReaderConstString(reader, BAD_CAST("mime-type"));
    constXcosNames[e_ModelExchange] = xmlTextReaderConstString(reader, BAD_CAST("ModelExchange"));
    constXcosNames[e_mol] = xmlTextReaderConstString(reader, BAD_CAST("mol"));
    constXcosNames[e_name] = xmlTextReaderConstString(reader, BAD_CAST("name"));
    constXcosNames[e_Note] = xmlTextReaderConstString(reader, BAD_CAST("Note"));
    constXcosNames[e_offset] = xmlTextReaderConstString(reader, BAD_CAST("offset"));
    constXcosNames[e_output] = xmlTextReaderConstString(reader, BAD_CAST("output"));
    constXcosNames[e_parameter] = xmlTextReaderConstString(reader, BAD_CAST("parameter"));
    constXcosNames[e_Parameter] = xmlTextReaderConstString(reader, BAD_CAST("Parameter"));
    constXcosNames[e_ParameterBinding] = xmlTextReaderConstString(reader, BAD_CAST("ParameterBinding"));
    constXcosNames[e_ParameterBindings] = xmlTextReaderConstString(reader, BAD_CAST("ParameterBindings"));
    constXcosNames[e_ParameterMapping] = xmlTextReaderConstString(reader, BAD_CAST("ParameterMapping"));
    constXcosNames[e_Parameters] = xmlTextReaderConstString(reader, BAD_CAST("Parameters"));
    constXcosNames[e_ParameterSet] = xmlTextReaderConstString(reader, BAD_CAST("ParameterSet"));
    constXcosNames[e_ParameterValues] = xmlTextReaderConstString(reader, BAD_CAST("ParameterValues"));
    constXcosNames[e_pointsX] = xmlTextReaderConstString(reader, BAD_CAST("pointsX"));
    constXcosNames[e_pointsY] = xmlTextReaderConstString(reader, BAD_CAST("pointsY"));
    constXcosNames[e_prefix] = xmlTextReaderConstString(reader, BAD_CAST("prefix"));
    constXcosNames[e_rad] = xmlTextReaderConstString(reader, BAD_CAST("rad"));
    constXcosNames[e_Real] = xmlTextReaderConstString(reader, BAD_CAST("Real"));
    constXcosNames[e_rotation] = xmlTextReaderConstString(reader, BAD_CAST("rotation"));
    constXcosNames[e_s] = xmlTextReaderConstString(reader, BAD_CAST("s"));
    constXcosNames[e_SignalDictionaries] = xmlTextReaderConstString(reader, BAD_CAST("SignalDictionaries"));
    constXcosNames[e_SignalDictionary] = xmlTextReaderConstString(reader, BAD_CAST("SignalDictionary"));
    constXcosNames[e_SignalDictionaryReference] = xmlTextReaderConstString(reader, BAD_CAST("SignalDictionaryReference"));
    constXcosNames[e_source] = xmlTextReaderConstString(reader, BAD_CAST("source"));
    constXcosNames[e_sourceBase] = xmlTextReaderConstString(reader, BAD_CAST("sourceBase"));
    constXcosNames[e_SSD] = xmlTextReaderConstString(reader, BAD_CAST("SSD"));
    constXcosNames[e_startConnector] = xmlTextReaderConstString(reader, BAD_CAST("startConnector"));
    constXcosNames[e_startElement] = xmlTextReaderConstString(reader, BAD_CAST("startElement"));
    constXcosNames[e_startTime] = xmlTextReaderConstString(reader, BAD_CAST("startTime"));
    constXcosNames[e_stopTime] = xmlTextReaderConstString(reader, BAD_CAST("stopTime"));
    constXcosNames[e_String] = xmlTextReaderConstString(reader, BAD_CAST("String"));
    constXcosNames[e_suppressUnitConversion] = xmlTextReaderConstString(reader, BAD_CAST("suppressUnitConversion"));
    constXcosNames[e_System] = xmlTextReaderConstString(reader, BAD_CAST("System"));
    constXcosNames[e_SystemGeometry] = xmlTextReaderConstString(reader, BAD_CAST("SystemGeometry"));
    constXcosNames[e_SystemStructureDescription] = xmlTextReaderConstString(reader, BAD_CAST("SystemStructureDescription"));
    constXcosNames[e_target] = xmlTextReaderConstString(reader, BAD_CAST("target"));
    constXcosNames[e_text] = xmlTextReaderConstString(reader, BAD_CAST("text"));
    constXcosNames[e_type] = xmlTextReaderConstString(reader, BAD_CAST("type"));
    constXcosNames[e_unit] = xmlTextReaderConstString(reader, BAD_CAST("unit"));
    constXcosNames[e_Unit] = xmlTextReaderConstString(reader, BAD_CAST("Unit"));
    constXcosNames[e_Units] = xmlTextReaderConstString(reader, BAD_CAST("Units"));
    constXcosNames[e_value] = xmlTextReaderConstString(reader, BAD_CAST("value"));
    constXcosNames[e_version] = xmlTextReaderConstString(reader, BAD_CAST("version"));
    constXcosNames[e_x] = xmlTextReaderConstString(reader, BAD_CAST("x"));
    constXcosNames[e_x1] = xmlTextReaderConstString(reader, BAD_CAST("x1"));
    constXcosNames[e_x2] = xmlTextReaderConstString(reader, BAD_CAST("x2"));
    constXcosNames[e_y] = xmlTextReaderConstString(reader, BAD_CAST("y"));
    constXcosNames[e_y1] = xmlTextReaderConstString(reader, BAD_CAST("y1"));
    constXcosNames[e_y2] = xmlTextReaderConstString(reader, BAD_CAST("y2"));

    xcosNamespaceUri = xmlTextReaderConstString(reader, BAD_CAST("org.scilab.modules.xcos"));
    xmlnsSSC = xmlTextReaderConstString(reader, BAD_CAST("http://ssp-standard.org/SSP1/SystemStructureCommon"));
    xmlnsSSB = xmlTextReaderConstString(reader, BAD_CAST("http://ssp-standard.org/SSP1/SystemStructureSignalDictionary"));
    xmlnsSSD = xmlTextReaderConstString(reader, BAD_CAST("http://ssp-standard.org/SSP1/SystemStructureDescription"));
    xmlnsSSV = xmlTextReaderConstString(reader, BAD_CAST("http://ssp-standard.org/SSP1/SystemStructureParameterValues"));
    xmlnsSSM = xmlTextReaderConstString(reader, BAD_CAST("http://ssp-standard.org/SSP1/SystemStructureParameterMapping"));
};

int SSPResource::load(const char* uri)
{
    int ret = 0;

    State st;

    /*
     * Decompress the SSP zip file into a temporary directory
     */
    archive_read_support_format_all(st.input());
    archive_read_support_filter_all(st.input());
    if (archive_read_open_filename(st.input(), uri, BLOCK_SIZE) != ARCHIVE_OK)
    {
        sciprint("Unable to open %s\n", uri);
        return -1;
    }

    /*
     * SystemStructure is extracted in memory, prepare a directory for other contents (fmu or other ressources)
     */
    archive_write_disk_set_standard_lookup(st.directory());
    /* Select which attributes we want to restore. */
    int flags = ARCHIVE_EXTRACT_TIME;
    flags |= ARCHIVE_EXTRACT_PERM;
    flags |= ARCHIVE_EXTRACT_ACL;
    flags |= ARCHIVE_EXTRACT_FFLAGS;
    archive_write_disk_set_options(st.directory(), flags);
    archive_write_disk_set_standard_lookup(st.directory());

    for (;;)
    {
        int res = archive_read_next_header2(st.input(), st.entry());
        if (res == ARCHIVE_EOF)
        {
            break;
        }
        if (res < ARCHIVE_WARN)
        {
            sciprint("Unable to load %s: %s\n", uri, archive_error_string(st.input()));
            return -1;
        }
        if (res == ARCHIVE_WARN)
        {
            Sciwarning("Warning on %s load: %s\n", uri, archive_error_string(st.input()));
        }

        // fprintf(stderr, "reading %s\n", archive_entry_pathname(st.entry()));
        const char* pathname = archive_entry_pathname(st.entry());
        if (strcmp(pathname, "SystemStructure.ssd") == 0)
        {
            /*
             * Load the main system structure, this is a mandatory file.
             *
             * Allocate the reader object, this API is used as it is simpler to use than SAX2 :
             *  * we have direct access to a node object
             *  * Strings are interned by libxml2
             *  * partial SAX2 callbacks are not supported by libxml2
             */
            xmlTextReaderPtr reader;
            /* resolve xinclude and intern strings */

            reader = xmlReaderForIO(ioread, ioclose, st.input(), uri, NULL, XML_PARSE_XINCLUDE | XML_PARSE_COMPACT);
            internPredefinedStrings(reader);

            /*
             * Process the document
             */
            if (reader != NULL)
            {
                ret = xmlTextReaderRead(reader);
                while (ret == 1)
                {
                    ret = processNode(reader);
                    if (ret == 1)
                    {
                        ret = xmlTextReaderRead(reader);
                    }
                }
                /*
                 * Once the document has been fully parsed check the validation results
                 */
                if (xmlTextReaderIsValid(reader) < 0)
                {
                    sciprint("Document %s does not validate\n", uri);
                }
                xmlNodePtr curNode = xmlTextReaderCurrentNode(reader);
                int line = xmlGetLineNo(curNode);
                xmlFreeTextReader(reader);
                if (ret < 0)
                {
                    sciprint("zip://%s#%s line %d was not parsed\n", uri, pathname, line);
                    return ret;
                }
            }
            else
            {
                sciprint("Unable to open %s\n", uri);
                return -1;
            }
        }
        else if (strncmp(pathname, "resources/", 10) == 0 || strncmp(pathname, "extra/", 6) == 0 || strncmp(pathname, "documentation/", 14) == 0)
        {
            // other files are extracted into the disk and can be used later
            char* tmpdir = getTMPDIR();
            const std::string fullPathname = tmpdir + std::string("/") + pathname;
            FREE(tmpdir);
            archive_entry_set_pathname(st.entry(), fullPathname.c_str());

            res = archive_write_header(st.directory(), st.entry());
            if (res < ARCHIVE_WARN)
            {
                sciprint("Unable to load %s: %s\n", uri, archive_error_string(st.directory()));
                return -1;
            }
            if (res == ARCHIVE_WARN)
            {
                Sciwarning("Warning on %s loading: %s\n", uri, archive_error_string(st.directory()));
            }
            res = copy_data(st.input(), st.directory());
            if (res < ARCHIVE_WARN)
            {
                sciprint("Unable to load %s: %s\n", uri, archive_error_string(st.directory()));
                return -1;
            }
            if (res == ARCHIVE_WARN)
            {
                Sciwarning("Warning on %s loading: %s\n", uri, archive_error_string(st.directory()));
            }
        }
        else
        {
            sciprint("The variant %s is not loaded from %s\n", pathname, uri);
        }
    }

    return ret;
}

/*
 * Convert an XML UTF-8 string to a model string
 */
static std::string to_string(const xmlChar* xmlStr)
{
    if (xmlStr == nullptr)
    {
        return "";
    }

    // the strings in the model are stored as UTF-8 as in libxml2
    return std::string((const char*)xmlStr);
}

/*
 * Convert an XML UTF-8 string to a model int
 */
static int to_int(const xmlChar* xmlStr)
{
    if (xmlStr == nullptr)
    {
        return 0;
    }

    return std::atoi((const char*)xmlStr);
}

/*
 * Convert an XML UTF-8 string to a model boolean
 */
static bool to_boolean(const xmlChar* xmlStr)
{
    if (xmlStr == nullptr)
    {
        return 0;
    }

    return xmlStrcmp(xmlStr, (const xmlChar*)"true") == 0;
}

/*
 * Convert an XML UTF-8 string to a model double
 */
static double to_double(const xmlChar* xmlStr)
{
    if (xmlStr == nullptr)
    {
        return 0.0;
    }

    return std::atof((const char*)xmlStr);
}

int SSPResource::loadSystemStructureDescription(xmlTextReaderPtr reader, model::BaseObject* o)
{
    assert(o->kind() == DIAGRAM);

    // iterate on attributes
    for (int rc = xmlTextReaderMoveToFirstAttribute(reader); rc > 0; rc = xmlTextReaderMoveToNextAttribute(reader))
    {
        const xmlChar* attribute = xmlTextReaderConstName(reader);
        auto found = std::find(constXcosNames.begin(), constXcosNames.end(), attribute);
        if (found == constXcosNames.end())
        {
            continue;
        }
        enum xcosNames current = static_cast<enum xcosNames>(std::distance(constXcosNames.begin(), found));
        switch (current)
        {
            case e_version:
            {
                std::string version = to_string(xmlTextReaderConstValue(reader));
                if (version != std::string("1.0") && std::string(version, 0, 3) != std::string("2.0"))
                {
                    sciprint("SSD version %s is not supported\n", version.c_str());
                    return -1;
                }
                break;
            }

            case e_name:
            {
                std::string name = to_string(xmlTextReaderConstValue(reader));
                if (controller.setObjectProperty(o, NAME, name) == FAIL)
                {
                    return -1;
                }
                break;
            }

            case e_description:
            {
                std::string description = to_string(xmlTextReaderConstValue(reader));
                if (controller.setObjectProperty(o, DESCRIPTION, description) == FAIL)
                {
                    return -1;
                }
                break;
            }

            case e_author:
            {
                std::string author = to_string(xmlTextReaderConstValue(reader));
                if (controller.setObjectProperty(o, AUTHOR, author) == FAIL)
                {
                    return -1;
                }
                break;
            }

            case e_fileversion:
            {
                std::string fileversion = to_string(xmlTextReaderConstValue(reader));
                if (controller.setObjectProperty(o, FILE_VERSION, fileversion) == FAIL)
                {
                    return -1;
                }
                break;
            }

            case e_copyright:
            {
                std::string copyright = to_string(xmlTextReaderConstValue(reader));
                if (controller.setObjectProperty(o, COPYRIGHT, copyright) == FAIL)
                {
                    return -1;
                }
                break;
            }

            case e_license:
            {
                std::string license = to_string(xmlTextReaderConstValue(reader));
                if (controller.setObjectProperty(o, LICENSE, license) == FAIL)
                {
                    return -1;
                }
                break;
            }

            case e_generationTool:
            {
                std::string generationTool = to_string(xmlTextReaderConstValue(reader));
                if (controller.setObjectProperty(o, GENERATION_TOOL, generationTool) == FAIL)
                {
                    return -1;
                }
                break;
            }

            case e_generationDateAndTime:
            {
                std::string generationDateAndTime = to_string(xmlTextReaderConstValue(reader));
                if (controller.setObjectProperty(o, GENERATION_DATE, generationDateAndTime) == FAIL)
                {
                    return -1;
                }
                break;
            }

            default:
                // ignore other parameters
                break;
        }
    }

    return 1;
}

int SSPResource::loadSystem(xmlTextReaderPtr reader, model::BaseObject* o)
{
    assert(o->kind() == DIAGRAM || o->kind() == BLOCK);

    // iterate on attributes
    for (int rc = xmlTextReaderMoveToFirstAttribute(reader); rc > 0; rc = xmlTextReaderMoveToNextAttribute(reader))
    {
        const xmlChar* attribute = xmlTextReaderConstName(reader);
        auto found = std::find(constXcosNames.begin(), constXcosNames.end(), attribute);
        if (found == constXcosNames.end())
        {
            continue;
        }
        enum xcosNames current = static_cast<enum xcosNames>(std::distance(constXcosNames.begin(), found));
        switch (current)
        {
            case e_name:
            {
                std::string name = to_string(xmlTextReaderConstValue(reader));
                if (controller.setObjectProperty(o, NAME, name) == FAIL)
                {
                    return -1;
                }
                break;
            }

            case e_description:
            {
                std::string description = to_string(xmlTextReaderConstValue(reader));
                if (controller.setObjectProperty(o, DESCRIPTION, description) == FAIL)
                {
                    return -1;
                }
                break;
            }

            default:
                // ignore other parameters
                break;
        }
    }

    return 1;
}

int SSPResource::loadDefaultExperiment(xmlTextReaderPtr reader, model::BaseObject* o)
{
    assert(o->kind() == DIAGRAM);

    double startTime = 0.;
    double stopTime = 30.;

    // iterate on attributes
    for (int rc = xmlTextReaderMoveToFirstAttribute(reader); rc > 0; rc = xmlTextReaderMoveToNextAttribute(reader))
    {
        const xmlChar* attribute = xmlTextReaderConstName(reader);
        auto found = std::find(constXcosNames.begin(), constXcosNames.end(), attribute);
        if (found == constXcosNames.end())
        {
            continue;
        }
        enum xcosNames current = static_cast<enum xcosNames>(std::distance(constXcosNames.begin(), found));
        switch (current)
        {
            case e_startTime:
            {
                startTime = to_double(xmlTextReaderConstValue(reader));
                break;
            }

            case e_stopTime:
            {
                stopTime = to_double(xmlTextReaderConstValue(reader));
                break;
            }

            default:
                break;
        }
    }

    std::vector<double> properties;
    controller.getObjectProperty(o, PROPERTIES, properties);

    properties[0] = stopTime - startTime;
    if (controller.setObjectProperty(o, PROPERTIES, properties) == FAIL)
    {
        return -1;
    }

    return 1;
}

int SSPResource::loadConnector(xmlTextReaderPtr reader, model::BaseObject* o)
{
    assert(o->kind() == PORT);

    // iterate on attributes
    for (int rc = xmlTextReaderMoveToFirstAttribute(reader); rc > 0; rc = xmlTextReaderMoveToNextAttribute(reader))
    {
        const xmlChar* attribute = xmlTextReaderConstName(reader);
        auto found = std::find(constXcosNames.begin(), constXcosNames.end(), attribute);
        if (found == constXcosNames.end())
        {
            continue;
        }
        enum xcosNames current = static_cast<enum xcosNames>(std::distance(constXcosNames.begin(), found));
        switch (current)
        {
            case e_id:
            {
                std::string id = to_string(xmlTextReaderConstValue(reader));
                if (controller.setObjectProperty(o, UID, id) == FAIL)
                {
                    return -1;
                }
                break;
            }

            case e_description:
            {
                std::string description = to_string(xmlTextReaderConstValue(reader));
                if (controller.setObjectProperty(o, DESCRIPTION, description) == FAIL)
                {
                    return -1;
                }
                break;
            }
            case e_name:
            {
                std::string name = to_string(xmlTextReaderConstValue(reader));
                if (controller.setObjectProperty(o, NAME, name) == FAIL)
                {
                    return -1;
                }
                references.back().connector = name;
                break;
            }

            case e_kind:
            {
                // input / output / inout are interned
                const xmlChar* kind = xmlTextReaderConstString(reader, xmlTextReaderConstValue(reader));
                auto found = std::find(constXcosNames.begin(), constXcosNames.end(), kind);
                if (found == constXcosNames.end())
                {
                    continue;
                }
                enum xcosNames current = static_cast<enum xcosNames>(std::distance(constXcosNames.begin(), found));

                enum portKind port = PORT_UNDEF;
                bool isImplicit = false;
                switch (current)
                {
                    case e_input:
                        port = PORT_IN;
                        break;
                    case e_output:
                        port = PORT_OUT;
                        break;
                    case e_parameter:
                        port = PORT_IN;
                        break;
                    case e_calculatedParameter:
                        port = PORT_OUT;
                        break;
                    case e_acausal: // fallthrough
                    case e_inout:
                        port = PORT_IN; // will be updated on geometry update
                        isImplicit = true;
                        break;
                    default:
                        return -1;
                }

                if (controller.setObjectProperty(o, PORT_KIND, port) == FAIL)
                {
                    return -1;
                }
                if (controller.setObjectProperty(o, IMPLICIT, isImplicit) == FAIL)
                {
                    return -1;
                }
                break;
            }

            default:
                // ignore other parameters
                break;
        }
    }

    return 1;
}

int SSPResource::loadReal(xmlTextReaderPtr reader, model::BaseObject* o)
{
    assert(o->kind() == PORT || o->kind() == BLOCK || o->kind() == DIAGRAM);

    switch (o->kind())
    {
        case PORT:
        {
            // set the type
            if (controller.setObjectProperty(o, DATATYPE_ROWS, 1) == FAIL)
            {
                return -1;
            }
            if (controller.setObjectProperty(o, DATATYPE_COLS, 1) == FAIL)
            {
                return -1;
            }
            if (controller.setObjectProperty(o, DATATYPE_TYPE, 1) == FAIL)
            {
                return -1;
            }

            if (xmlTextReaderMoveToFirstAttribute(reader) > 0)
            {
                const xmlChar* attribute = xmlTextReaderConstName(reader);
                auto found = std::find(constXcosNames.begin(), constXcosNames.end(), attribute);
                enum xcosNames current = static_cast<enum xcosNames>(std::distance(constXcosNames.begin(), found));
                if (current == e_unit)
                {
                    if (controller.setObjectProperty(o, PARAMETER_UNIT, to_string(xmlTextReaderConstValue(reader))) == FAIL)
                    {
                        return -1;
                    }
                }
            }
            break;
        }

        case DIAGRAM: // fallthrough
        case BLOCK:
        {
            std::vector<std::string> types;
            if (!controller.getObjectProperty(o, PARAMETER_TYPE, types))
            {
                return -1;
            }
            if (types.empty())
            {
                types.push_back("real");
            }
            else
            {
                types.back() = "real";
            }
            if (controller.setObjectProperty(o, PARAMETER_TYPE, types) == FAIL)
            {
                return -1;
            }

            // iterate on attributes
            for (int rc = xmlTextReaderMoveToFirstAttribute(reader); rc > 0; rc = xmlTextReaderMoveToNextAttribute(reader))
            {
                const xmlChar* attribute = xmlTextReaderConstName(reader);
                auto found = std::find(constXcosNames.begin(), constXcosNames.end(), attribute);
                if (found == constXcosNames.end())
                {
                    continue;
                }
                enum xcosNames current = static_cast<enum xcosNames>(std::distance(constXcosNames.begin(), found));
                switch (current)
                {
                    case e_value:
                    {
                        std::vector<std::string> values;
                        if (!controller.getObjectProperty(o, PARAMETER_VALUE, values))
                        {
                            return -1;
                        }
                        values.back() = to_string(xmlTextReaderConstValue(reader));
                        if (controller.setObjectProperty(o, PARAMETER_VALUE, values) == FAIL)
                        {
                            return -1;
                        }
                        break;
                    }
                    case e_unit:
                    {
                        std::vector<std::string> units;
                        if (!controller.getObjectProperty(o, PARAMETER_UNIT, units))
                        {
                            return -1;
                        }
                        units.back() = to_string(xmlTextReaderConstValue(reader));
                        if (controller.setObjectProperty(o, PARAMETER_UNIT, units) == FAIL)
                        {
                            return -1;
                        }
                        break;
                    }

                    default:
                        break;
                }
            }
            break;
        }

        default:
            // ignore other parameters
            break;
    }

    return 1;
}

int SSPResource::loadInteger(xmlTextReaderPtr reader, model::BaseObject* o)
{
    assert(o->kind() == PORT || o->kind() == BLOCK);

    switch (o->kind())
    {
        case PORT:
        {
            // set the type
            if (controller.setObjectProperty(o, DATATYPE_ROWS, 1) == FAIL)
            {
                return -1;
            }
            if (controller.setObjectProperty(o, DATATYPE_COLS, 1) == FAIL)
            {
                return -1;
            }
            if (controller.setObjectProperty(o, DATATYPE_TYPE, 3) == FAIL)
            {
                return -1;
            }
            break;
        }

        case BLOCK:
        {
            std::vector<std::string> types;
            if (!controller.getObjectProperty(o, PARAMETER_TYPE, types))
            {
                return -1;
            }
            if (types.empty())
            {
                types.push_back("integer");
            }
            else
            {
                types.back() = "integer";
            }
            if (controller.setObjectProperty(o, PARAMETER_TYPE, types) == FAIL)
            {
                return -1;
            }

            // iterate on attributes
            for (int rc = xmlTextReaderMoveToFirstAttribute(reader); rc > 0; rc = xmlTextReaderMoveToNextAttribute(reader))
            {
                const xmlChar* attribute = xmlTextReaderConstName(reader);
                auto found = std::find(constXcosNames.begin(), constXcosNames.end(), attribute);
                if (found == constXcosNames.end())
                {
                    continue;
                }
                enum xcosNames current = static_cast<enum xcosNames>(std::distance(constXcosNames.begin(), found));
                switch (current)
                {
                    case e_value:
                    {
                        std::vector<std::string> values;
                        if (!controller.getObjectProperty(o, PARAMETER_VALUE, values))
                        {
                            return -1;
                        }
                        values.back() = to_string(xmlTextReaderConstValue(reader));
                        if (controller.setObjectProperty(o, PARAMETER_VALUE, values) == FAIL)
                        {
                            return -1;
                        }
                        break;
                    }

                    default:
                        break;
                }
            }
            break;
        }

        default:
            // ignore other parameters
            break;
    }

    return 1;
}

int SSPResource::loadBoolean(xmlTextReaderPtr reader, model::BaseObject* o)
{
    assert(o->kind() == PORT || o->kind() == BLOCK);

    switch (o->kind())
    {
        case PORT:
        {
            // set the type
            if (controller.setObjectProperty(o, DATATYPE_ROWS, 1) == FAIL)
            {
                return -1;
            }
            if (controller.setObjectProperty(o, DATATYPE_COLS, 1) == FAIL)
            {
                return -1;
            }
            if (controller.setObjectProperty(o, DATATYPE_TYPE, 5) == FAIL)
            {
                return -1;
            }
            break;
        }
        case BLOCK:
        {
            std::vector<std::string> types;
            if (!controller.getObjectProperty(o, PARAMETER_TYPE, types))
            {
                return -1;
            }
            if (types.empty())
            {
                types.push_back("boolean");
            }
            else
            {
                types.back() = "boolean";
            }
            if (controller.setObjectProperty(o, PARAMETER_TYPE, types) == FAIL)
            {
                return -1;
            }

            // iterate on attributes
            for (int rc = xmlTextReaderMoveToFirstAttribute(reader); rc > 0; rc = xmlTextReaderMoveToNextAttribute(reader))
            {
                const xmlChar* attribute = xmlTextReaderConstName(reader);
                auto found = std::find(constXcosNames.begin(), constXcosNames.end(), attribute);
                if (found == constXcosNames.end())
                {
                    continue;
                }
                enum xcosNames current = static_cast<enum xcosNames>(std::distance(constXcosNames.begin(), found));
                switch (current)
                {
                    case e_value:
                    {
                        std::vector<std::string> values;
                        if (!controller.getObjectProperty(o, PARAMETER_VALUE, values))
                        {
                            return -1;
                        }
                        values.back() = to_string(xmlTextReaderConstValue(reader));
                        if (controller.setObjectProperty(o, PARAMETER_VALUE, values) == FAIL)
                        {
                            return -1;
                        }
                        break;
                    }

                    default:
                        break;
                }
            }
            break;
        }

        default:
            // ignore other parameters
            break;
    }

    return 1;
}

int SSPResource::loadString(xmlTextReaderPtr reader, model::BaseObject* o)
{
    assert(o->kind() == PORT || o->kind() == BLOCK);

    switch (o->kind())
    {
        case PORT:
        {
            // set the type
            if (controller.setObjectProperty(o, DATATYPE_ROWS, 1) == FAIL)
            {
                return -1;
            }
            if (controller.setObjectProperty(o, DATATYPE_COLS, 1) == FAIL)
            {
                return -1;
            }
            // string can be passed around as a pointer to a string
            if (controller.setObjectProperty(o, DATATYPE_TYPE, 1) == FAIL)
            {
                return -1;
            }
            break;
        }

        case BLOCK:
        {
            std::vector<std::string> types;
            if (!controller.getObjectProperty(o, PARAMETER_TYPE, types))
            {
                return -1;
            }
            if (types.empty())
            {
                types.push_back("string");
            }
            else
            {
                types.back() = "string";
            }
            if (controller.setObjectProperty(o, PARAMETER_TYPE, types) == FAIL)
            {
                return -1;
            }

            // iterate on attributes
            for (int rc = xmlTextReaderMoveToFirstAttribute(reader); rc > 0; rc = xmlTextReaderMoveToNextAttribute(reader))
            {
                const xmlChar* attribute = xmlTextReaderConstName(reader);
                auto found = std::find(constXcosNames.begin(), constXcosNames.end(), attribute);
                if (found == constXcosNames.end())
                {
                    continue;
                }
                enum xcosNames current = static_cast<enum xcosNames>(std::distance(constXcosNames.begin(), found));
                switch (current)
                {
                    case e_value:
                    {
                        std::vector<std::string> values;
                        if (!controller.getObjectProperty(o, PARAMETER_VALUE, values))
                        {
                            return -1;
                        }
                        values.back() = to_string(xmlTextReaderConstValue(reader));
                        if (controller.setObjectProperty(o, PARAMETER_VALUE, values) == FAIL)
                        {
                            return -1;
                        }
                        break;
                    }

                    default:
                        break;
                }
            }
            break;
        }

        default:
            // ignore other parameters
            break;
    }

    return 1;
}

int SSPResource::loadEnumeration(xmlTextReaderPtr reader, model::BaseObject* o)
{
    assert(o->kind() == PORT || o->kind() == BLOCK);

    switch (o->kind())
    {
        case PORT:
        {
            // set the type
            if (controller.setObjectProperty(o, DATATYPE_ROWS, 1) == FAIL)
            {
                return -1;
            }
            if (controller.setObjectProperty(o, DATATYPE_COLS, 1) == FAIL)
            {
                return -1;
            }
            if (controller.setObjectProperty(o, DATATYPE_TYPE, 3) == FAIL)
            {
                return -1;
            }
            break;
        }

        case BLOCK:
        {
            std::vector<std::string> types;
            if (!controller.getObjectProperty(o, PARAMETER_TYPE, types))
            {
                return -1;
            }
            if (types.empty())
            {
                types.push_back("enumeration");
            }
            else
            {
                types.back() = "enumeration";
            }
            if (controller.setObjectProperty(o, PARAMETER_TYPE, types) == FAIL)
            {
                return -1;
            }

            // iterate on attributes
            for (int rc = xmlTextReaderMoveToFirstAttribute(reader); rc > 0; rc = xmlTextReaderMoveToNextAttribute(reader))
            {
                const xmlChar* attribute = xmlTextReaderConstName(reader);
                auto found = std::find(constXcosNames.begin(), constXcosNames.end(), attribute);
                if (found == constXcosNames.end())
                {
                    continue;
                }
                enum xcosNames current = static_cast<enum xcosNames>(std::distance(constXcosNames.begin(), found));
                switch (current)
                {
                    case e_value:
                    {
                        std::vector<std::string> values;
                        if (!controller.getObjectProperty(o, PARAMETER_VALUE, values))
                        {
                            return -1;
                        }
                        values.back() = to_string(xmlTextReaderConstValue(reader));
                        if (controller.setObjectProperty(o, PARAMETER_VALUE, values) == FAIL)
                        {
                            return -1;
                        }
                        break;
                    }

                    default:
                        break;
                }
            }
            break;
        }

        default:
            // ignore other parameters
            break;
    }

    return 1;
}

int SSPResource::loadBinary(xmlTextReaderPtr reader, model::BaseObject* o)
{
    assert(o->kind() == PORT || o->kind() == BLOCK);

    switch (o->kind())
    {
        case PORT:
        {
            // set the type
            if (controller.setObjectProperty(o, DATATYPE_ROWS, 1) == FAIL)
            {
                return -1;
            }
            if (controller.setObjectProperty(o, DATATYPE_COLS, 1) == FAIL)
            {
                return -1;
            }
            // from the specification: a length-terminated binary data type
            if (controller.setObjectProperty(o, DATATYPE_TYPE, 5) == FAIL)
            {
                return -1;
            }
            break;
        }

        case BLOCK:
        {
            std::vector<std::string> types;
            if (!controller.getObjectProperty(o, PARAMETER_TYPE, types))
            {
                return -1;
            }
            if (types.empty())
            {
                types.push_back("binary");
            }
            else
            {
                types.back() = "binary";
            }
            if (controller.setObjectProperty(o, PARAMETER_TYPE, types) == FAIL)
            {
                return -1;
            }

            // iterate on attributes
            for (int rc = xmlTextReaderMoveToFirstAttribute(reader); rc > 0; rc = xmlTextReaderMoveToNextAttribute(reader))
            {
                const xmlChar* attribute = xmlTextReaderConstName(reader);
                auto found = std::find(constXcosNames.begin(), constXcosNames.end(), attribute);
                if (found == constXcosNames.end())
                {
                    continue;
                }
                enum xcosNames current = static_cast<enum xcosNames>(std::distance(constXcosNames.begin(), found));
                switch (current)
                {
                    case e_source: // fallthrough
                    case e_value:
                    {
                        std::vector<std::string> values;
                        if (!controller.getObjectProperty(o, PARAMETER_VALUE, values))
                        {
                            return -1;
                        }
                        values.back() = to_string(xmlTextReaderConstValue(reader));
                        if (controller.setObjectProperty(o, PARAMETER_VALUE, values) == FAIL)
                        {
                            return -1;
                        }
                        break;
                    }

                    case e_mime_type:
                    {
                        std::vector<std::string> values;
                        if (!controller.getObjectProperty(o, PARAMETER_ENCODING, values))
                        {
                            return -1;
                        }
                        values.back() = to_string(xmlTextReaderConstValue(reader));
                        if (controller.setObjectProperty(o, PARAMETER_ENCODING, values) == FAIL)
                        {
                            return -1;
                        }
                        break;
                    }

                    default:
                        break;
                }
            }
            break;
        }

        default:
            // ignore other parameters
            break;
    }

    return 1;
}

int SSPResource::loadConnection(xmlTextReaderPtr reader, model::BaseObject* o)
{
    assert(o->kind() == BLOCK || o->kind() == DIAGRAM);

    model::BaseObject* link = controller.createBaseObject(LINK);

    // assign the child
    model::BaseObject* parent = processed.back();
    controller.setObjectProperty(link, PARENT_DIAGRAM, root);
    if (parent->kind() == BLOCK)
    {
        controller.setObjectProperty(link, PARENT_BLOCK, parent->id());
    }
    std::vector<ScicosID> children;
    controller.getObjectProperty(parent, CHILDREN, children);
    children.push_back(link->id());
    controller.setObjectProperty(parent, CHILDREN, children);

    // store into processed if there is children
    if (!xmlTextReaderIsEmptyElement(reader))
    {
        processed.push_back(link);
    }

    std::string startElement;
    std::string startConnector;
    std::string endElement;
    std::string endConnector;

    // iterate on attributes
    for (int rc = xmlTextReaderMoveToFirstAttribute(reader); rc > 0; rc = xmlTextReaderMoveToNextAttribute(reader))
    {
        const xmlChar* attribute = xmlTextReaderConstName(reader);
        auto found = std::find(constXcosNames.begin(), constXcosNames.end(), attribute);
        if (found == constXcosNames.end())
        {
            continue;
        }

        enum xcosNames current = static_cast<enum xcosNames>(std::distance(constXcosNames.begin(), found));
        switch (current)
        {
            case e_startElement:
            {
                startElement = to_string(xmlTextReaderConstValue(reader));
                break;
            }
            case e_startConnector:
            {
                startConnector = to_string(xmlTextReaderConstValue(reader));
                break;
            }
            case e_endElement:
            {
                endElement = to_string(xmlTextReaderConstValue(reader));
                break;
            }
            case e_endConnector:
            {
                endConnector = to_string(xmlTextReaderConstValue(reader));
                break;
            }
            case e_suppressUnitConversion:
            {
                // FIXME: not decoded yet ; should incompatible unit produce an error ?
                break;
            }
            case e_description:
            {
                std::string description = to_string(xmlTextReaderConstValue(reader));
                if (controller.setObjectProperty(link, DESCRIPTION, description) == FAIL)
                {
                    return -1;
                }
                break;
            }

            default:
                sciprint("unable to decode Connection\n");
                return -1;
        }
    }

    // resolve linkage with available references
    auto startIT = std::find_if(references.rbegin(), references.rend(), [&startElement, &startConnector](const Reference& r)
                                { return r.element == startElement && r.connector == startConnector; });
    if (startIT == references.rend())
    {
        sciprint("unable to decode Connection - startConnector reference\n");
        return -1;
    }

    if (controller.setObjectProperty(link, SOURCE_PORT, startIT->port->id()) == FAIL)
    {
        sciprint("unable to decode Connection - startConnector set\n");
        return -1;
    }
    if (controller.setObjectProperty(startIT->port, CONNECTED_SIGNALS, link->id()) == FAIL)
    {
        sciprint("unable to decode Connection - startConnector set block\n");
        return -1;
    }

    auto endIT = std::find_if(references.rbegin(), references.rend(), [&endElement, &endConnector](const Reference& r)
                              { return r.element == endElement && r.connector == endConnector; });
    if (endIT == references.rend())
    {
        sciprint("unable to decode Connection - endConnector reference\n");
        return -1;
    }

    if (controller.setObjectProperty(link, DESTINATION_PORT, endIT->port->id()) == FAIL)
    {
        sciprint("unable to decode Connection - endConnector set\n");
        return -1;
    }
    if (controller.setObjectProperty(endIT->port, CONNECTED_SIGNALS, link->id()) == FAIL)
    {
        sciprint("unable to decode Connection - endConnector set block\n");
        return -1;
    }

    return 1;
}

int SSPResource::loadSystemGeometry(xmlTextReaderPtr reader, model::BaseObject* o)
{
    assert(o->kind() == DIAGRAM || o->kind() == BLOCK);

    // read values
    double x1 = 0;
    double y1 = 0;
    double x2 = 0;
    double y2 = 0;

    // iterate on attributes
    for (int rc = xmlTextReaderMoveToFirstAttribute(reader); rc > 0; rc = xmlTextReaderMoveToNextAttribute(reader))
    {
        const xmlChar* attribute = xmlTextReaderConstName(reader);
        auto found = std::find(constXcosNames.begin(), constXcosNames.end(), attribute);
        if (found == constXcosNames.end())
        {
            continue;
        }
        enum xcosNames current = static_cast<enum xcosNames>(std::distance(constXcosNames.begin(), found));
        switch (current)
        {
            case e_x1:
            {
                x1 = to_double(xmlTextReaderConstValue(reader));
                break;
            }
            case e_y1:
            {
                y1 = to_double(xmlTextReaderConstValue(reader));
                break;
            }
            case e_x2:
            {
                x2 = to_double(xmlTextReaderConstValue(reader));
                break;
            }
            case e_y2:
            {
                y2 = to_double(xmlTextReaderConstValue(reader));
                break;
            }

            default:
                break;
        }
    }

    //
    // relocate the IOBlock according to the Geometry
    //

    // helper lambda function
    auto set_ioblock_geometry = [this, x1, y1, x2, y2](const Reference& ioBlock)
    {
        std::vector<double> absoluteGeom = {0, 0, 40, 20};

        // x
        absoluteGeom[0] = (x1 + ioBlock.x * (x2 - x1)  + 10) * ASPECT_RATIO;
        // y
        absoluteGeom[1] = ((1.0 - ioBlock.y) * (y2 - y1)  - 10 - y2) * ASPECT_RATIO;

        if (controller.setObjectProperty(ioBlock.block, GEOMETRY, absoluteGeom) == FAIL)
        {
            sciprint("unable to set SystemGeometry\n");
            return -1;
        }

        return 0;
    };

    std::vector<Reference>::reverse_iterator it = references.rbegin();
    // go up to the System Connector definition
    while (it != references.rend() && it->element != "")
    {
        it++;
    }
    // look for each connector
    std::vector<Reference>::reverse_iterator ioBlock = it;
    std::vector<Reference>::reverse_iterator outter = it + 1;
    while (outter != references.rend() && outter->connector == ioBlock->connector)
    {
        int ret = set_ioblock_geometry(*ioBlock);
        if (ret)
        {
            return ret;
        }
        if (copy_property<std::vector<int>>(outter->port, ioBlock->port, DATATYPE) == FAIL)
        {
            return FAIL;
        }


        outter += 2;
        ioBlock += 2;
    }
    // position main System's connectors
    if (o->kind() == DIAGRAM)
    {
        for (std::vector<Reference>::iterator ioBlock = references.begin(); ioBlock != references.end() && ioBlock->element == ""; ++ioBlock)
        {
            int ret = set_ioblock_geometry(*ioBlock);
            if (ret)
            {
                return ret;
            }
        }
    }

    // translate y-axis on blocks and links according to the SystemGeometry
    // y-axis should already be inverted
    std::vector<ScicosID> children;
    controller.getObjectProperty(o, CHILDREN, children);
    for (ScicosID id : children)
    {
        model::BaseObject* child = controller.getBaseObject(id);
        switch (child->kind())
        {
            case BLOCK:
            {
                std::vector<double> geom;
                controller.getObjectProperty(child, GEOMETRY, geom);
                geom[0] = geom[0] - x1;
                geom[1] = geom[1] + y2;
                controller.setObjectProperty(child, GEOMETRY, geom);
                break;
            }
            case LINK:
            {
                std::vector<double> points;
                controller.getObjectProperty(child, CONTROL_POINTS, points);
                for (size_t i = 1; i < points.size(); i += 2)
                {
                    points[i-1] = points[i-1] - x1;
                    points[i] = points[i] + y2;
                }
                controller.setObjectProperty(child, CONTROL_POINTS, points);
                break;
            }
            case ANNOTATION:
            {
                std::vector<double> geom;
                controller.getObjectProperty(child, GEOMETRY, geom);
                geom[0] = geom[0] - x1;
                geom[1] = geom[1] + y2;
                controller.setObjectProperty(child, GEOMETRY, geom);
                break;
            }
            default:
                break;
        }
    }

    // reorder and assign port indexes
    assignPortIndexes(o);

    return 1;
}

int SSPResource::loadConnectorGeometry(xmlTextReaderPtr reader, model::BaseObject* o)
{
    // the argument should be the associated IOBlock
    assert(o->kind() == BLOCK || o->kind() == PORT);
    model::BaseObject* port;
    if (o->kind() == PORT)
    {
        port = o;
    }
    else
    {
        port = references.back().port;
    }

    // in SSP coordinates
    double x = 1.05;
    double y = 0.2;
    double rotation = 0;

    // iterate on attributes
    for (int rc = xmlTextReaderMoveToFirstAttribute(reader); rc > 0; rc = xmlTextReaderMoveToNextAttribute(reader))
    {
        const xmlChar* attribute = xmlTextReaderConstName(reader);
        auto found = std::find(constXcosNames.begin(), constXcosNames.end(), attribute);
        if (found == constXcosNames.end())
        {
            continue;
        }
        enum xcosNames current = static_cast<enum xcosNames>(std::distance(constXcosNames.begin(), found));
        switch (current)
        {
            case e_x:
            {
                x = to_double(xmlTextReaderConstValue(reader));
                break;
            }
            case e_y:
            {
                y = to_double(xmlTextReaderConstValue(reader));
                break;
            }
            case e_rotation:
            {
                rotation = to_double(xmlTextReaderConstValue(reader));
                break;
            }
            default:
                sciprint("unable to decode ConnectorGeometry\n");
                return -1;
        }
    }

    // all IMPLICIT connectors are allocated as INPUTS, depending on the geometry they can be moved to OUTPUTS
    bool isImplicit;
    controller.getObjectProperty(port, IMPLICIT, isImplicit);
    if (isImplicit)
    {
        std::vector<ScicosID> ports;
        controller.getObjectProperty(o, INPUTS, ports);

        if (x > 0.5)
        {
            enum portKind k = PORT_OUT;

            auto newEnd = std::remove(ports.begin(), ports.end(), o->id());
            ports.erase(newEnd);
            controller.setObjectProperty(o, INPUTS, ports);

            controller.setObjectProperty(o, PORT_KIND, k);

            object_properties_t opposite = property_from_port(k);
            std::vector<ScicosID> ports;
            controller.getObjectProperty(o, opposite, ports);
            ports.push_back(o->id());
            if (controller.setObjectProperty(o, opposite, ports) == FAIL)
            {
                return -1;
            }
        }
    }

    // set the geometry on the ioBlock
    auto ioBlock = references.rbegin();
    ioBlock->x = x;
    ioBlock->y = y;

    // set the geometry on the port
    if (references.size() > 1)
    {
        auto outter = references.rbegin() + 1;
        if (ioBlock->element == "" && ioBlock->connector == outter->connector)
        {
            outter->x = x;
            outter->y = y;
        }
    }

    
    return 1;
}

int SSPResource::loadElementGeometry(xmlTextReaderPtr reader, model::BaseObject* o)
{
    assert(o->kind() == BLOCK);

    // in Xcos coordinates
    std::vector<double> x_y_w_h;
    x_y_w_h.resize(4);

    // in SSP coordinates
    double x1 = 0;
    double y1 = 0;
    double x2 = 0;
    double y2 = 0;
    double rotation = 0;

    // for images
    std::string iconSource;
    double iconRotation;
    bool iconFlip;
    bool fixedAspectRatio;

    // iterate on attributes
    for (int rc = xmlTextReaderMoveToFirstAttribute(reader); rc > 0; rc = xmlTextReaderMoveToNextAttribute(reader))
    {
        const xmlChar* attribute = xmlTextReaderConstName(reader);
        auto found = std::find(constXcosNames.begin(), constXcosNames.end(), attribute);
        if (found == constXcosNames.end())
        {
            continue;
        }
        enum xcosNames current = static_cast<enum xcosNames>(std::distance(constXcosNames.begin(), found));
        switch (current)
        {
            case e_x1:
            {
                x1 = to_double(xmlTextReaderConstValue(reader));
                break;
            }
            case e_y1:
            {
                y1 = to_double(xmlTextReaderConstValue(reader));
                break;
            }
            case e_x2:
            {
                x2 = to_double(xmlTextReaderConstValue(reader));
                break;
            }
            case e_y2:
            {
                y2 = to_double(xmlTextReaderConstValue(reader));
                break;
            }
            case e_rotation:
            {
                rotation = to_double(xmlTextReaderConstValue(reader));
                break;
            }
            case e_iconSource:
            {
                iconSource = to_string(xmlTextReaderConstValue(reader));
                break;
            }

            default:
                break;
        }
    }

    // (x1,y1) and (x2,y2) define the positions of the lower-left and upper-right corners of the model element in the coordinate system of its parent system. If x1>x2 this indicates horizontal flipping, y1>y2 indicates vertical flipping.
    // The optional attribute rotation (in degrees) defines an additional rotation (applied after flipping), where positive numbers indicate a counter clockwise rotation.

    std::string style;
    controller.getObjectProperty(o, INTERFACE_FUNCTION, style);

    // special style for any FMU block and where the fmu_wrapper toolbox is not already loaded
    if (style == std::string("SimpleFMU"))
    {
        style += ";blockWithLabel;displayedLabel=FMU %1$s";
    }

    if (x1 < x2)
    {
        x_y_w_h[0] = x1;
        x_y_w_h[2] = x2 - x1;
    }
    else
    {
        x_y_w_h[0] = x2;
        x_y_w_h[2] = x1 - x2;

        style += ";mirror=true";
    }
    // y-axis will be translated on SystemGeometry decoding
    if (y1 < y2)
    {
        x_y_w_h[1] = -y2;
        x_y_w_h[3] = y2 - y1;
    }
    else
    {
        x_y_w_h[1] = -y1;
        x_y_w_h[3] = y2 - y1;

        style += ";flip=true";
    }

    if (controller.setObjectProperty(o, GEOMETRY, x_y_w_h) == FAIL)
    {
        sciprint("unable to set ElementGeometry\n");
        return -1;
    }

    if (std::abs(rotation) > std::numeric_limits<double>::epsilon())
    {
        style += ";rotation=" + std::to_string(rotation);
    }
    if (controller.setObjectProperty(o, STYLE, style) == FAIL)
    {
        sciprint("unable to set ConnectorGeometry\n");
        return -1;
    }

    // reorder and assign port indexes
    assignPortIndexes(o);

    return 1;
}

int SSPResource::loadConnectionGeometry(xmlTextReaderPtr reader, model::BaseObject* o)
{
    assert(o->kind() == LINK);

    // read values
    std::vector<double> points;

    // iterate on attributes
    for (int rc = xmlTextReaderMoveToFirstAttribute(reader); rc > 0; rc = xmlTextReaderMoveToNextAttribute(reader))
    {
        bool allocated = !points.empty();

        const xmlChar* attribute = xmlTextReaderConstName(reader);
        auto found = std::find(constXcosNames.begin(), constXcosNames.end(), attribute);
        if (found == constXcosNames.end())
        {
            continue;
        }
        enum xcosNames current = static_cast<enum xcosNames>(std::distance(constXcosNames.begin(), found));
        switch (current)
        {
            case e_pointsX:
            {
                char* ptr = (char*)xmlTextReaderConstValue(reader);
                char* end;
                double x = std::strtod(ptr, &end);
                double y = 0.;
                for (size_t i = 0; ptr != end; x = std::strtod(ptr, &end), i++)
                {
                    ptr = end;

                    if (allocated && (2 * i + 1) < points.size())
                    {
                        points[2 * i] = ASPECT_RATIO * x;
                    }
                    else
                        {
                        points.push_back(ASPECT_RATIO * x);
                        points.push_back(y);
                    }
                }
                break;
            }

            case e_pointsY:
            {
                char* ptr = (char*)xmlTextReaderConstValue(reader);
                char* end;
                double x = 0.;
                double y = std::strtod(ptr, &end);
                for (size_t i = 0; ptr != end; y = std::strtod(ptr, &end), i++)
                {
                    ptr = end;

                    if (allocated && (2 * i + 1) < points.size())
                    {
                        points[2 * i + 1] = ASPECT_RATIO * -y;
                    }
                    else
                    {
                        points.push_back(x);
                        points.push_back(ASPECT_RATIO * -y);
                    }
                }
                break;
            }

            default:
                sciprint("unable to decode ConnectionGeometry\n");
                return -1;
        }
    }

    // duplicate start and end points
    if (!points.empty())
    {
        points.insert(points.begin(), {points[0], points[1]});
        points.insert(points.end(), {points[points.size() - 2], points[points.size() - 1]});
    }

    if (controller.setObjectProperty(o, CONTROL_POINTS, points) == FAIL)
    {
        sciprint("unable to set ConnectionGeometry\n");
        return -1;
    }

    return 1;
}

int SSPResource::loadParameterSet(xmlTextReaderPtr reader, model::BaseObject* o)
{
    assert(o->kind() == BLOCK || o->kind() == DIAGRAM);

    switch (o->kind())
    {
        case BLOCK: // fallthrough
        case DIAGRAM:
        {
            // iterate on attributes
            for (int rc = xmlTextReaderMoveToFirstAttribute(reader); rc > 0; rc = xmlTextReaderMoveToNextAttribute(reader))
            {
                const xmlChar* attribute = xmlTextReaderConstName(reader);
                auto found = std::find(constXcosNames.begin(), constXcosNames.end(), attribute);
                if (found == constXcosNames.end())
                {
                    continue;
                }
                enum xcosNames current = static_cast<enum xcosNames>(std::distance(constXcosNames.begin(), found));
                switch (current)
                {
                    case e_version:
                    {
                        const xmlChar* version = xmlTextReaderConstValue(reader);
                        if (xmlStrcmp(version, (const xmlChar*)"1.0") != 0)
                        {
                            sciprint("unable to decode ParameterSet version\n");
                            return -1;
                        }
                        break;
                    }
                    case e_name:
                    {
                        // the name is ignored, only one ParameterSet is handled
                        break;
                    }

                    default:
                        break;
                }
            }
            break;
        }

        default:
            sciprint("unable to decode ParameterSet\n");
            return -1;
    }

    return 1;
}

int SSPResource::loadParameter(xmlTextReaderPtr reader, model::BaseObject* o)
{
    assert(o->kind() == BLOCK || o->kind() == DIAGRAM);

    switch (o->kind())
    {
        case BLOCK: // fallthrough
        case DIAGRAM:
        {
            // first allocate a new "undefined" parameter
            std::vector<std::string> names;
            if (!controller.getObjectProperty(o, PARAMETER_NAME, names))
            {
                sciprint("unable to retrieve Parameter name\n");
                return -1;
            }
            names.push_back("");
            if (controller.setObjectProperty(o, PARAMETER_NAME, names) == FAIL)
            {
                sciprint("unable to assign Parameter name\n");
                return -1;
            }

            std::vector<std::string> descriptions;
            if (!controller.getObjectProperty(o, PARAMETER_DESCRIPTION, descriptions))
            {
                sciprint("unable to retrieve Parameter description\n");
                return -1;
            }

            // iterate on attributes
            for (int rc = xmlTextReaderMoveToFirstAttribute(reader); rc > 0; rc = xmlTextReaderMoveToNextAttribute(reader))
            {
                const xmlChar* attribute = xmlTextReaderConstName(reader);
                auto found = std::find(constXcosNames.begin(), constXcosNames.end(), attribute);
                if (found == constXcosNames.end())
                {
                    continue;
                }
                enum xcosNames current = static_cast<enum xcosNames>(std::distance(constXcosNames.begin(), found));
                switch (current)
                {
                    case e_name:
                    {
                        names.back() = to_string(xmlTextReaderConstValue(reader));
                        if (controller.setObjectProperty(o, PARAMETER_NAME, names) == FAIL)
                        {
                            sciprint("unable to assign Parameter name\n");
                            return -1;
                        }
                        break;
                    }
                    case e_description:
                    {
                        descriptions.back() = to_string(xmlTextReaderConstValue(reader));
                        if (controller.setObjectProperty(o, PARAMETER_DESCRIPTION, descriptions) == FAIL)
                        {
                            sciprint("unable to assign Parameter name\n");
                            return -1;
                        }
                        break;
                    }

                    default:
                        break;
                }
            }
            break;
        }

        default:
            sciprint("unable to decode Parameter\n");
            return -1;
    }

    return 1;
}

int SSPResource::loadUnit(xmlTextReaderPtr reader, model::BaseObject* o)
{
    assert(o->kind() == BLOCK || o->kind() == DIAGRAM);

    // allocate a new temporary unit
    unit = model::Unit();

    for (int rc = xmlTextReaderMoveToFirstAttribute(reader); rc > 0; rc = xmlTextReaderMoveToNextAttribute(reader))
    {
        const xmlChar* attribute = xmlTextReaderConstName(reader);
        auto found = std::find(constXcosNames.begin(), constXcosNames.end(), attribute);
        if (found == constXcosNames.end())
        {
            continue;
        }
        enum xcosNames current = static_cast<enum xcosNames>(std::distance(constXcosNames.begin(), found));
        switch (current)
        {
            case e_name:
            {
                unit.name = to_string(xmlTextReaderConstValue(reader));
                break;
            }
            case e_description:
            {
                unit.description = to_string(xmlTextReaderConstValue(reader));
                break;
            }

            default:
                sciprint("unable to decode Unit\n");
                return -1;
        }
    }

    return 1;
}

int SSPResource::loadBaseUnit(xmlTextReaderPtr reader, model::BaseObject* o)
{
    assert(o->kind() == BLOCK || o->kind() == DIAGRAM);

    // decode the temporary unit, iterate on attributes
    for (int rc = xmlTextReaderMoveToFirstAttribute(reader); rc > 0; rc = xmlTextReaderMoveToNextAttribute(reader))
    {
        const xmlChar* attribute = xmlTextReaderConstName(reader);
        auto found = std::find(constXcosNames.begin(), constXcosNames.end(), attribute);
        if (found == constXcosNames.end())
        {
            continue;
        }
        enum xcosNames current = static_cast<enum xcosNames>(std::distance(constXcosNames.begin(), found));
        switch (current)
        {
            case e_kg:
            {
                unit.kg = to_double(xmlTextReaderConstValue(reader));
                break;
            }
            case e_m:
            {
                unit.m = to_double(xmlTextReaderConstValue(reader));
                break;
            }
            case e_s:
            {
                unit.s = to_double(xmlTextReaderConstValue(reader));
                break;
            }
            case e_A:
            {
                unit.A = to_double(xmlTextReaderConstValue(reader));
                break;
            }
            case e_K:
            {
                unit.K = to_double(xmlTextReaderConstValue(reader));
                break;
            }
            case e_mol:
            {
                unit.mol = to_double(xmlTextReaderConstValue(reader));
                break;
            }
            case e_cd:
            {
                unit.cd = to_double(xmlTextReaderConstValue(reader));
                break;
            }
            case e_rad:
            {
                unit.rad = to_double(xmlTextReaderConstValue(reader));
                break;
            }
            case e_factor:
            {
                unit.factor = to_double(xmlTextReaderConstValue(reader));
                break;
            }
            case e_offset:
            {
                unit.offset = to_double(xmlTextReaderConstValue(reader));
                break;
            }
            default:
                sciprint("unable to decode BaseUnit\n");
                return -1;
        }
    }

    // assign the unit to tha default datatype on the correct layer
    if (o->kind() == BLOCK)
    {
        model::Block* block = (model::Block*)o;
        model::Datatype d;

        // the datatype with a unit is scalar double
        d.m_datatype_id = 1;
        d.m_rows = 1;
        d.m_columns = 1;

        // assign the temporary unit
        d.m_unit = unit;

        std::vector<model::Datatype*> datatypes = block->getDatatypes();
        datatypes.push_back(controller.getInternalModel().flyweight(d));
        block->setDatatypes(datatypes);
    }
    else if (o->kind() == DIAGRAM)
    {
        model::Diagram* diagram = (model::Diagram*)o;
        model::Datatype d;

        // the datatype with a unit is scalar double
        d.m_datatype_id = 1;
        d.m_rows = 1;
        d.m_columns = 1;

        // assign the temporary unit
        d.m_unit = unit;

        std::vector<model::Datatype*> datatypes = diagram->getDatatypes();
        datatypes.push_back(controller.getInternalModel().flyweight(d));
        diagram->setDatatypes(datatypes);
    }

    return 1;
}

int SSPResource::loadAnnotation(xmlTextReaderPtr reader, model::BaseObject* o)
{
    // iterate on attributes
    for (int rc = xmlTextReaderMoveToFirstAttribute(reader); rc > 0; rc = xmlTextReaderMoveToNextAttribute(reader))
    {
        const xmlChar* attribute = xmlTextReaderConstName(reader);
        auto found = std::find(constXcosNames.begin(), constXcosNames.end(), attribute);
        if (found == constXcosNames.end())
        {
            continue;
        }
        enum xcosNames current = static_cast<enum xcosNames>(std::distance(constXcosNames.begin(), found));
        switch (current)
        {

            case e_type:
            {
                // only xcos URI is managed here
                const xmlChar* type = xmlTextReaderConstString(reader, xmlTextReaderConstValue(reader));
                if (type == xcosNamespaceUri)
                {
                    // TODO: decode data
                }
                else
                {
                    annotated = true;
                }
            }

            default:
                // ignore other parameters
                break;
        }
    }

    return 1;
}

int SSPResource::loadComponent(xmlTextReaderPtr reader, model::BaseObject* o)
{
    // iterate on attributes
    for (int rc = xmlTextReaderMoveToFirstAttribute(reader); rc > 0; rc = xmlTextReaderMoveToNextAttribute(reader))
    {
        const xmlChar* attribute = xmlTextReaderConstName(reader);
        auto found = std::find(constXcosNames.begin(), constXcosNames.end(), attribute);
        if (found == constXcosNames.end())
        {
            continue;
        }
        enum xcosNames current = static_cast<enum xcosNames>(std::distance(constXcosNames.begin(), found));
        switch (current)
        {

            case e_type:
            {
                const xmlChar* type = xmlTextReaderConstString(reader, xmlTextReaderConstName(reader));
                auto found = std::find(constXcosNames.begin(), constXcosNames.end(), type);
                if (found == constXcosNames.end())
                {
                    continue;
                }
                enum xcosNames t = static_cast<enum xcosNames>(std::distance(constXcosNames.begin(), found));
                switch (t)
                {
                    case e_application_x_fmu_sharedlibrary: // fallthrough
                    default:
                    {
                        controller.setObjectProperty(o, INTERFACE_FUNCTION, std::string("SimpleFMU"));
                        break;
                    }

                    case e_application_x_ssp_definition:
                    {
                        sciprint("application/x-ssp-definition is not supported\n");
                        return -1;
                        break;
                    }

                    case e_application_x_ssp_package:
                    {
                        sciprint("application/x-ssp-package is not supported\n");
                        return -1;
                        break;
                    }
                }
                break;
            }

            case e_source:
            {
                std::string source = to_string(xmlTextReaderConstValue(reader));
                
                const std::string fullPathname = std::string("TMPDIR/") + source;
                const std::string resourcesPathname = source.substr(strlen("resources/"));

                auto pFullPathname = std::filesystem::path(fullPathname);
                std::string workdir = pFullPathname.replace_filename(pFullPathname.stem()).string();

                // create the directories
                char* pStrWorkdir = expandPathVariable(workdir.c_str());
                auto pPathWorkdir = std::filesystem::path(pStrWorkdir);
                FREE(pStrWorkdir);
                std::filesystem::create_directories(pPathWorkdir.parent_path());
                std::filesystem::create_directories(pPathWorkdir);

                std::vector<std::string> v = {resourcesPathname, workdir};
                controller.setObjectProperty(o, EXPRS, v);
                break;
            }

            case e_implementation:
            {
                // the implementation is mapped to a specific simulation function
                const xmlChar* implementation = xmlTextReaderConstString(reader, xmlTextReaderConstValue(reader));
                std::string functionName;
                auto found = std::find(constXcosNames.begin(), constXcosNames.end(), implementation);
                if (found == constXcosNames.end())
                {
                    continue;
                }
                enum xcosNames impl = static_cast<enum xcosNames>(std::distance(constXcosNames.begin(), found));
                switch (impl)
                {
                case e_ModelExchange:
                    functionName = "fmu2_simulate_me";
                    break;
                case e_CoSimulation:
                    functionName = "fmu2_simulate_cs";
                    break;
                case e_any:
                    break;
                default:
                    sciprint("Component implementation \"%s\" is not supported\n", xmlTextReaderConstValue(reader));
                    break;
                }
                if (controller.setObjectProperty(o, SIM_FUNCTION_NAME, functionName) == FAIL)
                {
                    return -1;
                }
                if (controller.setObjectProperty(o, SIM_FUNCTION_API, 4) == FAIL)
                {
                    return -1;
                }
                break;
            }

            case e_name:
            {
                std::string name = to_string(xmlTextReaderConstValue(reader));
                if (controller.setObjectProperty(o, NAME, name) == FAIL)
                {
                    return -1;
                }
                break;
            }

            case e_description:
            {
                std::string description = to_string(xmlTextReaderConstValue(reader));
                if (controller.setObjectProperty(o, DESCRIPTION, description) == FAIL)
                {
                    return -1;
                }
                break;
            }

            default:
                // ignore other parameters
                break;
        }
    }

    return 1;
}

int SSPResource::processNode(xmlTextReaderPtr reader)
{
    // manage only xcos related XML nodes
    const xmlChar* nsURI = xmlTextReaderConstNamespaceUri(reader);
    if (nsURI == xmlnsSSC || nsURI == xmlnsSSB || nsURI == xmlnsSSD || nsURI == xmlnsSSV || nsURI == xmlnsSSM || nsURI == nullptr)
    {
        xmlReaderTypes nodeType = (xmlReaderTypes)xmlTextReaderNodeType(reader);
        switch (nodeType)
        {
            case XML_READER_TYPE_NONE:
                return 1;
            case XML_READER_TYPE_ELEMENT:
                return processElement(reader);
            case XML_READER_TYPE_ATTRIBUTE:
                sciprint("xmlReader attributes node not supported\n");
                return -1;
            case XML_READER_TYPE_TEXT:
                return processText(reader);
            case XML_READER_TYPE_CDATA:
                return processText(reader);
            case XML_READER_TYPE_ENTITY_REFERENCE:
                sciprint("xmlReader entity reference not supported\n");
                return -1;
            case XML_READER_TYPE_ENTITY:
                sciprint("xmlReader entity not supported\n");
                return -1;
            case XML_READER_TYPE_PROCESSING_INSTRUCTION:
                sciprint("xmlReader processing instruction not supported\n");
                return -1;
            case XML_READER_TYPE_COMMENT:
                return 1;
            case XML_READER_TYPE_DOCUMENT:
                return 1;
            case XML_READER_TYPE_DOCUMENT_TYPE:
                sciprint("xmlReader document type not supported\n");
                return -1;
            case XML_READER_TYPE_DOCUMENT_FRAGMENT:
                sciprint("xmlReader document fragment not supported\n");
                return -1;
            case XML_READER_TYPE_NOTATION:
                sciprint("xmlReader notation not supported\n");
                return -1;
            case XML_READER_TYPE_WHITESPACE:
                sciprint("xmlReader whitespace not supported\n");
                return -1;
            case XML_READER_TYPE_SIGNIFICANT_WHITESPACE:
                return 1; // ignore indent or end-of-line
            case XML_READER_TYPE_END_ELEMENT:
                return processEndElement(reader);
            case XML_READER_TYPE_END_ENTITY:
                sciprint("xmlReader end entity not supported\n");
                return -1;
            case XML_READER_TYPE_XML_DECLARATION:
                sciprint("xmlReader XML declaration not supported\n");
                return -1;
        }
    }
    else
    {
        // TODO mixed model should be preserved in some way and restored back on SSPResource_save.cpp .
        if (annotated)
            return 1;
    }
    sciprint("unable to process node\n");
    return -1;
}

static std::string interface_function(enum portKind kind, bool isImplicit, bool isMainDiagram)
{
    std::string interfaceBlock[] = {"", "OUT_f", "IN_f", "CLKOUTV_f", "CLKINV_f"};

    if (isImplicit)
    {
        interfaceBlock[PORT_IN] = "INIMPL_f";
        interfaceBlock[PORT_OUT] = "OUTIMPL_f";
    }

    if (isMainDiagram)
    {
        // corner case, this is implemented as fake subsystem
        interfaceBlock[PORT_IN] = "SSPOutputConnector";
        interfaceBlock[PORT_OUT] = "SSPInputConnector";
    }

    return interfaceBlock[kind];
}

static std::string simulation_function(enum portKind kind, bool isImplicit, bool isMainDiagram)
{
    std::string simulationFunction[] = {"", "output", "input", "output", "input"};

    if (isImplicit)
    {
        simulationFunction[PORT_OUT] = "outimpl";
        simulationFunction[PORT_IN] = "inimpl";
    }

    if (isMainDiagram)
    {
        // corner case, this is implemented as fake subsystem
        simulationFunction[PORT_IN] = "csuper";
        simulationFunction[PORT_OUT] = "csuper";
    }

    return simulationFunction[kind];
}

// assign port (if applicable) and compute its index
void SSPResource::assignPortIndexes(model::BaseObject* parent)
{
    const enum portKind opposite[] = {PORT_UNDEF, PORT_OUT, PORT_IN, PORT_EOUT, PORT_EIN};

    std::map<portKind, std::vector<Reference*>> ports;
    for (portKind kind : {PORT_IN, PORT_OUT, PORT_EIN, PORT_EOUT})
        ports[kind] = std::vector<Reference*>();

    std::map<portKind, std::vector<Reference*>> ioBlocks;
    for (portKind kind : {PORT_IN, PORT_OUT, PORT_EIN, PORT_EOUT})
        ioBlocks[kind] = std::vector<Reference*>();

    // look for all Component's connectors
    std::vector<Reference>::reverse_iterator it = references.rbegin();
    std::string element = it->element;
    while (it != references.rend() && it->element != "" && it->element == element && it->block == parent)
    {
        int kind = PORT_UNDEF;
        controller.getObjectProperty(it->port, PORT_KIND, kind);
        ports[(portKind)kind].push_back(&*it);

        it++;
    }
    // look for all System's connectors
    std::vector<Reference>::reverse_iterator ioBlock = references.rbegin();
    std::vector<Reference>::reverse_iterator outter = ioBlock + 1;
    while (outter != references.rend() && outter->connector == ioBlock->connector)
    {
        int kind = PORT_UNDEF;
        controller.getObjectProperty(outter->port, PORT_KIND, kind);

        ioBlocks[(portKind)kind].push_back(&*ioBlock);
        ports[(portKind)kind].push_back(&*outter);

        outter += 2;
        ioBlock += 2;
    }
    // add all main System's connectors
    if (parent->kind() == DIAGRAM)
    {
        for (std::vector<Reference>::iterator ioBlock = references.begin(); ioBlock != references.end() && ioBlock->element == ""; ++ioBlock)
        {
            int kind = PORT_UNDEF;
            controller.getObjectProperty(ioBlock->port, PORT_KIND, kind);
            ioBlocks[opposite[kind]].push_back(&*ioBlock);
        }
    }

    // sort per reference position
    for (portKind kind : {PORT_IN, PORT_OUT, PORT_EIN, PORT_EOUT})
    {
        auto cmp = [this](const Reference* a, const Reference* b)
        {
            if (std::fabs(a->x - b->x) <= std::numeric_limits<double>::epsilon())
                return a->y > b->y;
            return a->x > b->x;
        };
        std::stable_sort(ports[(portKind)kind].begin(), ports[(portKind)kind].end(), cmp);
        std::stable_sort(ioBlocks[(portKind)kind].begin(), ioBlocks[(portKind)kind].end(), cmp);
    }

    for (portKind kind : {PORT_IN, PORT_OUT, PORT_EIN, PORT_EOUT})
    {
        object_properties_t p = property_from_port(kind);

        // compute the port number
        if (!ports[(portKind)kind].empty()) // layer or Component have outter ports
        {
            std::vector<ScicosID> portsID;
            portsID.reserve(ports[(portKind)kind].size());
            for (const Reference* r : ports[(portKind)kind])
            {
                portsID.push_back(r->port->id());
            }
            controller.setObjectProperty(parent, p, portsID);
        }

        // layer computes its port number from its already decoded children
        for (int i = 0; i < ioBlocks[(portKind)kind].size(); ++i)
        {
            model::BaseObject* innerBlock = ioBlocks[(portKind)kind][i]->block;
            int index = i + 1;

            if (parent->kind() == DIAGRAM)
            {
                // on an SSPInputConnector or SSPOutputConnector
                std::vector<std::string> exprs;
                if (kind == PORT_IN)
                {
                    exprs = { ioBlocks[(portKind)kind][i]->connector, "256"};
                }
                else
                {
                    exprs = { ioBlocks[(portKind)kind][i]->connector};
                }
                
                controller.setObjectProperty(innerBlock, EXPRS, exprs);
                std::vector<int> ipar = { 1 };
                controller.setObjectProperty(innerBlock, IPAR, ipar);
            }
            else
            {
                // on a subsystem input/output
                std::vector<int> ipar = {(int)index};
                controller.setObjectProperty(innerBlock, IPAR, index);
                controller.setObjectProperty(innerBlock, EXPRS, std::to_string(index));
            }
        }
    }
}

int SSPResource::processElement(xmlTextReaderPtr reader)
{
    const xmlChar* name = xmlTextReaderConstLocalName(reader);
    const char* kind_str[] = {
        "BLOCK",
        "DIAGRAM",
        "LINK",
        "ANNOTATION",
        "PORT"};

    // ignore elements within annotation
    if (annotated)
    {
        return 1;
    }

    // verbose for debugging
//#ifndef NDEBUG
//    xmlNodePtr curNode = xmlTextReaderCurrentNode(reader);
//    int line = xmlGetLineNo(curNode);
//    sciprint("parsing line %d\n", line);
//#endif

    // lookup for known node names
    // thanks to the string intern-ing, the pointer comparison could be used
    auto found = std::find(constXcosNames.begin(), constXcosNames.end(), name);
    if (found == constXcosNames.end())
    {
        sciprint("Unknown \"%s\" element name\n", name);
        return -1;
    }
    enum xcosNames current = static_cast<enum xcosNames>(std::distance(constXcosNames.begin(), found));
    switch (current)
    {
        case e_SystemStructureDescription:
        {
            // the root diagram should be decoded
            model::BaseObject* o = controller.getBaseObject(root);
            processed = {o};

            return loadSystemStructureDescription(reader, o);
        }
        case e_DefaultExperiment:
        {
            return loadDefaultExperiment(reader, processed.back());
        }
        case e_System:
        {
            if (processed.size() == 1)
            {
                // this is the main diagram, resolve it
                processed.push_back(processed.back());
            }
            else
            {
                // this is a child of a diagram, create it
                model::BaseObject* parent = processed.back();
                model::BaseObject* o = controller.createBaseObject(BLOCK);
                processed.push_back(o);

                // assign the child
                controller.setObjectProperty(o, PARENT_DIAGRAM, root);
                if (parent->kind() == BLOCK)
                {
                    controller.setObjectProperty(o, PARENT_BLOCK, parent->id());
                }
                std::vector<ScicosID> children;
                controller.getObjectProperty(parent, CHILDREN, children);
                children.push_back(o->id());
                controller.setObjectProperty(parent, CHILDREN, children);

                controller.setObjectProperty(o, INTERFACE_FUNCTION, std::string("SUPER_f"));
            }
            return loadSystem(reader, processed.back());
        }
        case e_Connectors:
            processed.push_back(processed.back());
            break;
        case e_Connector:
        {
            int ret;
            model::BaseObject* innerBlock = nullptr;
            model::BaseObject* innerPort = nullptr;
            model::BaseObject* outterPort = nullptr;
            model::BaseObject* port = nullptr;
            model::BaseObject* parent = processed.back();

            bool isMainDiagram = parent->kind() == DIAGRAM;
            bool isLayer;
            {
                std::string interfaceFunction;
                controller.getObjectProperty(parent, INTERFACE_FUNCTION, interfaceFunction);
                isLayer = interfaceFunction == "SUPER_f";
            }

            if (isLayer)
            {
                innerBlock = controller.createBaseObject(BLOCK);
                controller.setObjectProperty(innerBlock, PARENT_DIAGRAM, root);
                controller.setObjectProperty(innerBlock, PARENT_BLOCK, parent->id());
                innerPort = controller.createBaseObject(PORT);
                controller.setObjectProperty(innerPort, SOURCE_BLOCK, innerBlock);

                std::vector<ScicosID> children;
                controller.getObjectProperty(parent, CHILDREN, children);
                children.push_back(innerBlock->id());
                controller.setObjectProperty(parent, CHILDREN, children);

                outterPort = controller.createBaseObject(PORT);
                controller.setObjectProperty(outterPort, SOURCE_BLOCK, parent);

                references.emplace_back(Reference{"", "", 0, 0, parent, outterPort});
                references.emplace_back(Reference{"", "", 0, 0, innerBlock, innerPort});

                processed.push_back(outterPort);
                port = outterPort;
            }
            else if (isMainDiagram)
            {
                innerBlock = controller.createBaseObject(BLOCK);
                controller.setObjectProperty(innerBlock, PARENT_DIAGRAM, root);
                innerPort = controller.createBaseObject(PORT);
                controller.setObjectProperty(innerPort, SOURCE_BLOCK, innerBlock);

                std::vector<ScicosID> children;
                controller.getObjectProperty(parent, CHILDREN, children);
                children.push_back(innerBlock->id());
                controller.setObjectProperty(parent, CHILDREN, children);

                references.emplace_back(Reference{"", "", 0, 0, innerBlock, innerPort});

                processed.push_back(innerPort);
                port = innerPort;
            }
            else // a regular FMU block
            {
                outterPort = controller.createBaseObject(PORT);
                controller.setObjectProperty(outterPort, SOURCE_BLOCK, parent);

                std::string parentName;
                controller.getObjectProperty(parent, NAME, parentName);

                references.emplace_back(Reference{parentName, "", 0, 0, parent, outterPort});

                processed.push_back(outterPort);
                port = outterPort;
            }

            ret = loadConnector(reader, port);
            if (ret != 1)
            {
                sciprint("unable to set Connector\n");
                return ret;
            }

            enum portKind kind;
            controller.getObjectProperty(port, PORT_KIND, (int&)kind);
            bool isImplicit;
            controller.getObjectProperty(port, IMPLICIT, isImplicit);

            const enum portKind opposite[] = {PORT_UNDEF, PORT_OUT, PORT_IN, PORT_EOUT, PORT_EIN};
            if (innerPort != nullptr && innerPort != port)
            {
                // sync port information to the inner block
                copy_property<bool>(port, innerPort, IMPLICIT);
                if (controller.setObjectProperty(innerPort, PORT_KIND, opposite[kind]) == FAIL)
                {
                    sciprint("unable to set Connector on innerPort\n");
                    return -1;
                }
            }

            // set innerBlock data
            if (innerBlock != nullptr)
            {
                portKind innerKind = opposite[kind];
                std::vector<ScicosID> ports = {innerPort->id()};
                controller.setObjectProperty(innerBlock, property_from_port(innerKind), ports);

                // this is the connector block, loaded as I/O block inside the inner layer
                copy_property<std::string>(port, innerBlock, NAME);
                copy_property<std::string>(port, innerBlock, DESCRIPTION);

                controller.setObjectProperty(innerBlock, INTERFACE_FUNCTION, interface_function(innerKind, isImplicit, isMainDiagram));
                controller.setObjectProperty(innerBlock, SIM_FUNCTION_NAME, simulation_function(innerKind, isImplicit, isMainDiagram));
                controller.setObjectProperty(innerBlock, STYLE, interface_function(innerKind, isImplicit, isMainDiagram));
            }

            // refresh the outter connector to connect it later
            if (isLayer)
            {
                std::string portName;
                controller.getObjectProperty(outterPort, NAME, portName);
                std::string layerName;
                controller.getObjectProperty(parent, NAME, layerName);
                std::vector<Reference>::reverse_iterator outter = references.rbegin() + 1;
                outter->element = layerName;
                outter->connector = portName;
            }

            return ret;
        }
        case e_Real:
            return loadReal(reader, processed.back());
        case e_Integer:
            return loadInteger(reader, processed.back());
        case e_Boolean:
            return loadBoolean(reader, processed.back());
        case e_String:
            return loadString(reader, processed.back());
        case e_Enumeration:
            return loadEnumeration(reader, processed.back());
        case e_Binary:
            return loadBinary(reader, processed.back());
        case e_SystemGeometry:
            // SystemGeometry is used to relocate connectors with absolute coordinates
            return loadSystemGeometry(reader, processed.back());
        case e_ElementGeometry:
            return loadElementGeometry(reader, processed.back());
        case e_ConnectorGeometry:
            return loadConnectorGeometry(reader, processed.back());
        case e_ConnectionGeometry:
            // geometry is used for rectangle coordinates of its parent
            return loadConnectionGeometry(reader, processed.back());
        case e_Connections:
            processed.push_back(processed.back());
            break;
        case e_Connection:
            return loadConnection(reader, processed.back());
        case e_Units:
            processed.push_back(processed.back());
            break;
        case e_Unit:
            processed.push_back(processed.back());
            return loadUnit(reader, processed.back());
        case e_BaseUnit:
            return loadBaseUnit(reader, processed.back());
        case e_Annotations:
            processed.push_back(processed.back());
            break;
        case e_ParameterBindings:
            processed.push_back(processed.back());
            break;
        case e_ParameterBinding:
            processed.push_back(processed.back());
            break;
        case e_ParameterValues:
            processed.push_back(processed.back());
            break;
        case e_ParameterSet:
            processed.push_back(processed.back());
            return loadParameterSet(reader, processed.back());
        case e_Parameters:
            // store into processed if there is children
            if (!xmlTextReaderIsEmptyElement(reader))
            {
                processed.push_back(processed.back());
            }
            break;
        case e_Parameter:
            processed.push_back(processed.back());
            return loadParameter(reader, processed.back());
        case e_Annotation:
            return loadAnnotation(reader, processed.back());
        case e_Elements:
            processed.push_back(processed.back());
            break;
        case e_Component:
        {
            model::BaseObject* o = controller.createBaseObject(BLOCK);
            model::BaseObject* parent = processed.back();
            processed.push_back(o);

            controller.setObjectProperty(o, PARENT_DIAGRAM, root);
            if (parent->kind() == BLOCK)
            {
                controller.setObjectProperty(o, PARENT_BLOCK, parent->id());
            }
            std::vector<ScicosID> children;
            controller.getObjectProperty(parent, CHILDREN, children);
            children.push_back(o->id());
            controller.setObjectProperty(parent, CHILDREN, children);

            return loadComponent(reader, o);
        }

        default:
            // ignore annotations
            if (annotated)
            {
                return 1;
            }

            sciprint("Unknown \"%s\" element name\n", name);
            return -1;
    }

    return 1;
}

int SSPResource::processText(xmlTextReaderPtr reader)
{
    int ret;

    sciprint("Unable to decode text value\n");
    ret = -1;

    return ret;
}

int SSPResource::processEndElement(xmlTextReaderPtr reader)
{
    const xmlChar* name = xmlTextReaderConstLocalName(reader);
    const char* kind_str[] = {
        "BLOCK",
        "DIAGRAM",
        "LINK",
        "ANNOTATION",
        "PORT"};
    
    // ignore annotations
    if (annotated)
    {
        annotated = false;
        return 1;
    }

    // on System (eg. layer) ending, clear all references
    if (xmlStrEqual(name, (const xmlChar*)"System"))
    {
        // drop the Component ports
        while (!references.empty() && references.back().element != "")
        {
            references.pop_back();
        }
        // drop the IOBlockss
        {
            // look for the end of the layer
            std::vector<Reference>::reverse_iterator ioBlock = references.rbegin();
            std::vector<Reference>::reverse_iterator outter = ioBlock + 1;
            while (outter != references.rend() && outter->connector == ioBlock->connector)
            {
                ioBlock += 2;
                outter += 2;
            }
            // erase all IOBlocks
            references.erase(std::remove_if(ioBlock.base(), references.end(), [](const Reference& r) { return r.element == ""; }), references.end());
        }

    }

    processed.pop_back();
    return 1;
}

} // namespace org_scilab_modules_scicos
