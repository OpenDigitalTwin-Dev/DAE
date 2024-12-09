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

#include "utilities.hxx"
#include "Controller.hxx"
#include "model/BaseObject.hxx"

#include "dynlib_scicos.h"

#include <map>
#include <array>
#include <string>
#include <vector>

extern "C" {
#include <archive.h>
#include <archive_entry.h>

#include <libxml/xmlwriter.h>
#include <libxml/xmlreader.h>
}

#define BLOCK_SIZE 10240

namespace org_scilab_modules_scicos
{

class SCICOS_IMPEXP SSPResource
{

public:
    SSPResource(ScicosID id);
    ~SSPResource();

    int save(const char* uri);
    int load(const char* uri);

private:
    /*
     * Save helpers
     */
    int writeDatatype(xmlTextWriterPtr writer, const std::vector<int>& datatype);
    int writePoint(xmlTextWriterPtr writer, double x, double y);
    int writeGeometry(xmlTextWriterPtr writer, ScicosID id, kind_t kind);
    int writeAbstractLayer(xmlTextWriterPtr writer, ScicosID id, kind_t kind);
    int writeAbstractBaseObject(xmlTextWriterPtr writer, ScicosID id, kind_t kind);
    int writeDiagram(xmlTextWriterPtr writer);
    int writeSimulationConfig(xmlTextWriterPtr writer, ScicosID id);
    int writeBlock(xmlTextWriterPtr writer, ScicosID id);
    int writePort(xmlTextWriterPtr writer, enum object_properties_t container, ScicosID id);
    int writeLink(xmlTextWriterPtr writer, ScicosID id);
    int writeAnnotation(xmlTextWriterPtr writer, ScicosID id, bool isLabel);

    /*
     * Load helpers
     */
    int loadSystemStructureDescription(xmlTextReaderPtr reader, model::BaseObject* o);
    int loadDefaultExperiment(xmlTextReaderPtr reader, model::BaseObject* o);
    int loadSystem(xmlTextReaderPtr reader, model::BaseObject* o);
    int loadComponent(xmlTextReaderPtr reader, model::BaseObject* o);
    int loadConnector(xmlTextReaderPtr reader, model::BaseObject* o);
    int loadParameterSet(xmlTextReaderPtr reader, model::BaseObject* o);
    int loadParameter(xmlTextReaderPtr reader, model::BaseObject* o);
    int loadUnit(xmlTextReaderPtr reader, model::BaseObject* o);
    int loadBaseUnit(xmlTextReaderPtr reader, model::BaseObject* o);
    int loadConnection(xmlTextReaderPtr reader, model::BaseObject* o);

    int loadReal(xmlTextReaderPtr reader, model::BaseObject* o);
    int loadInteger(xmlTextReaderPtr reader, model::BaseObject* o);
    int loadBoolean(xmlTextReaderPtr reader, model::BaseObject* o);
    int loadString(xmlTextReaderPtr reader, model::BaseObject* o);
    int loadEnumeration(xmlTextReaderPtr reader, model::BaseObject* o);
    int loadBinary(xmlTextReaderPtr reader, model::BaseObject* o);
    
    int loadSystemGeometry(xmlTextReaderPtr reader, model::BaseObject* o);
    int loadConnectorGeometry(xmlTextReaderPtr reader, model::BaseObject* o);
    int loadElementGeometry(xmlTextReaderPtr reader, model::BaseObject* o);
    int loadConnectionGeometry(xmlTextReaderPtr reader, model::BaseObject* o);

    int loadAnnotation(xmlTextReaderPtr reader, model::BaseObject* o);

    int processNode(xmlTextReaderPtr reader);
    int processElement(xmlTextReaderPtr reader);
    int processText(xmlTextReaderPtr reader);
    int processEndElement(xmlTextReaderPtr reader);

    // intern known string to speedup and simplify comparaison 
    void internPredefinedStrings(xmlTextReaderPtr reader);
    template<typename T> inline
    update_status_t copy_property(model::BaseObject* src, model::BaseObject* dest, object_properties_t prop)
    {
        T v;
        controller.getObjectProperty(src, prop, v);
        return controller.setObjectProperty(dest, prop, v);
    }
    template<typename T> inline
    update_status_t copy_property(model::BaseObject* src, object_properties_t src_prop, model::BaseObject* dest, object_properties_t dest_prop)
    {
        T v;
        controller.getObjectProperty(src, src_prop, v);
        return controller.setObjectProperty(dest, dest_prop, v);
    }
    void assignPortIndexes(model::BaseObject* parent);

private:
    /* shared controller */
    Controller controller;
    /* id of the diagram */
    ScicosID root;

    /*
     * load shared variables
     */

    /* interned string indexes */
    enum xcosNames
    {
        e_A,
        e_Annotation,
        e_Annotations,
        e_acausal,
        e_any,
        e_application_x_fmu_sharedlibrary,
        e_application_x_scilab_xcos,
        e_application_x_ssp_definition,
        e_application_x_ssp_package,
        e_text_x_modelica,
        e_author,
        e_BaseUnit,
        e_Binary,
        e_Boolean,
        e_BooleanMappingTransformation,
        e_calculatedParameter,
        e_cd,
        e_component,
        e_Component,
        e_Connection,
        e_ConnectionGeometry,
        e_Connections,
        e_Connector,
        e_ConnectorGeometry,
        e_Connectors,
        e_copyright,
        e_CoSimulation,
        e_DefaultExperiment,
        e_description,
        e_dictionary,
        e_DictionaryEntry,
        e_ElementGeometry,
        e_Elements,
        e_endConnector,
        e_endElement,
        e_Enumeration,
        e_EnumerationMappingTransformation,
        e_Enumerations,
        e_factor,
        e_fileversion,
        e_generationDateAndTime,
        e_generationTool,
        e_GraphicalElements,
        e_iconFixedAspectRatio,
        e_iconFlip,
        e_iconRotation,
        e_iconSource,
        e_id,
        e_implementation,
        e_inout,
        e_input,
        e_Integer,
        e_IntegerMappingTransformation,
        e_Item,
        e_K,
        e_kg,
        e_kind,
        e_license,
        e_LinearTransformation,
        e_m,
        e_MapEntry,
        e_MappingEntry,
        e_mime_type,
        e_ModelExchange,
        e_mol,
        e_name,
        e_Note,
        e_offset,
        e_output,
        e_parameter,
        e_Parameter,
        e_ParameterBinding,
        e_ParameterBindings,
        e_ParameterMapping,
        e_Parameters,
        e_ParameterSet,
        e_ParameterValues,
        e_pointsX,
        e_pointsY,
        e_prefix,
        e_rad,
        e_Real,
        e_rotation,
        e_s,
        e_SignalDictionaries,
        e_SignalDictionary,
        e_SignalDictionaryReference,
        e_source,
        e_sourceBase,
        e_SSD,
        e_startConnector,
        e_startElement,
        e_startTime,
        e_stopTime,
        e_String,
        e_suppressUnitConversion,
        e_System,
        e_SystemGeometry,
        e_SystemStructureDescription,
        e_target,
        e_text,
        e_type,
        e_unit,
        e_Unit,
        e_Units,
        e_value,
        e_version,
        e_x,
        e_x1,
        e_x2,
        e_y,
        e_y1,
        e_y2,
        NB_XCOS_NAMES
    };
    /* temporary interned string content (owned by a reader) */
    std::array<const xmlChar*, NB_XCOS_NAMES> constXcosNames;
    /* temporary interned xcos namespace (owned by a reader) */
    const xmlChar* xcosNamespaceUri;
    /* temporary interned xsi namespace (owned by a reader) */
    const xmlChar* xmlnsSSC;
    const xmlChar* xmlnsSSB;
    const xmlChar* xmlnsSSD;
    const xmlChar* xmlnsSSV;
    const xmlChar* xmlnsSSM;

    /* stack uid / kind used */
    std::vector<model::BaseObject*> processed;

    /* if true, the node is within an Annotation block*/
    bool annotated;

    /* temporary storage, corrently decoded Unit*/
    model::Unit unit;

    /* uid string - ScicosID  map */
    struct Reference
    {
        std::string element;
        std::string connector;
        
        double x;
        double y;

        model::BaseObject* block;
        model::BaseObject* port;

        inline
        bool operator==(const Reference& other) const {
            return (element == other.element) && (connector == other.connector);
        }
    };
    std::vector<Reference> references;

    // Aspect ratio for visual compatibility between Dymola, EasySSP and Xcos
    // from "System Structure and Parameterization 1.0.1":
    // When importing or exporting systems, the nominal unit of the coordinates is 1 mm for all axis. The
    // nominal unit is intended to ensure similar visual sizing and appearances when combining systems from
    // different implementations
    const double ASPECT_RATIO = 1; // set to 1, was 3. with Dymola
};

}

