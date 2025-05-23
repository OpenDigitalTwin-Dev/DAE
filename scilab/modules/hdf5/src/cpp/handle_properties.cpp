/*
* Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2015 - Scilab Enterprises - Antoine ELIAS
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

#include <vector>
#include <string>
#include <cfloat>

#include "handle_properties.hxx"
#include "double.hxx"

extern "C"
{
#include "h5_fileManagement.h"
#include "h5_writeDataToFile.h"
#include "h5_readDataFromFile.h"
#include "h5_attributeConstants.h"

#include "setGraphicObjectProperty.h"
#include "getGraphicObjectProperty.h"
#include "graphicObjectProperties.h"
#include "createGraphicObject.h"
#include "FigureList.h"
#include "CurrentFigure.h"
#include "BuildObjects.h"
#include "Matplot.h"
#include "HandleManagement.h"
}

extern types::InternalType* import_data(hid_t dataset);
extern hid_t export_data(hid_t parent, const std::string& name, types::InternalType* data, hid_t xfer_plist_id);

static int getHandleInt(hid_t dataset, const std::string& prop, int* val)
{
    hid_t node = 0;
    node = getDataSetIdFromName(dataset, prop.data());
    if (node < 0)
    {
        return -1;
    }

    readInteger32Matrix(node, val);
    return 0;
}

static int getHandleIntVector(hid_t dataset, const std::string& prop, int* row, int* col, int** vals)
{
    hid_t node = 0;
    node = getDataSetIdFromName(dataset, prop.data());
    if (node < 0)
    {
        return -1;
    }

    int complex = 0;
    int dims = 0;;
    int ret = getDatasetInfo(node, &complex, &dims, NULL);
    if (ret < 0)
    {
        closeDataSet(node);
        return -1;
    }


    std::vector<int> d(dims);
    int size = getDatasetInfo(node, &complex, &dims, d.data());

    if (dims == 0 || size <= 0)
    {
        closeDataSet(node);
        return -1;
    }

    *row = d[0];
    *col = d[1];
    *vals = new int[size];
    readInteger32Matrix(node, *vals);
    return 0;
}

static int getHandleBool(hid_t dataset, const std::string& prop, int* val)
{
    hid_t node = 0;
    node = getDataSetIdFromName(dataset, prop.data());
    if (node < 0)
    {
        return -1;
    }

    readBooleanMatrix(node, val);
    return 0;
}

static int getHandleBoolVector(hid_t dataset, const std::string& prop, int* row, int* col, int** vals)
{
    hid_t node = 0;
    node = getDataSetIdFromName(dataset, prop.data());
    if (node < 0)
    {
        return -1;
    }

    int complex = 0;
    int dims = 0;;
    int ret = getDatasetInfo(node, &complex, &dims, NULL);
    if (ret < 0)
    {
        closeDataSet(node);
        return -1;
    }


    std::vector<int> d(dims);
    int size = getDatasetInfo(node, &complex, &dims, d.data());

    if (dims == 0 || size <= 0)
    {
        closeDataSet(node);
        return -1;
    }

    *row = d[0];
    *col = d[1];

    *vals = new int[size];
    readBooleanMatrix(node, *vals);
    return 0;
}

static int getHandleDouble(hid_t dataset, const std::string& prop, double* val)
{
    hid_t node = 0;
    node = getDataSetIdFromName(dataset, prop.data());
    if (node < 0)
    {
        return -1;
    }

    readDoubleMatrix(node, val);
    return 0;
}

static int getHandleDoubleVector(hid_t dataset, const std::string& prop, int* row, int* col, double** vals)
{
    hid_t node = 0;
    node = getDataSetIdFromName(dataset, prop.data());
    if (node < 0)
    {
        return -1;
    }

    int complex = 0;
    int dims = 0;;
    int ret = getDatasetInfo(node, &complex, &dims, NULL);
    if (ret < 0)
    {
        closeDataSet(node);
        return -1;
    }

    if (dims == 0)
    {
        *row = 0;
        *col = 0;
        closeDataSet(node);
        return 0;
    }

    std::vector<int> d(dims);
    int size = getDatasetInfo(node, &complex, &dims, d.data());

    *row = d[0];
    *col = d[1];

    *vals = new double[size];
    readDoubleMatrix(node, *vals);
    return 0;
}

static hid_t getHandleString(hid_t dataset, const std::string& prop, char** val)
{
    hid_t node = 0;
    node = getDataSetIdFromName(dataset, prop.data());
    if (node < 0)
    {
        return -1;
    }

    int complex = 0;
    int dims = 0;;
    int ret = getDatasetInfo(node, &complex, &dims, NULL);
    if (ret < 0)
    {
        closeDataSet(node);
        return -1;
    }


    std::vector<int> d(dims);
    int size = getDatasetInfo(node, &complex, &dims, d.data());

    if (dims == 0 || size <= 0)
    {
        closeDataSet(node);
        return -1;
    }

    readStringMatrix(node, val);
    return node;
}

static hid_t getHandleStringVector(hid_t dataset, const std::string& prop, int* row, int* col, char*** vals)
{
    hid_t node = 0;
    node = getDataSetIdFromName(dataset, prop.data());
    if (node < 0)
    {
        return -1;
    }

    int complex = 0;
    int dims = 0;;
    int ret = getDatasetInfo(node, &complex, &dims, NULL);
    if (ret < 0)
    {
        closeDataSet(node);
        return -1;
    }

    if (dims == 0)
    {
        *row = 0;
        *col = 0;
        closeDataSet(node);
        return -1;
    }

    std::vector<int> d(dims);
    int size = getDatasetInfo(node, &complex, &dims, d.data());

    if (dims == 0 || size <= 0)
    {
        closeDataSet(node);
        return -1;
    }

    *row = d[0];
    *col = d[1];

    *vals = new char*[size];
    readStringMatrix(node, *vals);
    return node;
}

static int import_handle_generic(hid_t dataset, int uid, int parent, const HandleProp& props, bool childrenFirst, int version);

static void import_userdata(hid_t dataset, int uid)
{
    types::InternalType* ud = nullptr;
    hid_t node = 0;
    node = getDataSetIdFromName(dataset, "userdata");
    if (node < 0)
    {
        //no user data ?
        //assign an empty matrix
        ud = types::Double::Empty();
    }
    else
    {
        ud = import_data(node);
    }

    //increase ref before trying to delete old value to avoid double free
    ud->IncreaseRef();

    //get previous value
    int size = 0;
    int *psize = &size;
    int *data = NULL;

    getGraphicObjectProperty(uid, __GO_USER_DATA_SIZE__, jni_int, (void **)&psize);
    getGraphicObjectProperty(uid, __GO_USER_DATA__, jni_int_vector, (void **)&data);

    if (size != 0)
    {
        types::InternalType* previous = nullptr;
        if (size == 1)
        {
            //32 bits
            int* p = (int*)data;
            previous = ((types::InternalType*) * p);
        }
        else
        {
            //64 bits
            long long* p = (long long*)data;
            previous = ((types::InternalType*) * p);
        }

        previous->DecreaseRef();
        previous->killMe();
    }

    //set new value
    size = sizeof(void*) / sizeof(int);
    setGraphicObjectProperty(uid, __GO_USER_DATA__, &ud, jni_int_vector, size);
}

static void import_handle_tag(hid_t dataset, int uid)
{
    char* tag = nullptr;
    hid_t node = getHandleString(dataset, "tag", &tag);
    setGraphicObjectProperty(uid, __GO_TAG__, tag, jni_string, 1);
    freeStringMatrix(node, &tag);
}

static int import_handle_children(hid_t dataset, int parent, int version)
{
    //reload children
    hid_t children = getDataSetIdFromName(dataset, "children");
    int childcount = 0;
    getListDims6(children, &childcount);

    //reverse order
    for (int i = childcount - 1; i >= 0; --i)
    {
        hid_t c = getDataSetIdFromName(children, std::to_string(i).data());
        int newChild = import_handle(c, parent, version);
        if (newChild < 0)
        {
            return -1;
        }
    }

    closeList6(children);
    return parent;
}

static int import_handle_generic(hid_t dataset, int uid, int parent, const HandleProp& props, bool childrenFirst, int version)
{
    //  link current handle with its parent
    if (parent != -1)
    {
        setGraphicObjectRelationship(parent, uid);
    }

    //restore children before other property in case of properties has an
    //effect on children

    //reload children
    if (childrenFirst)
    {
        import_handle_children(dataset, uid, version);
    }

    for (auto& prop : props)
    {
        std::string name = prop.first;
        std::vector<int> info(prop.second);

        if (info[0] == SAVE_ONLY)
        {
            continue;
        }

        int go = info[1];
        int type = info[2];
        int row = 0;
        int col = 0;

        switch (type)
        {
            case jni_bool:
            {
                int val = 0;
                if (getHandleBool(dataset, name, &val) != -1)
                {
                    setGraphicObjectProperty(uid, go, &val, jni_bool, 1);
                }
                break;
            }
            case jni_int:
            {
                int val = 0;
                if (getHandleInt(dataset, name, &val) != -1)
                {
                    setGraphicObjectProperty(uid, go, &val, jni_int, 1);
                }
                break;
            }
            case jni_double:
            {
                double val = 0;
                if (getHandleDouble(dataset, name, &val) != -1)
                {
                    setGraphicObjectProperty(uid, go, &val, jni_double, 1);
                }
                break;
            }
            case jni_string:
            {
                char* data = nullptr;
                hid_t node = getHandleString(dataset, name, &data);
                if (node != - 1)
                {
                    setGraphicObjectProperty(uid, go, data, jni_string, 1);
                }
                freeStringMatrix(node, &data);
                break;
            }
            case jni_bool_vector:
            {
                int* vals = nullptr;
                getHandleBoolVector(dataset, name, &row, &col, &vals);
                if (vals)
                {
                    setGraphicObjectProperty(uid, go, vals, jni_bool_vector, row * col);
                    delete[] vals;
                }
                break;
            }
            case jni_int_vector:
            {
                int* vals = nullptr;
                getHandleIntVector(dataset, name, &row, &col, &vals);
                if (vals)
                {
                    setGraphicObjectProperty(uid, go, vals, jni_int_vector, row * col);
                    delete[] vals;
                }
                break;
            }
            case jni_double_vector:
            {
                double* vals = nullptr;
                getHandleDoubleVector(dataset, name, &row, &col, &vals);
                if (vals)
                {
                    setGraphicObjectProperty(uid, go, vals, jni_double_vector, row * col);
                    delete[] vals;
                }
                break;
            }
            case jni_string_vector:
            {
                char** vals = nullptr;
                hid_t node = getHandleStringVector(dataset, name, &row, &col, &vals);
                if (vals)
                {
                    setGraphicObjectProperty(uid, go, vals, jni_string_vector, row * col);
                }

                freeStringMatrix(node, vals);
                delete[] vals;
                break;
            }
        }
    }

    //userdata
    import_userdata(dataset, uid);

    //tag
    import_handle_tag(dataset, uid);

    //reload children
    if (childrenFirst == false)
    {
        import_handle_children(dataset, uid, version);
    }

    return uid;
}

static int import_handle_border(hid_t dataset);

static int import_handle_border_none(hid_t dataset, int border)
{
    closeList6(dataset);
    return border;
}

static int import_handle_border_line(hid_t dataset, int border)
{
    int status = 0;
    //color
    char* color = nullptr;
    hid_t nc = getHandleString(dataset, "color", &color);
    setGraphicObjectProperty(border, __GO_UI_FRAME_BORDER_COLOR__, color, jni_string, 1);
    freeStringMatrix(nc, &color);

    //thickness
    int thickness = 0;
    status = getHandleInt(dataset, "thickness", &thickness);
    if (status != -1)
    {
        setGraphicObjectProperty(border, __GO_LINE_THICKNESS__, &thickness, jni_int, 1);
    }

    //rounded
    int rounded = 0;
    status = getHandleBool(dataset, "rounded", &rounded);
    if (status != -1)
    {
        setGraphicObjectProperty(border, __GO_UI_FRAME_BORDER_ROUNDED__, &rounded, jni_bool, 1);
    }

    closeList6(dataset);
    return border;
}

static int import_handle_border_bevel(hid_t dataset, int border)
{
    char* data = nullptr;
    hid_t node = 0;
    //type
    int type = 0;
    getHandleInt(dataset, "type", &type);
    setGraphicObjectProperty(border, __GO_UI_FRAME_BORDER_TYPE__, &type, jni_int, 1);

    //highlight out
    node = getHandleString(dataset, "highlight_out", &data);
    if (data)
    {
        setGraphicObjectProperty(border, __GO_UI_FRAME_BORDER_HIGHLIGHT_OUT__, data, jni_string, 1);
    }

    freeStringMatrix(node, &data);
    data = nullptr;

    //highlight in
    node = getHandleString(dataset, "highlight_in", &data);
    if (data)
    {
        setGraphicObjectProperty(border, __GO_UI_FRAME_BORDER_HIGHLIGHT_IN__, data, jni_string, 1);
    }

    freeStringMatrix(node, &data);
    data = nullptr;

    //shadow out
    node = getHandleString(dataset, "shadow_out", &data);
    if (data)
    {
        setGraphicObjectProperty(border, __GO_UI_FRAME_BORDER_SHADOW_OUT__, data, jni_string, 1);
    }

    freeStringMatrix(node, &data);
    data = nullptr;

    //shadow in
    node = getHandleString(dataset, "shadow_in", &data);
    if (data)
    {
        setGraphicObjectProperty(border, __GO_UI_FRAME_BORDER_SHADOW_IN__, data, jni_string, 1);
    }

    freeStringMatrix(node, &data);
    data = nullptr;

    closeList6(dataset);
    return border;
}

static int import_handle_border_soft_bevel(hid_t dataset, int border)
{
    return import_handle_border_bevel(dataset, border);
}

static int import_handle_border_etched(hid_t dataset, int border)
{
    int status = 0;
    char* data = nullptr;
    hid_t node = 0;

    //type
    int type = 0;
    status = getHandleInt(dataset, "type", &type);
    if (status != -1)
    {
        setGraphicObjectProperty(border, __GO_UI_FRAME_BORDER_TYPE__, &type, jni_int, 1);
    }

    //highlight out
    node = getHandleString(dataset, "highlight_out", &data);
    if (data)
    {
        setGraphicObjectProperty(border, __GO_UI_FRAME_BORDER_HIGHLIGHT_OUT__, data, jni_string, 1);
    }

    freeStringMatrix(node, &data);
    data = nullptr;

    //shadow out
    node = getHandleString(dataset, "shadow_out", &data);
    if (data)
    {
        setGraphicObjectProperty(border, __GO_UI_FRAME_BORDER_SHADOW_OUT__, data, jni_string, 1);
    }

    freeStringMatrix(node, &data);
    data = nullptr;

    closeList6(dataset);
    return border;
}

static int import_handle_border_titled(hid_t dataset, int border)
{
    char* data = nullptr;
    hid_t node = 0;
    int status = 0;

    //title border
    hid_t title_border = getDataSetIdFromName(dataset, "title_border");
    if (title_border != -1)
    {
        int hidden = 1;
        int uiborder = import_handle_border(title_border);
        setGraphicObjectProperty(border, __GO_UI_FRAME_BORDER_TITLE__, &uiborder, jni_int, 1);
        //force new border to be hide in scilab view
        setGraphicObjectProperty(uiborder, __GO_HIDDEN__, &hidden, jni_bool, 1);
    }

    //title
    node = getHandleString(dataset, "title", &data);
    if (data)
    {
        setGraphicObjectProperty(border, __GO_TITLE__, data, jni_string, 1);
    }

    freeStringMatrix(node, &data);
    data = nullptr;

    //justification
    int justification = 0;
    status = getHandleInt(dataset, "justification", &justification);
    if (status != -1)
    {
        setGraphicObjectProperty(border, __GO_UI_FRAME_BORDER_JUSTIFICATION__, &justification, jni_int, 1);
    }

    //fontname
    node = getHandleString(dataset, "fontname", &data);
    if (data)
    {
        setGraphicObjectProperty(border, __GO_UI_FONTNAME__, data, jni_string, 1);
    }

    freeStringMatrix(node, &data);
    data = nullptr;

    //fontangle
    node = getHandleString(dataset, "fontangle", &data);
    if (data)
    {
        setGraphicObjectProperty(border, __GO_UI_FONTANGLE__, data, jni_string, 1);
    }

    freeStringMatrix(node, &data);
    data = nullptr;

    //fontsize
    int fontsize = 0;
    status = getHandleInt(dataset, "fontsize", &fontsize);
    if (status != -1)
    {
        setGraphicObjectProperty(border, __GO_UI_FONTSIZE__, &fontsize, jni_int, 1);
    }

    //fontweight
    node = getHandleString(dataset, "fontweight", &data);
    if (data)
    {
        setGraphicObjectProperty(border, __GO_UI_FONTWEIGHT__, data, jni_string, 1);
    }

    freeStringMatrix(node, &data);
    data = nullptr;

    //position
    int position = 0;
    status = getHandleInt(dataset, "position", &position);
    if (status != -1)
    {
        setGraphicObjectProperty(border, __GO_UI_FRAME_BORDER_POSITION__, &position, jni_int, 1);
    }

    //color
    node = getHandleString(dataset, "color", &data);
    if (data)
    {
        setGraphicObjectProperty(border, __GO_UI_FRAME_BORDER_COLOR__, data, jni_string, 1);
    }

    freeStringMatrix(node, &data);
    data = nullptr;

    closeList6(dataset);
    return border;
}

static int import_handle_border_empty(hid_t dataset, int border)
{
    int row = 0;
    int col = 0;
    double* pos = nullptr;

    getHandleDoubleVector(dataset, "position", &row, &col, &pos);
    if (pos && row * col == 4)
    {
        setGraphicObjectProperty(border, __GO_POSITION__, pos, jni_double_vector, row * col);
    }
    delete[] pos;

    closeList6(dataset);
    return border;
}

static int import_handle_border_compound(hid_t dataset, int border)
{
    //out_border
    hid_t out_border = getDataSetIdFromName(dataset, "out_border");
    if (out_border != -1)
    {
        int hidden = 1;
        int uiborder = import_handle_border(out_border);
        setGraphicObjectProperty(border, __GO_UI_FRAME_BORDER_OUT_BORDER__, &uiborder, jni_int, 1);
        //force new border to be hide in scilab view
        setGraphicObjectProperty(uiborder, __GO_HIDDEN__, &hidden, jni_bool, 1);
    }

    //in_border
    hid_t in_border = getDataSetIdFromName(dataset, "in_border");
    if (in_border != -1)
    {
        int hidden = 1;
        int uiborder = import_handle_border(in_border);
        setGraphicObjectProperty(border, __GO_UI_FRAME_BORDER_IN_BORDER__, &uiborder, jni_int, 1);
        //force new border to be hide in scilab view
        setGraphicObjectProperty(uiborder, __GO_HIDDEN__, &hidden, jni_bool, 1);
    }

    closeList6(dataset);
    return border;
}

static int import_handle_border_matte(hid_t dataset, int border)
{
    int row = 0;
    int col = 0;
    double* pos = nullptr;

    getHandleDoubleVector(dataset, "position", &row, &col, &pos);
    setGraphicObjectProperty(border, __GO_POSITION__, pos, jni_double_vector, row * col);
    delete[] pos;

    //color
    char* data = nullptr;
    hid_t nc = getHandleString(dataset, "color", &data);
    setGraphicObjectProperty(border, __GO_UI_FRAME_BORDER_COLOR__, data, jni_string, 1);
    freeStringMatrix(nc, &data);

    closeList6(dataset);
    return border;
}

static int import_handle_border(hid_t dataset)
{
    int border = createGraphicObject(__GO_UI_FRAME_BORDER__);
    int style = 0;
    getHandleInt(dataset, "style", &style);

    setGraphicObjectProperty(border, __GO_UI_FRAME_BORDER_STYLE__, &style, jni_int, 1);

    switch (style)
    {
        default:
        case NONE:
            return import_handle_border_none(dataset, border);
        case LINE:
            return import_handle_border_line(dataset, border);
        case BEVEL:
            return import_handle_border_bevel(dataset, border);
        case SOFTBEVEL:
            return import_handle_border_soft_bevel(dataset, border);
        case ETCHED:
            return import_handle_border_etched(dataset, border);
        case TITLED:
            return import_handle_border_titled(dataset, border);
        case EMPTY:
            return import_handle_border_empty(dataset, border);
        case COMPOUND:
            return import_handle_border_compound(dataset, border);
        case MATTE:
            return import_handle_border_matte(dataset, border);
    }
}

static int import_handle_uicontrol(hid_t dataset, int parent, int version)
{
    int style = 0;
    getHandleInt(dataset, "style", &style);

    // some properties must be set before parent ( in import_handle_generic )

    // scrollable
    int scrollable = 0;
    getHandleBool(dataset, "scrollable", &scrollable);

    int uic = 0; 
    // uicontrol was created by their style instead of type like others graphic objects.
    if (style == __GO_UI_FRAME__ && scrollable == 1)
    {
        uic = createGraphicObject(__GO_UI_FRAME_SCROLLABLE__);
    }
    else
    {
        uic = createGraphicObject(style);
    }
    
    // layout
    int layout = 0;
    getHandleInt(dataset, "layout", &layout);
    setGraphicObjectProperty(uic, __GO_LAYOUT__, &layout, jni_int, 1);


    //margins
    int row = 0;
    int col = 0;

    double* margins = nullptr;
    getHandleDoubleVector(dataset, "margins", &row, &col, &margins);
    setGraphicObjectProperty(uic, __GO_MARGINS__, margins, jni_double_vector, row * col);
    delete[] margins;

    // constraints

    // border_position
    int border_position = 0;
    getHandleInt(dataset, "border_position", &border_position);
    setGraphicObjectProperty(uic, __GO_UI_BORDER_POSITION__, &border_position, jni_int, 1);

    // border_size
    int* border_size = nullptr;
    getHandleIntVector(dataset, "border_size", &row, &col, &border_size);
    setGraphicObjectProperty(uic, __GO_UI_BORDER_PREFERREDSIZE__, border_size, jni_int_vector, row * col);
    delete[] border_size;

    // gridbad_grid
    int* gridbad_grid = nullptr;
    getHandleIntVector(dataset, "gridbad_grid", &row, &col, &gridbad_grid);
    setGraphicObjectProperty(uic, __GO_UI_GRIDBAG_GRID__, gridbad_grid, jni_int_vector, row * col);
    delete[] gridbad_grid;

    // gridbad_weight
    double* gridbad_weight = nullptr;
    getHandleDoubleVector(dataset, "gridbad_weight", &row, &col, &gridbad_weight);
    setGraphicObjectProperty(uic, __GO_UI_GRIDBAG_WEIGHT__, gridbad_weight, jni_double_vector, row * col);
    delete[] gridbad_weight;

    // gridbad_fill
    int gridbad_fill = 0;
    getHandleInt(dataset, "gridbad_fill", &gridbad_fill);
    setGraphicObjectProperty(uic, __GO_UI_GRIDBAG_FILL__, &gridbad_fill, jni_int, 1);

    // gridbad_anchor
    int gridbad_anchor = 0;
    getHandleInt(dataset, "gridbad_anchor", &gridbad_anchor);
    setGraphicObjectProperty(uic, __GO_UI_GRIDBAG_ANCHOR__, &gridbad_anchor, jni_int, 1);

    // gridbad_padding
    int* gridbad_padding = nullptr;
    getHandleIntVector(dataset, "gridbad_padding", &row, &col, &gridbad_padding);
    setGraphicObjectProperty(uic, __GO_UI_GRIDBAG_PADDING__, gridbad_padding, jni_int_vector, row * col);
    delete[] gridbad_padding;

    // gridbad_size
    int* gridbad_size = nullptr;
    getHandleIntVector(dataset, "gridbad_size", &row, &col, &gridbad_size);
    setGraphicObjectProperty(uic, __GO_UI_GRIDBAG_PREFERREDSIZE__, gridbad_size, jni_int_vector, row * col);
    delete[] gridbad_size;

    //import "standards" properties
    import_handle_generic(dataset, uic, parent, UicontrolHandle::getPropertyList(), false, version);

    //string
    char** string = nullptr;
    hid_t node = getHandleStringVector(dataset, "string", &row, &col, &string);
    setGraphicObjectProperty(uic, __GO_UI_STRING_COLNB__, &col, jni_int, 1);
    setGraphicObjectProperty(uic, __GO_UI_STRING__, string, jni_string_vector, row * col);
    freeStringMatrix(node, string);
    delete[] string;

    //border
    hid_t dborder = getDataSetIdFromName(dataset, "border");
    int border = import_handle_border(dborder);
    setGraphicObjectProperty(uic, __GO_UI_FRAME_BORDER__, &border, jni_int, 1);

    //value
    double* value = nullptr;
    getHandleDoubleVector(dataset, "value", &row, &col, &value);
    setGraphicObjectProperty(uic, __GO_UI_VALUE__, value, jni_double_vector, row * col);
    delete[] value;

    closeList6(dataset);
    return uic;
}

static int import_handle_uicontextmenu(hid_t dataset, int parent, int version)
{
    int menu = createGraphicObject(__GO_UICONTEXTMENU__);

    //import "standards" properties
    import_handle_generic(dataset, menu, parent, UicontextmenuHandle::getPropertyList(), false, version);

    closeList6(dataset);
    return menu;
}

static int import_handle_uimenu(hid_t dataset, int parent, int version)
{
    int menu = createGraphicObject(__GO_UIMENU__);

    //import "standards" properties
    import_handle_generic(dataset, menu, parent, UimenuHandle::getPropertyList(), false, version);

    closeList6(dataset);
    return menu;
}

static int import_handle_light(hid_t dataset, int parent, int version)
{
    int light = createGraphicObject(__GO_LIGHT__);

    //import "standards" properties
    import_handle_generic(dataset, light, parent, LightHandle::getPropertyList(), true, version);

    closeList6(dataset);
    return light;
}

static int import_handle_axis(hid_t dataset, int parent, int version)
{
    int axis = createGraphicObject(__GO_AXIS__);

    //import "standards" properties
    import_handle_generic(dataset, axis, parent, AxisHandle::getPropertyList(), true, version);

    closeList6(dataset);
    return axis;
}

static int import_handle_label(hid_t dataset, int uid, int version)
{
    //import "standards" properties
    //do not create releationship between parent
    import_handle_generic(dataset, uid, -1, LabelHandle::getPropertyList(), true, version);

    //text
    std::vector<int> dims(2);
    char** data = nullptr;
    hid_t node = getHandleStringVector(dataset, "text", &dims[0], &dims[1], &data);

    setGraphicObjectProperty(uid, __GO_TEXT_ARRAY_DIMENSIONS__, dims.data(), jni_int_vector, 2);
    setGraphicObjectProperty(uid, __GO_TEXT_STRINGS__, data, jni_string_vector, dims[0] * dims[1]);
    freeStringMatrix(node, data);
    delete[] data;

    //interpreter
    data = nullptr;
    node = getHandleStringVector(dataset, "interpreter", &dims[0], &dims[1], &data);
    if (data)
    {
        setGraphicObjectProperty(uid, __GO_TEXT_INTERPRETERS__, data, jni_string_vector, dims[0] * dims[1]);
        freeStringMatrix(node, data);
        delete[] data;
    }

    closeList6(dataset);
    return uid;
}

static int import_handle_text(hid_t dataset, int parent, int version)
{
    int t = createGraphicObject(__GO_TEXT__);

    //import "standards" properties
    import_handle_generic(dataset, t, parent, TextHandle::getPropertyList(), true, version);


    //text
    int dims[2];
    char** text = nullptr;
    hid_t textnode = getHandleStringVector(dataset, "text", &dims[0], &dims[1], &text);
    setGraphicObjectProperty(t, __GO_TEXT_ARRAY_DIMENSIONS__, dims, jni_int_vector, 2);
    setGraphicObjectProperty(t, __GO_TEXT_STRINGS__, text, jni_string_vector, dims[0] * dims[1]);
    freeStringMatrix(textnode, text);
    delete[] text;

    //interpreter
    text = nullptr;
    textnode = getHandleStringVector(dataset, "interpreter", &dims[0], &dims[1], &text);
    if (text)
    {
        setGraphicObjectProperty(t, __GO_TEXT_INTERPRETERS__, text, jni_string_vector, dims[0] * dims[1]);
        freeStringMatrix(textnode, text);
        delete[] text;
    }

    closeList6(dataset);
    return t;
}

static int import_handle_legend(hid_t dataset, int parent, int version)
{
    int legend = createGraphicObject(__GO_LEGEND__);

    //import "standards" properties
    import_handle_generic(dataset, legend, parent, LegendHandle::getPropertyList(), true, version);


    //text
    int dims[2];
    char** text = nullptr;
    hid_t textnode = getHandleStringVector(dataset, "text", &dims[0], &dims[1], &text);
    setGraphicObjectProperty(legend, __GO_TEXT_ARRAY_DIMENSIONS__, dims, jni_int_vector, 2);
    setGraphicObjectProperty(legend, __GO_TEXT_STRINGS__, text, jni_string_vector, dims[0] * dims[1]);
    freeStringMatrix(textnode, text);
    delete[] text;

    //interpreter
    text = nullptr;
    textnode = getHandleStringVector(dataset, "interpreter", &dims[0], &dims[1], &text);
    if (text)
    {
        setGraphicObjectProperty(legend, __GO_TEXT_INTERPRETERS__, text, jni_string_vector, dims[0] * dims[1]);
        freeStringMatrix(textnode, text);
        delete[] text;
    }

    //links

    //to retore links we need to have the entire hierarchie loaded.
    //store links information in a "global" variable and update variable at the end of process.
    hid_t node = getDataSetIdFromName(dataset, "links");
    int count = 0;
    getListDims6(node, &count);

    Links::PathList paths;

    for (int i = 0; i < count; ++i)
    {
        int row = 0;
        int col = 0;
        int* path = nullptr;
        getHandleIntVector(node, std::to_string(i).data(), &row, &col, &path);
        std::vector<int> res(row * col);
        for (int j = 0; j < row * col; ++j)
        {
            res[j] = path[j];
        }

        delete[] path;

        paths.push_back(res);
    }

    Links::add(legend, paths);

    closeList6(node);
    closeList6(dataset);
    return legend;
}

static int import_handle_fec(hid_t dataset, int parent, int version)
{
    int fec = createGraphicObject(__GO_FEC__);
    createDataObject(fec, __GO_FEC__);

    //import "standards" properties
    import_handle_generic(dataset, fec, parent, FecHandle::getPropertyList(), true, version);

    //triangles
    int coordR = 0;
    int coordC = 0;
    int valuesR = 0;
    int valuesC = 0;
    int trianglesR = 0;
    int trianglesC = 0;

    double* coords = nullptr;
    getHandleDoubleVector(dataset, "coords", &coordR, &coordC, &coords);
    double* values = nullptr;
    getHandleDoubleVector(dataset, "values", &valuesR, &valuesC, &values);
    double* triangles = nullptr;
    getHandleDoubleVector(dataset, "triangles", &trianglesR, &trianglesC, &triangles);
    int realcol = trianglesC - 2;

    setGraphicObjectPropertyAndNoWarn(fec, __GO_DATA_MODEL_NUM_VERTICES__, &valuesC, jni_int, 1);
    setGraphicObjectPropertyAndNoWarn(fec, __GO_DATA_MODEL_NUM_VERTICES_BY_ELEM__, &realcol, jni_int, 1);
    setGraphicObjectPropertyAndNoWarn(fec, __GO_DATA_MODEL_NUM_INDICES__, &trianglesR, jni_int, 1);
    //setGraphicObjectPropertyAndNoWarn(fec, __GO_DATA_MODEL_X__, coords, jni_double_vector, coordR);
    //setGraphicObjectPropertyAndNoWarn(fec, __GO_DATA_MODEL_Y__, coords + coordR, jni_double_vector, coordR);
    setGraphicObjectPropertyAndNoWarn(fec, __GO_DATA_MODEL_COORDINATES__, coords, jni_double_vector, coordC);
    setGraphicObjectPropertyAndNoWarn(fec, __GO_DATA_MODEL_FEC_ELEMENTS__, triangles, jni_double_vector, trianglesR);
    setGraphicObjectPropertyAndNoWarn(fec, __GO_DATA_MODEL_VALUES__, values, jni_double_vector, valuesC);

    delete[] coords;
    delete[] values;
    delete[] triangles;

    closeList6(dataset);
    return fec;
}

static int import_handle_matplot(hid_t dataset, int parent, int version)
{
    int plot = createGraphicObject(__GO_MATPLOT__);
    createDataObject(plot, __GO_MATPLOT__);

    //import "standards" properties
    import_handle_generic(dataset, plot, parent, MatplotHandle::getPropertyList(), true, version);

    int row = 0;
    int col = 0;
    //rect
    double* rect = nullptr;
    getHandleDoubleVector(dataset, "rect", &row, &col, &rect);
    double tmp = rect[1];
    rect[1] = rect[2];
    rect[2] = tmp;

    //data
    int num_x = 0;
    int num_y = 0;
    getHandleInt(dataset, "num_x", &num_x);
    getHandleInt(dataset, "num_y", &num_y);
    double* data = nullptr;
    int data_x = 0;
    int data_y = 0;
    getHandleDoubleVector(dataset, "data", &data_x, &data_y, &data);

    int grid[4];
    grid[0] = num_x;
    grid[1] = 1;
    grid[2] = num_y;
    grid[3] = 1;

    setGraphicObjectPropertyAndNoWarn(plot, __GO_DATA_MODEL_GRID_SIZE__, grid, jni_int_vector, 4);

    double scale[2];
    setGraphicObjectProperty(plot, __GO_MATPLOT_TRANSLATE__, rect, jni_double_vector, 2);
    scale[0] = (rect[2] - rect[0]) / (num_x - 1.0);
    scale[1] = (rect[3] - rect[1]) / (num_y - 1.0);
    setGraphicObjectProperty(plot, __GO_MATPLOT_SCALE__, scale, jni_double_vector, 2);

    setGraphicObjectProperty(plot, __GO_DATA_MODEL_MATPLOT_BOUNDS__, rect, jni_double_vector, 4);

    setGraphicObjectProperty(plot, __GO_DATA_MODEL_MATPLOT_IMAGE_DATA__, data, jni_double_vector, data_x * data_y);

    delete[] rect;
    delete[] data;

    closeList6(dataset);
    return plot;
}

static int import_handle_grayplot(hid_t dataset, int parent, int version)
{
    int plot = createGraphicObject(__GO_GRAYPLOT__);
    createDataObject(plot, __GO_GRAYPLOT__);

    //import "standards" properties
    import_handle_generic(dataset, plot, parent, GrayplotHandle::getPropertyList(), true, version);

    int row = 0;
    int col = 0;
    double* dataX = nullptr;
    double* dataY = nullptr;
    double* dataZ = nullptr;
    getHandleDoubleVector(dataset, "data_x", &row, &col, &dataX);
    getHandleDoubleVector(dataset, "data_y", &row, &col, &dataY);
    getHandleDoubleVector(dataset, "data_z", &row, &col, &dataZ);

    int grid[4];
    grid[0] = row;
    grid[1] = 1;
    grid[2] = col;
    grid[3] = 1;

    //set dimensions
    setGraphicObjectPropertyAndNoWarn(plot, __GO_DATA_MODEL_GRID_SIZE__, grid, jni_int_vector, 4);

    //set data
    setGraphicObjectPropertyAndNoWarn(plot, __GO_DATA_MODEL_X__, dataX, jni_double_vector, row);
    setGraphicObjectPropertyAndNoWarn(plot, __GO_DATA_MODEL_Y__, dataY, jni_double_vector, col);
    setGraphicObjectProperty(plot, __GO_DATA_MODEL_Z__, dataZ, jni_double_vector, row * col);

    closeList6(dataset);
    return plot;
}

static int import_handle_segs(hid_t dataset, int parent, int version)
{
    int segs = createGraphicObject(__GO_SEGS__);

    //import "standards" properties
    import_handle_generic(dataset, segs, parent, SegsHandle::getPropertyList(), true, version);

    closeList6(dataset);
    return segs;
}

static int import_handle_arc(hid_t dataset, int parent, int version)
{
    int arc = createGraphicObject(__GO_ARC__);

    //import "standards" properties
    import_handle_generic(dataset, arc, parent, ArcHandle::getPropertyList(), true, version);

    closeList6(dataset);
    return arc;
}

static int import_handle_rectangle(hid_t dataset, int parent, int version)
{
    int rect = createGraphicObject(__GO_RECTANGLE__);

    //import "standards" properties
    import_handle_generic(dataset, rect, parent, RectangleHandle::getPropertyList(), true, version);

    closeList6(dataset);
    return rect;
}

static int import_handle_compound(hid_t dataset, int parent, int version)
{
    int compound = createGraphicObject(__GO_COMPOUND__);

    //import "standards" properties
    import_handle_generic(dataset, compound, parent, CompoundHandle::getPropertyList(), true, version);

    closeList6(dataset);
    return compound;
}

static int import_handle_datatip(hid_t dataset, int parent, int version)
{
    int row;
    int col;
    double* dblData = nullptr;
    double dblIndex[2];

    int datatip = createGraphicObject(__GO_DATATIP__);


    //set parent manually, there is no real releationship between datatip and parent
    setGraphicObjectProperty(datatip, __GO_PARENT__, &parent, jni_int, 1);

    // data index (integer index and double "ratio")
    getHandleDoubleVector(dataset, "data_index", &row, &col, &dblData);
    if (dblData != nullptr)
    {
        dblIndex[0]=dblData[0];
        dblIndex[1]=dblData[1];
    }
    else
    {
        // legacy 0 "ratio"
        int index;
        getHandleInt(dataset, "data_index", &index);        
        dblIndex[0] = index;
        dblIndex[1] = 0;
    }

    setGraphicObjectProperty(datatip, __GO_DATATIP_INDEXES__, dblIndex, jni_double_vector, 2);

    //import "standards" properties
    import_handle_generic(dataset, datatip, -1, DatatipHandle::getPropertyList(), true, version);

    //interpreter
    char **text = nullptr;
    int dims[2];
    hid_t textnode = getHandleStringVector(dataset, "interpreter", &dims[0], &dims[1], &text);
    if (text)
    {
        setGraphicObjectProperty(datatip, __GO_TEXT_INTERPRETERS__, text, jni_string_vector, dims[0] * dims[1]);
        freeStringMatrix(textnode, text);
        delete[] text;
    }

    closeList6(dataset);
    return datatip;
}

static int import_handle_datatips(hid_t dataset, int uid, int version)
{
    hid_t datatip = getDataSetIdFromName(dataset, "datatips");
    int count = 0;
    getListDims6(datatip, &count);

    std::vector<int> datatips(count);
    for (int i = 0; i < count; ++i)
    {
        hid_t d = getDataSetIdFromName(datatip, std::to_string(i).data());
        datatips[i] = import_handle_datatip(d, uid, version);
    }

    setGraphicObjectProperty(uid, __GO_DATATIPS__, datatips.data(), jni_int_vector, count);

    closeList6(datatip);
    return uid;
}

static int import_polyline_shift(hid_t dataset, int uid, const std::string& name, int go_set, int go_data)
{
    int row = 0;
    int col = 0;
    int set = 0;
    double* data = nullptr;

    getHandleDoubleVector(dataset, name.data(), &row, &col, &data);
    if (row * col != 0)
    {
        set = 1;
        setGraphicObjectProperty(uid, go_data, data, jni_double_vector, row * col);
    }
    else
    {
        set = 0;
    }

    delete[] data;
    setGraphicObjectProperty(uid, go_set, &set, jni_int, 1);
    return uid;
}

static void updateXYDataBounds(double rect[6], int axes = -1)
{
    int firstPlot = 0;
    int * piFirstPlot = &firstPlot;
    if (axes == -1)
    {
        axes = getOrCreateDefaultSubwin();
    }

    getGraphicObjectProperty(axes, __GO_FIRST_PLOT__, jni_bool, (void **)&piFirstPlot);
    if (firstPlot)
    {
        rect[4] = 0;
        rect[5] = 0;
    }
    else
    {
        double * dataBounds = NULL;
        getGraphicObjectProperty(axes, __GO_DATA_BOUNDS__, jni_double_vector, (void **)&dataBounds);

        rect[0] = Min(rect[0], dataBounds[0]);
        rect[1] = Max(rect[1], dataBounds[1]);
        rect[2] = Min(rect[2], dataBounds[2]);
        rect[3] = Max(rect[3], dataBounds[3]);
        rect[4] = dataBounds[4];
        rect[5] = dataBounds[5];
    }

    setGraphicObjectProperty(axes, __GO_DATA_BOUNDS__, rect, jni_double_vector, 6);
}

static int mustUpdate(int axes = -1)
{
    int iTmp = 0;
    int * piTmp = &iTmp;
    if (axes == -1)
    {
        axes = getOrCreateDefaultSubwin();
    }

    getGraphicObjectProperty(axes, __GO_AUTO_SCALE__, jni_bool, (void **)&piTmp);
    return iTmp;
}

void MiniMaxi(const double vect[], int n, double * const min, double * const max)
{
    int i = 0;
    double _min = DBL_MAX;
    double _max = -DBL_MAX;
    for (; i < n; i++)
    {
        /*    if ( isinf(vect[i])== 0 && isnan(vect[i])==0 && vect[i] < vmin)  */
        if (finite(vect[i]) == 1)
        {
            if (vect[i] < _min)
            {
                _min = vect[i];
            }
            if (vect[i] > _max)
            {
                _max = vect[i];
            }
        }
    }

    *min = _min;
    *max = _max;
}


static int import_handle_polyline(hid_t dataset, int parent, int version)
{
    int polyline = createGraphicObject(__GO_POLYLINE__);
    createDataObject(polyline, __GO_POLYLINE__);

    //import "standards" properties
    import_handle_generic(dataset, polyline, parent, PolylineHandle::getPropertyList(), true, version);

    //x_shift
    import_polyline_shift(dataset, polyline, "x_shift", __GO_DATA_MODEL_X_COORDINATES_SHIFT_SET__, __GO_DATA_MODEL_X_COORDINATES_SHIFT__);
    //y_shift
    import_polyline_shift(dataset, polyline, "y_shift", __GO_DATA_MODEL_Y_COORDINATES_SHIFT_SET__, __GO_DATA_MODEL_Y_COORDINATES_SHIFT__);
    //z_shift
    import_polyline_shift(dataset, polyline, "z_shift", __GO_DATA_MODEL_Z_COORDINATES_SHIFT_SET__, __GO_DATA_MODEL_Z_COORDINATES_SHIFT__);


    //interp_color_vector
    {
        int row = 0;
        int col = 0;
        int set = 0;
        int* data = nullptr;

        getHandleIntVector(dataset, "interp_color_vector", &row, &col, &data);
        if (row * col != 0)
        {
            set = 1;
            setGraphicObjectProperty(polyline, __GO_INTERP_COLOR_VECTOR__, data, jni_double_vector, row * col);
        }
        else
        {
            set = 0;
        }

        delete[] data;
        setGraphicObjectProperty(polyline, __GO_INTERP_COLOR_VECTOR_SET__, &set, jni_bool, 1);
    }

    //data
    {
        int numElementsArray[2];
        int size = 0;
        double* dataX = nullptr;
        double* dataY = nullptr;
        double* dataZ = nullptr;
        getHandleDoubleVector(dataset, "data_x", &numElementsArray[0], &numElementsArray[1], &dataX);
        size = numElementsArray[0] * numElementsArray[1];
        getHandleDoubleVector(dataset, "data_y", &numElementsArray[0], &numElementsArray[1], &dataY);

        if (numElementsArray[0] * numElementsArray[1] != size)
        {
            std::cout << "size trouble !!!" << std::endl;
        }

        setGraphicObjectPropertyAndNoWarn(polyline, __GO_DATA_MODEL_NUM_ELEMENTS_ARRAY__, numElementsArray, jni_int_vector, 2);
        setGraphicObjectPropertyAndNoWarn(polyline, __GO_DATA_MODEL_X__, dataX, jni_double_vector, size);
        setGraphicObjectPropertyAndNoWarn(polyline, __GO_DATA_MODEL_Y__, dataY, jni_double_vector, size);

        //data z
        int zSet = 0;
        numElementsArray[0] = 0;
        numElementsArray[1] = 0;
        getHandleDoubleVector(dataset, "data_z", &numElementsArray[0], &numElementsArray[1], &dataZ);
        if (numElementsArray[0] * numElementsArray[1] != 0)
        {
            setGraphicObjectPropertyAndNoWarn(polyline, __GO_DATA_MODEL_Z__, dataZ, jni_double_vector, size);
            zSet = 1;
        }

        setGraphicObjectProperty(polyline, __GO_DATA_MODEL_Z_COORDINATES_SET__, &zSet, jni_int, 1);

        //update parent axes data_bounds
        if (mustUpdate())
        {
            double rect[6];

            MiniMaxi(dataX, size, rect, rect + 1);
            MiniMaxi(dataY, size, rect + 2, rect + 3);

            updateXYDataBounds(rect);
        }

        delete[] dataX;
        delete[] dataY;
        delete[] dataZ;
    }

    //datatips
    import_handle_datatips(dataset, polyline, version);

    closeList6(dataset);
    return polyline;
}

static int import_handle_surface(hid_t dataset, int uid, int parent, int version)
{
    //import "standards" properties
    import_handle_generic(dataset, uid, parent, SurfaceHandle::getPropertyList(), true, version);
    return uid;
}

static int import_handle_plot3d(hid_t dataset, int parent, int version)
{
    int plot = createGraphicObject(__GO_PLOT3D__);
    createDataObject(plot, __GO_PLOT3D__);
    import_handle_surface(dataset, plot, parent, version);

    //data
    int xR = 0, xC = 0;
    double* dataX = nullptr;
    int yR = 0, yC = 0;
    double* dataY = nullptr;
    int zR = 0, zC = 0;
    double* dataZ = nullptr;

    getHandleDoubleVector(dataset, "data_x", &xR, &xC, &dataX);
    getHandleDoubleVector(dataset, "data_y", &yR, &yC, &dataY);
    getHandleDoubleVector(dataset, "data_z", &zR, &zC, &dataZ);

    int gridSize[4];
    int result;

    gridSize[0] = xR;
    gridSize[1] = xC;
    gridSize[2] = yR;
    gridSize[3] = yC;

    result = setGraphicObjectPropertyAndNoWarn(plot, __GO_DATA_MODEL_GRID_SIZE__, gridSize, jni_int_vector, 4);
    if (result == FALSE)
    {
        return -1;
    }

    setGraphicObjectPropertyAndNoWarn(plot, __GO_DATA_MODEL_X__, dataX, jni_double_vector, xR * xC);
    setGraphicObjectPropertyAndNoWarn(plot, __GO_DATA_MODEL_Y__, dataY, jni_double_vector, yR * yC);
    setGraphicObjectProperty(plot, __GO_DATA_MODEL_Z__, dataZ, jni_double_vector, zR * zC);

    delete[] dataX;
    delete[] dataY;
    delete[] dataZ;


    closeList6(dataset);
    return plot;
}

static int import_handle_fac3d(hid_t dataset, int parent, int version)
{
    int fac = createGraphicObject(__GO_FAC3D__);
    createDataObject(fac, __GO_FAC3D__);

    import_handle_surface(dataset, fac, parent, version);

    //data
    int xR = 0, xC = 0;
    double* dataX = nullptr;
    int yR = 0, yC = 0;
    double* dataY = nullptr;
    int zR = 0, zC = 0;
    double* dataZ = nullptr;

    getHandleDoubleVector(dataset, "data_x", &xR, &xC, &dataX);
    getHandleDoubleVector(dataset, "data_y", &yR, &yC, &dataY);
    getHandleDoubleVector(dataset, "data_z", &zR, &zC, &dataZ);

    int cR = 0;
    int cC = 0;
    double* colors = nullptr;
    getHandleDoubleVector(dataset, "colors", &cR, &cC, &colors);
    int cSize = cR * cC;

    int numElementsArray[3];

    numElementsArray[0] = xC;
    numElementsArray[1] = xR;
    numElementsArray[2] = cSize;

    setGraphicObjectPropertyAndNoWarn(fac, __GO_DATA_MODEL_NUM_ELEMENTS_ARRAY__, numElementsArray, jni_int_vector, 3);

    setGraphicObjectPropertyAndNoWarn(fac, __GO_DATA_MODEL_X__, dataX, jni_double_vector, xR * xC);
    setGraphicObjectPropertyAndNoWarn(fac, __GO_DATA_MODEL_Y__, dataY, jni_double_vector, yR * yC);
    setGraphicObjectPropertyAndNoWarn(fac, __GO_DATA_MODEL_Z__, dataZ, jni_double_vector, zR * zC);
    setGraphicObjectProperty(fac, __GO_DATA_MODEL_COLORS__, colors, jni_double_vector, cSize);

    delete[] dataX;
    delete[] dataY;
    delete[] dataZ;
    delete[] colors;

    //cdata_mapping
    int cdata = 0;
    getHandleInt(dataset, "cdata_mapping", &cdata);
    setGraphicObjectProperty(fac, __GO_DATA_MAPPING__, &cdata, jni_int, 1); // Redraw in case properties below are not found

    int row = 0, col = 0;
    // cdata_bounds (2025.0.0)
    double* pdblVals = nullptr;
    getHandleDoubleVector(dataset, "cdata_bounds", &row, &col, &pdblVals);
    if (pdblVals)
    {
        setGraphicObjectPropertyAndNoWarn(fac, __GO_CDATA_BOUNDS__, pdblVals, jni_double_vector, row * col); // Redraw will be done when updating "color_range"
    }
    delete[] pdblVals;

    // color_range (2025.0.0)
    int* piVals = nullptr;
    getHandleIntVector(dataset, "color_range", &row, &col, &piVals);
    if (piVals)
    {
        setGraphicObjectProperty(fac, __GO_COLOR_RANGE__, piVals, jni_int_vector, row * col);
    }
    delete[] piVals;

    closeList6(dataset);
    return fac;
}


static int import_handle_champ(hid_t dataset, int parent, int version)
{
    //need to get properties and call a "creator" :x

    int champ = createGraphicObject(__GO_CHAMP__);

    //data
    int row = 0;
    int col = 0;
    int dims[2];
    int num = 0;
    double* baseX = nullptr;
    getHandleDoubleVector(dataset, "base_x", &row, &col, &baseX);
    dims[0] = col;

    double* baseY = nullptr;
    getHandleDoubleVector(dataset, "base_y", &row, &col, &baseY);
    dims[1] = col;
    num = dims[0] * dims[1];

    double* directionX = nullptr;
    getHandleDoubleVector(dataset, "direction_x", &row, &col, &directionX);

    double* directionY = nullptr;
    getHandleDoubleVector(dataset, "direction_y", &row, &col, &directionY);

    setGraphicObjectProperty(champ, __GO_NUMBER_ARROWS__, &num, jni_int, 1);
    setGraphicObjectProperty(champ, __GO_CHAMP_DIMENSIONS__, dims, jni_int_vector, 2);
    setGraphicObjectProperty(champ, __GO_BASE_X__, baseX, jni_double_vector, dims[0]);
    setGraphicObjectProperty(champ, __GO_BASE_Y__, baseY, jni_double_vector, dims[1]);
    setGraphicObjectProperty(champ, __GO_DIRECTION_X__, directionX, jni_double_vector, dims[0] * dims[1]);
    setGraphicObjectProperty(champ, __GO_DIRECTION_Y__, directionY, jni_double_vector, dims[0] * dims[1]);

    delete[] baseX;
    delete[] baseY;
    delete[] directionX;
    delete[] directionY;

    //import "standards" properties
    import_handle_generic(dataset, champ, parent, ChampHandle::getPropertyList(), true, version);

    closeList6(dataset);
    return champ;
}

static int import_handle_axes(hid_t dataset, int parent, int version)
{
    //how to manage call by %h_copy ?

    int axes = createSubWin(parent);

    //hide current axes
    int visible = 0;
    setGraphicObjectProperty(axes, __GO_VISIBLE__, &visible, jni_bool, 1);

    //import "standards" properties
    if (version == 3)
    {
        import_handle_generic(dataset, axes, parent, AxesHandle::getPropertyList3(), true, version);
    }
    else
    {
        import_handle_generic(dataset, axes, parent, AxesHandle::getPropertyList(), true, version);
    }

    //title
    int title = 0;
    int *ptitle = &title;
    hid_t nodeTitle = getDataSetIdFromName(dataset, "title");
    getGraphicObjectProperty(axes, __GO_TITLE__, jni_int, (void **)&ptitle);
    import_handle_label(nodeTitle, title, version);

    //x_label
    int x_label = 0;
    int *px_label = &x_label;
    hid_t nodeX = getDataSetIdFromName(dataset, "x_label");
    getGraphicObjectProperty(axes, __GO_X_AXIS_LABEL__, jni_int, (void **)&px_label);
    import_handle_label(nodeX, x_label, version);

    //y_label
    int y_label = 0;
    int *py_label = &y_label;
    hid_t nodeY = getDataSetIdFromName(dataset, "y_label");
    getGraphicObjectProperty(axes, __GO_Y_AXIS_LABEL__, jni_int, (void **)&py_label);
    import_handle_label(nodeY, y_label, version);

    //z_label
    int z_label = 0;
    int *pz_label = &z_label;
    hid_t nodeZ = getDataSetIdFromName(dataset, "z_label");
    getGraphicObjectProperty(axes, __GO_Z_AXIS_LABEL__, jni_int, (void **)&pz_label);
    import_handle_label(nodeZ, z_label, version);

    //set real visible state
    getHandleInt(dataset, "visible", &visible);
    setGraphicObjectProperty(axes, __GO_VISIBLE__, &visible, jni_bool, 1);

    closeList6(dataset);
    return axes;
}

static int import_handle_layout_options(hid_t dataset, int frame)
{
    int layout_type = 0;
    getHandleInt(dataset, "layout", &layout_type);
    int row = 0;
    int col = 0;
    int* data = nullptr;

    switch (layout_type)
    {
        case 2: //grid
        {
            hid_t node = getDataSetIdFromName(dataset, "layout_options");
            getHandleIntVector(node, "grid", &row, &col, &data);
            if (data && row * col == 2)
            {
                setGraphicObjectProperty(frame, __GO_GRID_OPT_GRID__, data, jni_int_vector, 2);
            }

            delete[] data;
            data = nullptr;

            getHandleIntVector(node, "padding", &row, &col, &data);
            if (data && row * col == 2)
            {
                setGraphicObjectProperty(frame, __GO_GRID_OPT_PADDING__, data, jni_int_vector, 2);
            }

            delete[] data;
            data = nullptr;

            closeList6(node);
            break;
        }
        case 3: //border
        {
            hid_t node = getDataSetIdFromName(dataset, "layout_options");
            getHandleIntVector(node, "padding", &row, &col, &data);
            if (data && row * col == 2)
            {
                setGraphicObjectProperty(frame, __GO_BORDER_OPT_PADDING__, data, jni_int_vector, 2);
            }

            delete[] data;
            data = nullptr;

            closeList6(node);
            break;
        }
    }

    return frame;
}

static int import_handle_figure(hid_t dataset, int parent, int version)
{
    //some properties must be set @ creation time
    int menubar = 0;
    getHandleInt(dataset, "menubar", &menubar);
    int toolbar = 0;
    getHandleInt(dataset, "toolbar", &toolbar);
    int dockable = 0;
    getHandleBool(dataset, "dockable", &dockable);
    int default_axes = 0;
    getHandleBool(dataset, "default_axes", &default_axes);
    int antiAliasing = 0;
    if (getHandleInt(dataset, "anti_aliasing", &antiAliasing) != 0)
    {
        antiAliasing = 0; // No anti-aliasing by default
    }

    //force visible true FOR DEBUG ONLY
    int visible = 0;

    //create a new hidden figure without default_axes.
    int fig = createFigure(dockable, menubar, toolbar, 0, visible, antiAliasing);
    //set default axes properties after creation to avoid useless axes creation
    setGraphicObjectProperty(fig, __GO_DEFAULT_AXES__, &default_axes, jni_bool, 1);

    int id = getValidDefaultFigureId();
    setGraphicObjectProperty(fig, __GO_ID__, &id, jni_int, 1);

    int menu = 0;
    getHandleBool(dataset, "menubar_visible", &menu);
    int notmenu = menu == 0;
    int info = 0;
    getHandleBool(dataset, "infobar_visible", &info);
    int notinfo = info == 0;
    int tool = 0;
    getHandleBool(dataset, "toolbar_visible", &tool);
    int nottool = tool == 0;

    //force inverse flag
    setGraphicObjectProperty(fig, __GO_MENUBAR_VISIBLE__, &notmenu, jni_bool, 1);
    setGraphicObjectProperty(fig, __GO_INFOBAR_VISIBLE__, &notinfo, jni_bool, 1);
    setGraphicObjectProperty(fig, __GO_TOOLBAR_VISIBLE__, &nottool, jni_bool, 1);

    //set real value
    setGraphicObjectProperty(fig, __GO_MENUBAR_VISIBLE__, &menu, jni_bool, 1);
    setGraphicObjectProperty(fig, __GO_INFOBAR_VISIBLE__, &info, jni_bool, 1);
    setGraphicObjectProperty(fig, __GO_TOOLBAR_VISIBLE__, &tool, jni_bool, 1);

    //layout
    int layout = 0;
    getHandleInt(dataset, "layout", &layout);
    setGraphicObjectProperty(fig, __GO_LAYOUT__, &layout, jni_int, 1);

    //import "standards" properties
    import_handle_generic(dataset, fig, -1, FigureHandle::getPropertyList(), true, version);

    import_handle_layout_options(dataset, fig);
    closeList6(dataset);
    return fig;
}

int import_handle(hid_t dataset, int parent, int version)
{
    //get type
    int type = 0;
    getHandleInt(dataset, "type", &type);
    switch (type)
    {
        case __GO_FIGURE__:
        {
            return import_handle_figure(dataset, parent, version);
        }
        case __GO_AXES__:
        {
            return import_handle_axes(dataset, parent, version);
        }
        case __GO_CHAMP__:
        {
            return import_handle_champ(dataset, parent, version);
        }
        case __GO_FAC3D__:
        {
            return import_handle_fac3d(dataset, parent, version);
        }
        case __GO_PLOT3D__:
        {
            return import_handle_plot3d(dataset, parent, version);
        }
        case __GO_COMPOUND__:
        {
            return import_handle_compound(dataset, parent, version);
        }
        case __GO_POLYLINE__:
        {
            return import_handle_polyline(dataset, parent, version);
        }
        case __GO_RECTANGLE__:
        {
            return import_handle_rectangle(dataset, parent, version);
        }
        case __GO_ARC__:
        {
            return import_handle_arc(dataset, parent, version);
        }
        case __GO_SEGS__:
        {
            return import_handle_segs(dataset, parent, version);
        }
        case __GO_GRAYPLOT__:
        {
            return import_handle_grayplot(dataset, parent, version);
        }
        case __GO_MATPLOT__:
        {
            return import_handle_matplot(dataset, parent, version);
        }
        case __GO_FEC__:
        {
            return import_handle_fec(dataset, parent, version);
        }
        case __GO_LEGEND__:
        {
            return import_handle_legend(dataset, parent, version);
        }
        case __GO_TEXT__:
        {
            return import_handle_text(dataset, parent, version);
        }
        case __GO_AXIS__:
        {
            return import_handle_axis(dataset, parent, version);
        }
        case __GO_LIGHT__:
        {
            return import_handle_light(dataset, parent, version);
        }
        case __GO_UIMENU__:
        {
            return import_handle_uimenu(dataset, parent, version);
        }
        case __GO_UICONTEXTMENU__:
        {
            return import_handle_uicontextmenu(dataset, parent, version);
        }
        case __GO_UICONTROL__:
        {
            return import_handle_uicontrol(dataset, parent, version);
        }
    }
    return -1;
}

void update_link_path(int legend, Links::PathList& paths)
{
    //find legend parent axes ( origin of path items )
    int type = 0;
    int* ptype = &type;
    int axes = legend; //start point
    int* paxes = &axes;
    getGraphicObjectProperty(legend, __GO_PARENT_AXES__, jni_int, (void**)&paxes);
    std::vector<int> links;
    //loop on child following path index
    for (auto & path : paths)
    {
        int current = axes;
        for (int j = 0; j < path.size(); ++j)
        {
            int index = path[path.size() - 1 - j];
            int count = 0;
            int* pcount = &count;
            getGraphicObjectProperty(current, __GO_CHILDREN_COUNT__, jni_int, (void**)&pcount);
            if (count == 0 || index >= count)
            {
                getGraphicObjectProperty(current, __GO_TYPE__, jni_int, (void**)&ptype);
                break;
            }

            int* children = nullptr;
            getGraphicObjectProperty(current, __GO_CHILDREN__, jni_int_vector, (void**)&children);

            current = children[index];

            releaseGraphicObjectProperty(__GO_CHILDREN__, children, jni_int_vector, count);
        }

        links.push_back(current);
    }

    setGraphicObjectProperty(legend, __GO_LINKS__, links.data(), jni_int_vector, static_cast<int>(links.size()));
}

static bool getHandleBoolProperty(int uid, int prop, int* data)
{
    int val = 0;
    int* pVal = &val;
    getGraphicObjectProperty(uid, prop, jni_bool, (void **)&pVal);
    if (pVal == nullptr)
    {
        return false;
    }

    *data = val;
    return true;
}

static bool getHandleIntProperty(int uid, int prop, int* data)
{
    int val = 0;
    int* pVal = &val;
    getGraphicObjectProperty(uid, prop, jni_int, (void **)&pVal);
    if (pVal == nullptr)
    {
        return false;
    }

    *data = val;
    return true;
}

static bool getHandleDoubleProperty(int uid, int prop, double* data)
{
    double val = 0;
    double* pVal = &val;
    getGraphicObjectProperty(uid, prop, jni_double, (void **)&pVal);
    if (pVal == nullptr)
    {
        return false;
    }

    *data = val;
    return true;
}

static void getHandleStringProperty(int uid, int prop, char** str)
{
    getGraphicObjectProperty(uid, prop, jni_string, (void **)str);
}


static void getHandleBoolVectorProperty(int uid, int prop, int** vals)
{
    getGraphicObjectProperty(uid, prop, jni_bool_vector, (void **)vals);
}

static void getHandleIntVectorProperty(int uid, int prop, int** vals)
{
    getGraphicObjectProperty(uid, prop, jni_int_vector, (void **)vals);
}

static void getHandleDoubleVectorProperty(int uid, int prop, double** vals)
{
    getGraphicObjectProperty(uid, prop, jni_double_vector, (void **)vals);
}

static void getHandleStringVectorProperty(int uid, int prop, char*** vals)
{
    getGraphicObjectProperty(uid, prop, jni_string_vector, (void **)vals);
}

static bool export_handle_generic(hid_t parent, int uid, const HandleProp& props, hid_t xfer_plist_id);
static bool export_handle_layout_options(hid_t parent, int uid, hid_t xfer_plist_id);
static bool export_handle_userdata(hid_t parent, int uid, hid_t xfer_plist_id);
static bool export_handle_tag(hid_t parent, int uid, hid_t xfer_plist_id);
static bool export_handle_figure(hid_t parent, int uid, hid_t xfer_plist_id);
static bool export_handle_axes(hid_t parent, int uid, hid_t xfer_plist_id);
static bool export_handle_label(hid_t parent, int uid, hid_t xfer_plist_id);
static bool export_handle_champ(hid_t parent, int uid, hid_t xfer_plist_id);
static bool export_handle_children(hid_t parent, int uid, hid_t xfer_plist_id);

static bool export_handle_generic(hid_t parent, int uid, const HandleProp& props, hid_t xfer_plist_id)
{
    for (auto & prop : props)
    {
        const char* name = prop.first.data();
        std::vector<int> info(prop.second);

        //scalar variable
        if (info.size() == 3)
        {
            int go = info[1];
            int type = info[2];

            switch (type)
            {
                case jni_bool:
                {
                    std::vector<int> dims = {1, 1};
                    int val = 0;
                    getHandleBoolProperty(uid, go, &val);
                    writeBooleanMatrix6(parent, name, static_cast<int>(dims.size()), dims.data(), &val, xfer_plist_id);
                    break;
                }
                case jni_double:
                {
                    std::vector<int> dims = {1, 1};
                    double val = 0;
                    getHandleDoubleProperty(uid, go, &val);
                    writeDoubleMatrix6(parent, name, static_cast<int>(dims.size()), dims.data(), &val, xfer_plist_id);
                    break;
                }
                case jni_int:
                {
                    std::vector<int> dims = {1, 1};
                    int val = 0;
                    getHandleIntProperty(uid, go, &val);
                    writeIntegerMatrix6(parent, name, H5T_NATIVE_INT32, "32", static_cast<int>(dims.size()), dims.data(), &val, xfer_plist_id);
                    break;
                }
                case jni_string:
                {
                    std::vector<int> dims = {1, 1};
                    char* val;
                    getHandleStringProperty(uid, go, &val);
                    writeStringMatrix6(parent, name, static_cast<int>(dims.size()), dims.data(), &val, xfer_plist_id);
                    releaseGraphicObjectProperty(go, val, jni_string, 1);
                    break;
                }
            }
        }
        else //vector variable
        {
            int go = info[1];
            int type = info[2];
            int row = info[3];
            int col = info.size() > 3 ? info[4] : -1;

            if (row < 0)
            {
                row = -row;
            }
            else
            {
                getHandleIntProperty(uid, row, &row);
            }

            if (col < 0)
            {
                col = -col;
            }
            else
            {
                getHandleIntProperty(uid, col, &col);
            }

            switch (type)
            {
                case jni_bool_vector:
                {
                    std::vector<int> dims = {row, col};
                    int* vals;
                    getHandleBoolVectorProperty(uid, go, &vals);
                    writeBooleanMatrix6(parent, name, static_cast<int>(dims.size()), dims.data(), vals, xfer_plist_id);
                    releaseGraphicObjectProperty(go, vals, jni_bool_vector, row * col);
                    break;
                }
                case jni_double_vector:
                {
                    std::vector<int> dims = {row, col};
                    double* vals;
                    getHandleDoubleVectorProperty(uid, go, &vals);
                    writeDoubleMatrix6(parent, name, static_cast<int>(dims.size()), dims.data(), vals, xfer_plist_id);
                    releaseGraphicObjectProperty(go, vals, jni_double_vector, row * col);
                    break;
                }
                case jni_int_vector:
                {
                    std::vector<int> dims = {row, col};
                    int* vals;
                    getHandleIntVectorProperty(uid, go, &vals);
                    writeIntegerMatrix6(parent, name, H5T_NATIVE_INT32, "32", static_cast<int>(dims.size()), dims.data(), vals, xfer_plist_id);
                    releaseGraphicObjectProperty(go, vals, jni_int_vector, row * col);
                    break;
                }
                case jni_string_vector:
                {
                    std::vector<int> dims = {row, col};
                    char** vals;
                    getHandleStringVectorProperty(uid, go, &vals);
                    writeStringMatrix6(parent, name, static_cast<int>(dims.size()), dims.data(), vals, xfer_plist_id);
                    releaseGraphicObjectProperty(go, vals, jni_string_vector, row * col);
                    break;
                }
                default:
                {
                    break;
                }
            }
        }
    }

    //user_data
    export_handle_userdata(parent, uid, xfer_plist_id);
    //tag
    export_handle_tag(parent, uid, xfer_plist_id);
    //children
    export_handle_children(parent, uid, xfer_plist_id);
    return true;
}

static bool export_handle_layout_options(hid_t parent, int uid, hid_t xfer_plist_id)
{
    int layout_type = 0;
    getHandleIntProperty(uid, __GO_LAYOUT__, &layout_type);
    if (layout_type == 0 || layout_type == 1) //LAYOUT_NONE or LAYOUT_GRIDBAG
    {
        //nothing to save
        return true;
    }

    hid_t layout = openList6(parent, "layout_options", g_SCILAB_CLASS_HANDLE);

    switch (layout_type)
    {
        case 2: //grid
        {
            std::vector<int> dims = {1, 2};
            int* grid = nullptr;
            getHandleIntVectorProperty(uid, __GO_GRID_OPT_GRID__, &grid);
            writeIntegerMatrix6(layout, "grid", H5T_NATIVE_INT32, "32", 2, dims.data(), grid, xfer_plist_id);
            releaseGraphicObjectProperty(__GO_GRID_OPT_GRID__, grid, jni_int_vector, 2);

            int* pad = nullptr;
            getHandleIntVectorProperty(uid, __GO_GRID_OPT_PADDING__, &pad);
            writeIntegerMatrix6(layout, "padding", H5T_NATIVE_INT32, "32", 2, dims.data(), pad, xfer_plist_id);
            releaseGraphicObjectProperty(__GO_GRID_OPT_PADDING__, pad, jni_int_vector, 2);
            break;
        }
        case 3: //border
        {
            std::vector<int> dims = {1, 2};
            int* pad = nullptr;
            getHandleIntVectorProperty(uid, __GO_BORDER_OPT_PADDING__, &pad);
            writeIntegerMatrix6(layout, "padding", H5T_NATIVE_INT32, "32", 2, dims.data(), pad, xfer_plist_id);
            releaseGraphicObjectProperty(__GO_BORDER_OPT_PADDING__, pad, jni_int_vector, 2);
            break;
        }
    }

    closeList6(layout);

    return true;
}
static bool export_handle_tag(hid_t parent, int uid, hid_t xfer_plist_id)
{
    char* tag = nullptr;
    getHandleStringProperty(uid, __GO_TAG__, &tag);
    int dims[2];
    dims[0] = 1;
    dims[1] = 1;
    writeStringMatrix6(parent, "tag", 2, dims, &tag, xfer_plist_id);
    releaseGraphicObjectProperty(__GO_TAG__, tag, jni_string, 1);
    return true;
}

static bool export_handle_userdata(hid_t parent, int uid, hid_t xfer_plist_id)
{

    int size = 0;
    getHandleIntProperty(uid, __GO_USER_DATA_SIZE__, &size);

    if (size == 0)
    {
        std::vector<int> dims = {0, 0};
        writeDoubleMatrix6(parent, "userdata", 2, dims.data(), NULL, xfer_plist_id);
    }
    else
    {
        int *data = NULL;
        getHandleIntVectorProperty(uid, __GO_USER_DATA__, &data);

        types::InternalType* pUD = nullptr;
        if (size == 1)
        {
            //32 bits
            int* p = (int*)data;
            pUD = ((types::InternalType*) * p);
        }
        else
        {
            //64 bits
            long long* p = (long long*)data;
            pUD = ((types::InternalType*) * p);
        }

        export_data(parent, "userdata", pUD, xfer_plist_id);
        //do not release, data is a reference on data in model
        //releaseGraphicObjectProperty(__GO_USER_DATA__, data, jni_int_vector, size);
    }

    return true;
}

static bool export_handle_datatips(hid_t parent, int uid, hid_t xfer_plist_id)
{
    int count = 0;
    getHandleIntProperty(uid, __GO_DATATIPS_COUNT__, &count);
    hid_t node = openList6(parent, "datatips", g_SCILAB_CLASS_HANDLE);
    int* datatips = nullptr;

    if (count != 0)
    {
        getHandleIntVectorProperty(uid, __GO_DATATIPS__, &datatips);
    }

    for (int i = 0; i < count; ++i)
    {
        if (export_handle(node, std::to_string(i), datatips[i], xfer_plist_id) == false)
        {
            releaseGraphicObjectProperty(__GO_DATATIPS__, datatips, jni_int_vector, count);
            closeList6(node);
            return false;
        }
    }

    releaseGraphicObjectProperty(__GO_DATATIPS__, datatips, jni_int_vector, count);
    closeList6(node);
    return true;
}

static bool export_handle_border(hid_t dataset, int uid, hid_t xfer_plist_id);

static bool export_handle_border_none(hid_t dataset, int uid, hid_t xfer_plist_id)
{
    //nothing to do
    closeList6(dataset);
    return true;
}

static bool export_handle_border_line(hid_t dataset, int uid, hid_t xfer_plist_id)
{
    bool ret = false;
    int dims[2];
    dims[0] = 1;
    dims[1] = 1;

    //color
    char* color = nullptr;
    getHandleStringProperty(uid, __GO_UI_FRAME_BORDER_COLOR__, &color);
    writeStringMatrix6(dataset, "color", 2, dims, &color, xfer_plist_id);
    releaseGraphicObjectProperty(__GO_UI_FRAME_BORDER_COLOR__, color, jni_string, 1);

    //thickness
    int thickness = 0;
    ret = getHandleIntProperty(uid, __GO_LINE_THICKNESS__, &thickness);
    if (ret)
    {
        writeIntegerMatrix6(dataset, "thickness", H5T_NATIVE_INT32, "32", 2, dims, &thickness, xfer_plist_id);
    }

    //rounded
    int rounded = 0;
    ret = getHandleBoolProperty(uid, __GO_UI_FRAME_BORDER_ROUNDED__, &rounded);
    if (ret)
    {
        writeBooleanMatrix6(dataset, "rounded", 2, dims, &rounded, xfer_plist_id);
    }

    closeList6(dataset);
    return true;
}

static bool export_handle_border_bevel(hid_t dataset, int uid, hid_t xfer_plist_id)
{
    bool ret = false;
    int dims[2];
    dims[0] = 1;
    dims[1] = 1;
    char* data = nullptr;

    //type
    int type = 0;
    ret = getHandleIntProperty(uid, __GO_UI_FRAME_BORDER_TYPE__, &type);
    if (ret)
    {
        writeIntegerMatrix6(dataset, "type", H5T_NATIVE_INT32, "32", 2, dims, &type, xfer_plist_id);
    }

    //highlight out
    getHandleStringProperty(uid, __GO_UI_FRAME_BORDER_HIGHLIGHT_OUT__, &data);
    if (data)
    {
        writeStringMatrix6(dataset, "highlight_out", 2, dims, &data, xfer_plist_id);
        releaseGraphicObjectProperty(__GO_UI_FRAME_BORDER_HIGHLIGHT_OUT__, data, jni_string, 1);
    }

    //highlight in
    getHandleStringProperty(uid, __GO_UI_FRAME_BORDER_HIGHLIGHT_IN__, &data);
    if (data)
    {
        writeStringMatrix6(dataset, "highlight_in", 2, dims, &data, xfer_plist_id);
        releaseGraphicObjectProperty(__GO_UI_FRAME_BORDER_HIGHLIGHT_IN__, data, jni_string, 1);
    }

    //shadow out
    getHandleStringProperty(uid, __GO_UI_FRAME_BORDER_SHADOW_OUT__, &data);
    if (data)
    {
        writeStringMatrix6(dataset, "shadow_out", 2, dims, &data, xfer_plist_id);
        releaseGraphicObjectProperty(__GO_UI_FRAME_BORDER_SHADOW_OUT__, data, jni_string, 1);
    }

    //shadow in
    getHandleStringProperty(uid, __GO_UI_FRAME_BORDER_SHADOW_IN__, &data);
    if (data)
    {
        writeStringMatrix6(dataset, "shadow_in", 2, dims, &data, xfer_plist_id);
        releaseGraphicObjectProperty(__GO_UI_FRAME_BORDER_SHADOW_IN__, data, jni_string, 1);
    }
    closeList6(dataset);
    return true;
}

static bool export_handle_border_soft_bevel(hid_t dataset, int uid, hid_t xfer_plist_id)
{
    return export_handle_border_bevel(dataset, uid, xfer_plist_id);
}

static bool export_handle_border_etched(hid_t dataset, int uid, hid_t xfer_plist_id)
{
    bool ret = false;
    int dims[2];
    dims[0] = 1;
    dims[1] = 1;
    char* data = nullptr;

    //type
    int type = 0;
    ret = getHandleIntProperty(uid, __GO_UI_FRAME_BORDER_TYPE__, &type);
    if (ret)
    {
        writeIntegerMatrix6(dataset, "type", H5T_NATIVE_INT32, "32", 2, dims, &type, xfer_plist_id);
    }

    //highlight out
    getHandleStringProperty(uid, __GO_UI_FRAME_BORDER_HIGHLIGHT_OUT__, &data);
    if (data)
    {
        writeStringMatrix6(dataset, "highlight_out", 2, dims, &data, xfer_plist_id);
        releaseGraphicObjectProperty(__GO_UI_FRAME_BORDER_HIGHLIGHT_OUT__, data, jni_string, 1);
    }

    //shadow out
    getHandleStringProperty(uid, __GO_UI_FRAME_BORDER_SHADOW_OUT__, &data);
    if (data)
    {
        writeStringMatrix6(dataset, "shadow_out", 2, dims, &data, xfer_plist_id);
        releaseGraphicObjectProperty(__GO_UI_FRAME_BORDER_SHADOW_OUT__, data, jni_string, 1);
    }

    closeList6(dataset);
    return true;
}

static bool export_handle_border_titled(hid_t dataset, int uid, hid_t xfer_plist_id)
{
    bool ret = false;
    int dims[2];
    dims[0] = 1;
    dims[1] = 1;
    char* data = nullptr;

    //title_border
    int title = 0;
    ret = getHandleIntProperty(uid, __GO_UI_FRAME_BORDER_TITLE__, &title);
    if (ret)
    {
        hid_t node = openList6(dataset, "title_border", g_SCILAB_CLASS_HANDLE);
        export_handle_border(node, title, xfer_plist_id);
    }

    //title
    getHandleStringProperty(uid, __GO_TITLE__, &data);
    if (data)
    {
        writeStringMatrix6(dataset, "title", 2, dims, &data, xfer_plist_id);
        releaseGraphicObjectProperty(__GO_TITLE__, data, jni_string, 1);
    }

    //justification
    int justification = 0;
    ret = getHandleIntProperty(uid, __GO_UI_FRAME_BORDER_JUSTIFICATION__, &justification);
    if (ret)
    {
        writeIntegerMatrix6(dataset, "justification", H5T_NATIVE_INT32, "32", 2, dims, &justification, xfer_plist_id);
    }

    //fontname
    getHandleStringProperty(uid, __GO_UI_FONTNAME__, &data);
    if (data)
    {
        writeStringMatrix6(dataset, "fontname", 2, dims, &data, xfer_plist_id);
        releaseGraphicObjectProperty(__GO_UI_FONTNAME__, data, jni_string, 1);
        data = nullptr;
    }

    //fontangle
    getHandleStringProperty(uid, __GO_UI_FONTANGLE__, &data);
    if (data)
    {
        writeStringMatrix6(dataset, "fontangle", 2, dims, &data, xfer_plist_id);
        releaseGraphicObjectProperty(__GO_UI_FONTANGLE__, data, jni_string, 1);
        data = nullptr;
    }

    //fontsize
    int fonsize = 0;
    ret = getHandleIntProperty(uid, __GO_UI_FONTSIZE__, &fonsize);
    if (ret)
    {
        writeIntegerMatrix6(dataset, "fontsize", H5T_NATIVE_INT32, "32", 2, dims, &fonsize, xfer_plist_id);
    }

    //fontweight
    getHandleStringProperty(uid, __GO_UI_FONTWEIGHT__, &data);
    if (data)
    {
        writeStringMatrix6(dataset, "fontweight", 2, dims, &data, xfer_plist_id);
        releaseGraphicObjectProperty(__GO_UI_FONTWEIGHT__, data, jni_string, 1);
        data = nullptr;
    }

    //position
    int position = 0;
    ret = getHandleIntProperty(uid, __GO_UI_FRAME_BORDER_POSITION__, &position);
    if (ret)
    {
        writeIntegerMatrix6(dataset, "position", H5T_NATIVE_INT32, "32", 2, dims, &position, xfer_plist_id);
    }

    //color
    getHandleStringProperty(uid, __GO_UI_FRAME_BORDER_COLOR__, &data);
    if (data)
    {
        writeStringMatrix6(dataset, "color", 2, dims, &data, xfer_plist_id);
        releaseGraphicObjectProperty(__GO_UI_FRAME_BORDER_COLOR__, data, jni_string, 1);
        data = nullptr;
    }

    closeList6(dataset);
    return true;
}

static bool export_handle_border_empty(hid_t dataset, int uid, hid_t xfer_plist_id)
{
    int dims[2];
    dims[0] = 1;
    dims[1] = 4;
    double* pos = nullptr;

    //position
    getHandleDoubleVectorProperty(uid, __GO_POSITION__, &pos);
    if (pos)
    {
        writeDoubleMatrix6(dataset, "position", 2, dims, pos, xfer_plist_id);
        releaseGraphicObjectProperty(__GO_POSITION__, pos, jni_double_vector, 4);
    }

    closeList6(dataset);
    return true;
}

static bool export_handle_border_compound(hid_t dataset, int uid, hid_t xfer_plist_id)
{
    bool ret = false;
    //out_border
    int out_border = 0;

    ret = getHandleIntProperty(uid, __GO_UI_FRAME_BORDER_OUT_BORDER__, &out_border);
    if (ret)
    {
        hid_t node = openList6(dataset, "out_border", g_SCILAB_CLASS_HANDLE);
        export_handle_border(node, out_border, xfer_plist_id);

        //title_border
        int in_border = 0;
        getHandleIntProperty(uid, __GO_UI_FRAME_BORDER_IN_BORDER__, &in_border);
        node = openList6(dataset, "in_border", g_SCILAB_CLASS_HANDLE);
        export_handle_border(node, in_border, xfer_plist_id);
    }

    closeList6(dataset);
    return true;
}

static bool export_handle_border_matte(hid_t dataset, int uid, hid_t xfer_plist_id)
{
    int dims[2];
    dims[0] = 1;
    dims[1] = 4;
    char* data = nullptr;
    double* pos = nullptr;

    //position
    dims[0] = 1;
    dims[1] = 4;
    getHandleDoubleVectorProperty(uid, __GO_POSITION__, &pos);
    writeDoubleMatrix6(dataset, "position", 2, dims, pos, xfer_plist_id);
    releaseGraphicObjectProperty(__GO_POSITION__, pos, jni_double_vector, 4);

    //color
    dims[0] = 1;
    dims[1] = 1;
    getHandleStringProperty(uid, __GO_UI_FRAME_BORDER_COLOR__, &data);
    writeStringMatrix6(dataset, "color", 2, dims, &data, xfer_plist_id);
    releaseGraphicObjectProperty(__GO_UI_FRAME_BORDER_COLOR__, data, jni_string, 1);

    closeList6(dataset);
    return true;
}

static bool export_handle_border(hid_t dataset, int uid, hid_t xfer_plist_id)
{
    int style = 0;
    getHandleIntProperty(uid, __GO_UI_FRAME_BORDER_STYLE__, &style);
    int dims[2];
    dims[0] = 1;
    dims[1] = 1;

    writeIntegerMatrix6(dataset, "style", H5T_NATIVE_INT32, "32", 2, dims, &style, xfer_plist_id);

    switch (style)
    {
        default:
        case NONE:
            return export_handle_border_none(dataset, uid, xfer_plist_id);
        case LINE:
            return export_handle_border_line(dataset, uid, xfer_plist_id);
        case BEVEL:
            return export_handle_border_bevel(dataset, uid, xfer_plist_id);
        case SOFTBEVEL:
            return export_handle_border_soft_bevel(dataset, uid, xfer_plist_id);
        case ETCHED:
            return export_handle_border_etched(dataset, uid, xfer_plist_id);
        case TITLED:
            return export_handle_border_titled(dataset, uid, xfer_plist_id);
        case EMPTY:
            return export_handle_border_empty(dataset, uid, xfer_plist_id);
        case COMPOUND:
            return export_handle_border_compound(dataset, uid, xfer_plist_id);
        case MATTE:
            return export_handle_border_matte(dataset, uid, xfer_plist_id);
    }
}

static bool export_handle_uicontrol(hid_t parent, int uid, hid_t xfer_plist_id)
{
    bool ret = false;
    if (export_handle_generic(parent, uid, UicontrolHandle::getPropertyList(), xfer_plist_id) == false)
    {
        return false;
    }

    //string
    int size = 0;
    getHandleIntProperty(uid, __GO_UI_STRING_SIZE__, &size);
    int col = 0;
    getHandleIntProperty(uid, __GO_UI_STRING_COLNB__, &col);

    int dims[2];

    if (col == 0)
    {
        dims[0] = 1;
        dims[1] = 1;
        char null_char = '\0';
        char* empty = &null_char;
        writeStringMatrix6(parent, "string", 2, dims, &empty, xfer_plist_id);
    }
    else
    {
        int row = size / col;
        dims[0] = row;
        dims[1] = col;
        char** string = nullptr;
        getHandleStringVectorProperty(uid, __GO_UI_STRING__, &string);
        writeStringMatrix6(parent, "string", 2, dims, string, xfer_plist_id);
        releaseGraphicObjectProperty(__GO_UI_STRING__, string, jni_string_vector, size);
    }

    //border
    int border = 0;
    ret = getHandleIntProperty(uid, __GO_UI_FRAME_BORDER__, &border);
    if (ret)
    {
        hid_t ub = openList6(parent, "border", g_SCILAB_CLASS_HANDLE);
        export_handle_border(ub, border, xfer_plist_id);
    }

    closeList6(parent);
    return true;
}

static bool export_handle_uicontextmenu(hid_t parent, int uid, hid_t xfer_plist_id)
{
    if (export_handle_generic(parent, uid, UicontextmenuHandle::getPropertyList(), xfer_plist_id) == false)
    {
        return false;
    }

    closeList6(parent);
    return true;
}

static bool export_handle_uimenu(hid_t parent, int uid, hid_t xfer_plist_id)
{
    if (export_handle_generic(parent, uid, UimenuHandle::getPropertyList(), xfer_plist_id) == false)
    {
        return false;
    }

    closeList6(parent);
    return true;
}

static bool export_handle_light(hid_t parent, int uid, hid_t xfer_plist_id)
{
    if (export_handle_generic(parent, uid, LightHandle::getPropertyList(), xfer_plist_id) == false)
    {
        return false;
    }

    closeList6(parent);
    return true;
}

static bool export_handle_axis(hid_t parent, int uid, hid_t xfer_plist_id)
{
    if (export_handle_generic(parent, uid, AxisHandle::getPropertyList(), xfer_plist_id) == false)
    {
        return false;
    }

    closeList6(parent);
    return true;
}

static bool export_handle_text(hid_t parent, int uid, hid_t xfer_plist_id)
{
    if (export_handle_generic(parent, uid, TextHandle::getPropertyList(), xfer_plist_id) == false)
    {
        return false;
    }

    //text
    int* dims = nullptr;
    getHandleIntVectorProperty(uid, __GO_TEXT_ARRAY_DIMENSIONS__, &dims);
    char** text;
    char** interpreter;
    getHandleStringVectorProperty(uid, __GO_TEXT_STRINGS__, &text);
    writeStringMatrix6(parent, "text", 2, dims, text, xfer_plist_id);
    getHandleStringVectorProperty(uid, __GO_TEXT_INTERPRETERS__, &interpreter);
    writeStringMatrix6(parent, "interpreter", 2, dims, interpreter, xfer_plist_id);

    closeList6(parent);
    return true;
}

//find parent axes of a entity and return path ( via children index )
bool get_entity_path(int entity, std::vector<int>& path)
{
    path.clear();

    while (true)
    {
        int parent = 0;
        getHandleIntProperty(entity, __GO_PARENT__, &parent);
        int count = 0;
        getHandleIntProperty(parent, __GO_CHILDREN_COUNT__, &count);
        //get children of parent to find "my" index
        int* children = nullptr;
        getHandleIntVectorProperty(parent, __GO_CHILDREN__, &children);
        bool find = false;
        for (int i = 0; i < count; ++i)
        {
            if (children[i] == entity)
            {
                path.push_back(i);
                find = true;
                break;
            }
        }

        releaseGraphicObjectProperty(__GO_CHILDREN__, children, jni_int_vector, count);
        if (find == false)
        {
            return false;
        }

        int type = 0;
        getHandleIntProperty(parent, __GO_TYPE__, &type);
        if (type == __GO_AXES__)
        {
            break;
        }

        entity = parent;
    }

    return true;
}

static bool export_handle_legend(hid_t parent, int uid, hid_t xfer_plist_id)
{
    if (export_handle_generic(parent, uid, LegendHandle::getPropertyList(), xfer_plist_id) == false)
    {
        return false;
    }

    //links
    hid_t node = openList6(parent, "links", g_SCILAB_CLASS_HANDLE);
    int link = 0;
    getHandleIntProperty(uid, __GO_LINKS_COUNT__, &link);
    int* links = nullptr;
    getHandleIntVectorProperty(uid, __GO_LINKS__, &links);
    for (int i = 0; i < link; ++i)
    {
        std::vector<int> path;
        if (get_entity_path(links[i], path))
        {
            int dims[2];
            dims[0] = 1;
            dims[1] = static_cast<int>(path.size());
            writeIntegerMatrix6(node, std::to_string(i).data(), H5T_NATIVE_INT32, "32", 2, dims, path.data(), xfer_plist_id);
        }
    }

    releaseGraphicObjectProperty(__GO_LINKS__, links, jni_int_vector, link);
    closeList6(node);

    //text
    int* dims = nullptr;
    getHandleIntVectorProperty(uid, __GO_TEXT_ARRAY_DIMENSIONS__, &dims);
    char** text;
    getHandleStringVectorProperty(uid, __GO_TEXT_STRINGS__, &text);
    writeStringMatrix6(parent, "text", 2, dims, text, xfer_plist_id);

    //interpreters
    char** interpreters;
    getHandleStringVectorProperty(uid, __GO_TEXT_INTERPRETERS__, &interpreters);
    writeStringMatrix6(parent, "interpreter", 2, dims, interpreters, xfer_plist_id);

    closeList6(parent);
    return true;
}

static bool export_handle_fec(hid_t parent, int uid, hid_t xfer_plist_id)
{
    if (export_handle_generic(parent, uid, FecHandle::getPropertyList(), xfer_plist_id) == false)
    {
        return false;
    }

    //triangles
    int indices = 0;
    getHandleIntProperty(uid, __GO_DATA_MODEL_NUM_INDICES__, &indices);
    int vect = 0;
    getHandleIntProperty(uid, __GO_DATA_MODEL_NUM_VERTICES_BY_ELEM__, &vect);
    double* triangles = nullptr;
    getHandleDoubleVectorProperty(uid, __GO_DATA_MODEL_FEC_ELEMENTS__, &triangles);

    int dims[2];
    dims[0] = indices;
    dims[1] = vect + 2;
    writeDoubleMatrix6(parent, "triangles", 2, dims, triangles, xfer_plist_id);
    releaseGraphicObjectProperty(__GO_DATA_MODEL_FEC_ELEMENTS__, triangles, jni_double_vector, dims[0] * dims[1]);

    closeList6(parent);
    return true;
}

static bool export_handle_matplot(hid_t parent, int uid, hid_t xfer_plist_id)
{
    if (export_handle_generic(parent, uid, MatplotHandle::getPropertyList(), xfer_plist_id) == false)
    {
        return false;
    }

    int row = 0;
    int col = 0;
    getHandleIntProperty(uid, __GO_DATA_MODEL_NUM_X__, &col);
    getHandleIntProperty(uid, __GO_DATA_MODEL_NUM_Y__, &row);
    int datatype = 0;
    getHandleIntProperty(uid, __GO_DATA_MODEL_MATPLOT_DATA_TYPE__, &datatype);
    int imagetype = 0;
    getHandleIntProperty(uid, __GO_DATA_MODEL_MATPLOT_IMAGE_TYPE__, &imagetype);
    int size = (row - 1) * (col - 1);

    //data can be char, uchar, short, ushort, ... hide in a double*
    //save double like this but need to compute exact dimensions to
    //store it

    double* data = nullptr;
    getHandleDoubleVectorProperty(uid, __GO_DATA_MODEL_Z__, &data);

    switch (datatype)
    {
        case MATPLOT_Char:
        case MATPLOT_HM1_Char:
        case MATPLOT_HM1_UChar:
            size /= (sizeof(double) / sizeof(char));
            break;
        case MATPLOT_HM3_Char:
        case MATPLOT_HM3_UChar:
            size /= (sizeof(double) / sizeof(char));
            size *= 3;
            break;

        case MATPLOT_HM3_Double:
            size *= 3;
            break;
        case MATPLOT_HM4_Char:
        case MATPLOT_HM4_UChar:
            size /= (sizeof(double) / sizeof(char));
            size *= 4;
            break;
        case MATPLOT_HM4_Double:
            size *= 4;
            break;
        case MATPLOT_UChar:
            size /= (sizeof(double) / sizeof(char));
            if ((ImageType)imagetype == MATPLOT_RGB)
            {
                size *= 3;
            }
            else if ((GLType)imagetype == MATPLOT_GL_RGBA)
            {
                size *= 4;
            }
            break;
        case MATPLOT_Int:
        case MATPLOT_UInt:
            size /= (sizeof(double) / sizeof(int));
            break;
        case MATPLOT_Short:
        case MATPLOT_UShort:
            size /= (sizeof(double) / sizeof(short));
            break;
        case MATPLOT_Double:
        case MATPLOT_HM1_Double:
        default:
            break;
    }

    int dims[2];
    dims[0] = 1;
    dims[1] = size;

    writeDoubleMatrix6(parent, "data", 2, dims, data, xfer_plist_id);
    releaseGraphicObjectProperty(__GO_DATA_MODEL_Z__, data, jni_double_vector, size);
    closeList6(parent);
    return true;
}

static bool export_handle_grayplot(hid_t parent, int uid, hid_t xfer_plist_id)
{
    if (export_handle_generic(parent, uid, GrayplotHandle::getPropertyList(), xfer_plist_id) == false)
    {
        return false;
    }

    int row = 0;
    getHandleIntProperty(uid, __GO_DATA_MODEL_NUM_X__, &row);
    int col = 0;
    getHandleIntProperty(uid, __GO_DATA_MODEL_NUM_Y__, &col);

    double* dataX = nullptr;
    double* dataY = nullptr;
    double* dataZ = nullptr;

    getHandleDoubleVectorProperty(uid, __GO_DATA_MODEL_X__, &dataX);
    getHandleDoubleVectorProperty(uid, __GO_DATA_MODEL_Y__, &dataY);
    getHandleDoubleVectorProperty(uid, __GO_DATA_MODEL_Z__, &dataZ);

    int dims[2];
    dims[0] = 1;
    dims[1] = row;
    writeDoubleMatrix6(parent, "data_x", 2, dims, dataX, xfer_plist_id);

    dims[0] = 1;
    dims[1] = col;
    writeDoubleMatrix6(parent, "data_y", 2, dims, dataY, xfer_plist_id);

    dims[0] = row;
    dims[1] = col;
    writeDoubleMatrix6(parent, "data_z", 2, dims, dataZ, xfer_plist_id);

    closeList6(parent);
    return true;
}

static bool export_handle_segs(hid_t parent, int uid, hid_t xfer_plist_id)
{
    if (export_handle_generic(parent, uid, SegsHandle::getPropertyList(), xfer_plist_id) == false)
    {
        return false;
    }

    closeList6(parent);
    return true;
}

static bool export_handle_arc(hid_t parent, int uid, hid_t xfer_plist_id)
{
    if (export_handle_generic(parent, uid, ArcHandle::getPropertyList(), xfer_plist_id) == false)
    {
        return false;
    }

    closeList6(parent);
    return true;
}

static bool export_handle_rectangle(hid_t parent, int uid, hid_t xfer_plist_id)
{
    if (export_handle_generic(parent, uid, RectangleHandle::getPropertyList(), xfer_plist_id) == false)
    {
        return false;
    }

    closeList6(parent);
    return true;
}

static bool export_handle_datatip(hid_t parent, int uid, hid_t xfer_plist_id)
{
    if (export_handle_generic(parent, uid, DatatipHandle::getPropertyList(), xfer_plist_id) == false)
    {
        return false;
    }

    double *dblIndex = nullptr;
    int dims[2] = {1,2};
    getHandleDoubleVectorProperty(uid, __GO_DATATIP_INDEXES__, &dblIndex);
    writeDoubleMatrix6(parent,"data_index", 2, dims, dblIndex, xfer_plist_id);    

    int* dims_interp = nullptr;
    getHandleIntVectorProperty(uid, __GO_TEXT_ARRAY_DIMENSIONS__, &dims_interp);
    char** interpreters;
    getHandleStringVectorProperty(uid, __GO_TEXT_INTERPRETERS__, &interpreters);
    writeStringMatrix6(parent, "interpreter", 2, dims_interp, interpreters, xfer_plist_id);

    closeList6(parent);
    return true;
}

static bool export_handle_polyline_shift(hid_t parent, int uid, const std::string& name, int go_set, int go_data, hid_t xfer_plist_id)
{
    int set = 0;
    getHandleBoolProperty(uid, go_set, &set);
    if (set)
    {
        int count = 0;
        getHandleIntProperty(uid, __GO_DATA_MODEL_NUM_ELEMENTS__, &count);
        double* data = nullptr;
        getHandleDoubleVectorProperty(uid, go_data, &data);

        int dims[2];
        dims[0] = 1;
        dims[1] = count;
        writeDoubleMatrix6(parent, name.data(), 2, dims, data, xfer_plist_id);

        //data from __GO_DATA_MODEL_XXX does not need to be delete
        //releaseGraphicObjectProperty(uid, data, jni_double_vector, count);
    }
    else
    {
        int dims[2];
        dims[0] = 0;
        dims[1] = 0;
        writeDoubleMatrix6(parent, name.data(), 2, dims, NULL, xfer_plist_id);
    }

    return true;
}

static bool export_handle_polyline(hid_t parent, int uid, hid_t xfer_plist_id)
{
    if (export_handle_datatips(parent, uid, xfer_plist_id) == false)
    {
        return false;
    }

    if (export_handle_generic(parent, uid, PolylineHandle::getPropertyList(), xfer_plist_id) == false)
    {
        return false;
    }

    //x_shift
    export_handle_polyline_shift(parent, uid, "x_shift", __GO_DATA_MODEL_X_COORDINATES_SHIFT_SET__, __GO_DATA_MODEL_X_COORDINATES_SHIFT__, xfer_plist_id);
    //y_shift
    export_handle_polyline_shift(parent, uid, "y_shift", __GO_DATA_MODEL_Y_COORDINATES_SHIFT_SET__, __GO_DATA_MODEL_Y_COORDINATES_SHIFT__, xfer_plist_id);
    //z_shift
    export_handle_polyline_shift(parent, uid, "z_shift", __GO_DATA_MODEL_Z_COORDINATES_SHIFT_SET__, __GO_DATA_MODEL_Z_COORDINATES_SHIFT__, xfer_plist_id);

    //interp_color_vector
    int set = 0;
    getHandleBoolProperty(uid, __GO_INTERP_COLOR_VECTOR_SET__, &set);
    if (set)
    {
        int count = 0;
        getHandleIntProperty(uid, __GO_DATA_MODEL_NUM_ELEMENTS__, &count);
        int* data = nullptr;
        getHandleIntVectorProperty(uid, __GO_INTERP_COLOR_VECTOR__, &data);

        int dims[2];
        dims[0] = 1;
        dims[1] = count;
        writeIntegerMatrix6(parent, "interp_color_vector", H5T_NATIVE_INT32, "32", 2, dims, data, xfer_plist_id);
        releaseGraphicObjectProperty(uid, data, jni_int_vector, count);
    }
    else
    {
        int dims[2];
        dims[0] = 0;
        dims[1] = 0;
        writeIntegerMatrix6(parent, "interp_color_vector", H5T_NATIVE_INT32, "32", 2, dims, NULL, xfer_plist_id);
    }

    //data
    int count = 0;
    getHandleIntProperty(uid, __GO_DATA_MODEL_NUM_ELEMENTS__, &count);

    int dims[2];
    dims[0] = 1;
    dims[1] = count;

    double* dataX = nullptr;
    double* dataY = nullptr;
    getHandleDoubleVectorProperty(uid, __GO_DATA_MODEL_X__, &dataX);
    getHandleDoubleVectorProperty(uid, __GO_DATA_MODEL_Y__, &dataY);

    writeDoubleMatrix6(parent, "data_x", 2, dims, dataX, xfer_plist_id);
    writeDoubleMatrix6(parent, "data_y", 2, dims, dataY, xfer_plist_id);

    releaseGraphicObjectProperty(__GO_DATA_MODEL_X__, dataX, jni_double_vector, count);
    releaseGraphicObjectProperty(__GO_DATA_MODEL_Y__, dataY, jni_double_vector, count);

    getHandleIntProperty(uid, __GO_DATA_MODEL_Z_COORDINATES_SET__, &set);
    if (set)
    {
        double* dataZ = nullptr;
        getHandleDoubleVectorProperty(uid, __GO_DATA_MODEL_Z__, &dataZ);
        writeDoubleMatrix6(parent, "data_z", 2, dims, dataZ, xfer_plist_id);
        releaseGraphicObjectProperty(__GO_DATA_MODEL_Z__, dataZ, jni_double_vector, count);
    }
    else
    {
        //[]
        dims[0] = 0;
        dims[1] = 0;
        writeDoubleMatrix6(parent, "data_z", 2, dims, NULL, xfer_plist_id);
    }

    closeList6(parent);
    return true;
}

static bool export_handle_surface(hid_t parent, int uid, hid_t xfer_plist_id)
{
    return export_handle_generic(parent, uid, SurfaceHandle::getPropertyList(), xfer_plist_id);
}

static bool export_handle_plot3d(hid_t parent, int uid, hid_t xfer_plist_id)
{
    bool ret = export_handle_surface(parent, uid, xfer_plist_id);
    if (ret)
    {
        double* dataX = NULL;
        double* dataY = NULL;
        double* dataZ = NULL;

        //data
        getHandleDoubleVectorProperty(uid, __GO_DATA_MODEL_X__, &dataX);
        getHandleDoubleVectorProperty(uid, __GO_DATA_MODEL_Y__, &dataY);
        getHandleDoubleVectorProperty(uid, __GO_DATA_MODEL_Z__, &dataZ);

        int row = 0;
        getHandleIntProperty(uid, __GO_DATA_MODEL_NUM_X__, &row);
        int col = 0;
        getHandleIntProperty(uid, __GO_DATA_MODEL_NUM_Y__, &col);

        int* xDims = nullptr;
        int* yDims = nullptr;
        getHandleIntVectorProperty(uid, __GO_DATA_MODEL_X_DIMENSIONS__, &xDims);
        getHandleIntVectorProperty(uid, __GO_DATA_MODEL_Y_DIMENSIONS__, &yDims);

        int dims[2];
        dims[0] = xDims[0];
        dims[1] = xDims[1];
        writeDoubleMatrix6(parent, "data_x", 2, dims, dataX, xfer_plist_id);
        releaseGraphicObjectProperty(__GO_DATA_MODEL_X__, dataX, jni_double_vector, dims[0] * dims[1]);

        dims[0] = yDims[0];
        dims[1] = yDims[1];
        writeDoubleMatrix6(parent, "data_y", 2, dims, dataY, xfer_plist_id);
        releaseGraphicObjectProperty(__GO_DATA_MODEL_Y__, dataY, jni_double_vector, dims[0] * dims[1]);

        dims[0] = row;
        dims[1] = col;
        writeDoubleMatrix6(parent, "data_z", 2, dims, dataZ, xfer_plist_id);
        releaseGraphicObjectProperty(__GO_DATA_MODEL_Z__, dataZ, jni_double_vector, dims[0] * dims[1]);

        releaseGraphicObjectProperty(__GO_DATA_MODEL_X_DIMENSIONS__, xDims, jni_int_vector, 2);
        releaseGraphicObjectProperty(__GO_DATA_MODEL_Y_DIMENSIONS__, dataZ, jni_int_vector, 2);
    }

    closeList6(parent);
    return ret;
}

static bool export_handle_fac3d(hid_t parent, int uid, hid_t xfer_plist_id)
{
    bool ret = export_handle_surface(parent, uid, xfer_plist_id);
    if (ret)
    {
        double* colors = NULL;
        double* dataX = NULL;
        double* dataY = NULL;
        double* dataZ = NULL;

        //data
        getHandleDoubleVectorProperty(uid, __GO_DATA_MODEL_X__, &dataX);
        getHandleDoubleVectorProperty(uid, __GO_DATA_MODEL_Y__, &dataY);
        getHandleDoubleVectorProperty(uid, __GO_DATA_MODEL_Z__, &dataZ);

        int row = 0;
        getHandleIntProperty(uid, __GO_DATA_MODEL_NUM_VERTICES_PER_GON__, &row);
        int col = 0;
        getHandleIntProperty(uid, __GO_DATA_MODEL_NUM_GONS__, &col);

        int dims[2];
        dims[0] = row;
        dims[1] = col;

        writeDoubleMatrix6(parent, "data_x", 2, dims, dataX, xfer_plist_id);
        writeDoubleMatrix6(parent, "data_y", 2, dims, dataY, xfer_plist_id);
        writeDoubleMatrix6(parent, "data_z", 2, dims, dataZ, xfer_plist_id);

        releaseGraphicObjectProperty(__GO_DATA_MODEL_X__, dataX, jni_double_vector, dims[0] * dims[1]);
        releaseGraphicObjectProperty(__GO_DATA_MODEL_Y__, dataY, jni_double_vector, dims[0] * dims[1]);
        releaseGraphicObjectProperty(__GO_DATA_MODEL_Z__, dataZ, jni_double_vector, dims[0] * dims[1]);

        getHandleDoubleVectorProperty(uid, __GO_DATA_MODEL_COLORS__, &colors);
        if (colors)
        {
            int numColors = 0;
            getHandleIntProperty(uid, __GO_DATA_MODEL_NUM_COLORS__, &numColors);
            if (numColors == col)
            {
                dims[0] = 1;
            }
            else
            {
                dims[0] = row;
            }

            dims[1] = col;
        }
        else
        {
            //export []
            dims[0] = 0;
            dims[1] = 0;
        }

        writeDoubleMatrix6(parent, "colors", 2, dims, colors, xfer_plist_id);
        releaseGraphicObjectProperty(__GO_DATA_MODEL_COLORS__, colors, jni_double_vector, dims[0] * dims[1]);

        //cdata_mapping
        int cdata = 0;
        getHandleIntProperty(uid, __GO_DATA_MAPPING__, &cdata);
        dims[0] = 1;
        dims[1] = 1;
        writeIntegerMatrix6(parent, "cdata_mapping", H5T_NATIVE_INT32, "32", 2, dims, &cdata, xfer_plist_id);

        dims[0] = 1;
        dims[1] = 2;
        // cdata_bounds (2025.0.0)
        double* pdblVals;
        getHandleDoubleVectorProperty(uid, __GO_CDATA_BOUNDS__, &pdblVals);
        writeDoubleMatrix6(parent, "cdata_bounds", 2, dims, pdblVals, xfer_plist_id);
        releaseGraphicObjectProperty(__GO_CDATA_BOUNDS__, pdblVals, jni_double_vector, 2);

        // color_range (2025.0.0)
        int* piVals;
        getHandleIntVectorProperty(uid, __GO_COLOR_RANGE__, &piVals);
        writeIntegerMatrix6(parent, "color_range", H5T_NATIVE_INT32, "32", 2, dims, piVals, xfer_plist_id);
        releaseGraphicObjectProperty(__GO_COLOR_RANGE__, piVals, jni_int_vector, 2);
    }

    closeList6(parent);
    return ret;
}


static bool export_handle_champ(hid_t parent, int uid, hid_t xfer_plist_id)
{
    if (export_handle_generic(parent, uid, ChampHandle::getPropertyList(), xfer_plist_id) == false)
    {
        return false;
    }

    //data
    int* dimensions = NULL;
    double* arrowBasesX = NULL;
    double* arrowBasesY = NULL;
    double* arrowDirectionsX = NULL;
    double* arrowDirectionsY = NULL;
    int dims[2];
    getHandleIntVectorProperty(uid, __GO_CHAMP_DIMENSIONS__, &dimensions);

    //base X
    getHandleDoubleVectorProperty(uid, __GO_BASE_X__, &arrowBasesX);
    dims[0] = 1;
    dims[1] = dimensions[0];
    writeDoubleMatrix6(parent, "base_x", 2, dims, arrowBasesX, xfer_plist_id);
    releaseGraphicObjectProperty(__GO_BASE_X__, arrowBasesX, jni_double_vector, dims[1]);

    //base y
    getHandleDoubleVectorProperty(uid, __GO_BASE_Y__, &arrowBasesY);
    dims[0] = 1;
    dims[1] = dimensions[1];
    writeDoubleMatrix6(parent, "base_y", 2, dims, arrowBasesY, xfer_plist_id);
    releaseGraphicObjectProperty(__GO_BASE_Y__, arrowBasesY, jni_double_vector, dims[1]);

    //direction x
    getHandleDoubleVectorProperty(uid, __GO_DIRECTION_X__, &arrowDirectionsX);
    dims[0] = dimensions[0];
    dims[1] = dimensions[1];
    writeDoubleMatrix6(parent, "direction_x", 2, dims, arrowDirectionsX, xfer_plist_id);
    releaseGraphicObjectProperty(__GO_DIRECTION_X__, arrowDirectionsX, jni_double_vector, dims[0] * dims[1]);

    //direction y
    getHandleDoubleVectorProperty(uid, __GO_DIRECTION_Y__, &arrowDirectionsY);
    dims[0] = dimensions[0];
    dims[1] = dimensions[1];
    writeDoubleMatrix6(parent, "direction_y", 2, dims, arrowDirectionsY, xfer_plist_id);
    releaseGraphicObjectProperty(__GO_DIRECTION_Y__, arrowDirectionsY, jni_double_vector, dims[0] * dims[1]);

    releaseGraphicObjectProperty(__GO_CHAMP_DIMENSIONS__, dimensions, jni_int_vector, 2);
    closeList6(parent);
    return true;
}
static bool export_handle_label(hid_t parent, int uid, hid_t xfer_plist_id)
{
    if (export_handle_generic(parent, uid, LabelHandle::getPropertyList(), xfer_plist_id) == false)
    {
        return false;
    }

    //text
    int* dimensions = nullptr;
    char** text = nullptr;
    char** interpreters = nullptr;

    getHandleIntVectorProperty(uid, __GO_TEXT_ARRAY_DIMENSIONS__, &dimensions);
    getHandleStringVectorProperty(uid, __GO_TEXT_STRINGS__, &text);
    getHandleStringVectorProperty(uid, __GO_TEXT_INTERPRETERS__, &interpreters);

    std::vector<int> dims = {dimensions[0], dimensions[1]};
    releaseGraphicObjectProperty(__GO_TEXT_ARRAY_DIMENSIONS__, dimensions, jni_int_vector, 2);

    writeStringMatrix6(parent, "text", 2, dims.data(), text, xfer_plist_id);
    releaseGraphicObjectProperty(__GO_TEXT_STRINGS__, text, jni_string_vector, dims[0] * dims[1]);
    writeStringMatrix6(parent, "interpreter", 2, dims.data(), interpreters, xfer_plist_id);
    releaseGraphicObjectProperty(__GO_TEXT_INTERPRETERS__, interpreters, jni_string_vector, dims[0] * dims[1]);

    closeList6(parent);
    return true;
}
static bool export_handle_axes(hid_t parent, int uid, hid_t xfer_plist_id)
{
    if (export_handle_generic(parent, uid, AxesHandle::getPropertyList(), xfer_plist_id) == false)
    {
        return false;
    }

    //title
    int title = 0;
    getHandleIntProperty(uid, __GO_TITLE__, &title);
    export_handle(parent, "title", title, xfer_plist_id);

    //x_label
    int x_label = 0;
    getHandleIntProperty(uid, __GO_X_AXIS_LABEL__, &x_label);
    export_handle(parent, "x_label", x_label, xfer_plist_id);

    //y_label
    int y_label = 0;
    getHandleIntProperty(uid, __GO_Y_AXIS_LABEL__, &y_label);
    export_handle(parent, "y_label", y_label, xfer_plist_id);

    //z_label
    int z_label = 0;
    getHandleIntProperty(uid, __GO_Z_AXIS_LABEL__, &z_label);
    export_handle(parent, "z_label", z_label, xfer_plist_id);


    closeList6(parent);
    return true;
}

static bool export_handle_figure(hid_t parent, int uid, hid_t xfer_plist_id)
{
    if (export_handle_generic(parent, uid, FigureHandle::getPropertyList(), xfer_plist_id) == false)
    {
        return false;
    }

    //layout_options
    export_handle_layout_options(parent, uid, xfer_plist_id);

    closeList6(parent);
    return true;
}

static bool export_handle_compound(hid_t parent, int uid, hid_t xfer_plist_id)
{
    if (export_handle_generic(parent, uid, CompoundHandle::getPropertyList(), xfer_plist_id) == false)
    {
        return false;
    }

    closeList6(parent);
    return true;
}

bool export_handle(hid_t parent, const std::string& name, int uid, hid_t xfer_plist_id)
{
    //get handle type
    int type = 0;
    getHandleIntProperty(uid, __GO_TYPE__, &type);

    //create handle node in __refs__
    hid_t h = openList6(parent, name.data(), g_SCILAB_CLASS_HANDLE);

    bool ret = false;
    switch (type)
    {
        case __GO_FIGURE__:
        {
            ret = export_handle_figure(h, uid, xfer_plist_id);
            break;
        }
        case __GO_AXES__:
        {
            ret = export_handle_axes(h, uid, xfer_plist_id);
            break;
        }
        case __GO_LABEL__:
        {
            ret = export_handle_label(h, uid, xfer_plist_id);
            break;
        }
        case __GO_CHAMP__:
        {
            ret = export_handle_champ(h, uid, xfer_plist_id);
            break;
        }
        case __GO_FAC3D__:
        {
            ret = export_handle_fac3d(h, uid, xfer_plist_id);
            break;
        }
        case __GO_PLOT3D__:
        {
            ret = export_handle_plot3d(h, uid, xfer_plist_id);
            break;
        }
        case __GO_POLYLINE__:
        {
            ret = export_handle_polyline(h, uid, xfer_plist_id);
            break;
        }
        case __GO_DATATIP__:
        {
            ret = export_handle_datatip(h, uid, xfer_plist_id);
            break;
        }
        case __GO_COMPOUND__:
        {
            ret = export_handle_compound(h, uid, xfer_plist_id);
            break;
        }
        case __GO_RECTANGLE__:
        {
            ret = export_handle_rectangle(h, uid, xfer_plist_id);
            break;
        }
        case __GO_ARC__:
        {
            ret = export_handle_arc(h, uid, xfer_plist_id);
            break;
        }
        case __GO_SEGS__:
        {
            ret = export_handle_segs(h, uid, xfer_plist_id);
            break;
        }
        case __GO_GRAYPLOT__:
        {
            ret = export_handle_grayplot(h, uid, xfer_plist_id);
            break;
        }
        case __GO_MATPLOT__:
        {
            ret = export_handle_matplot(h, uid, xfer_plist_id);
            break;
        }
        case __GO_FEC__:
        {
            ret = export_handle_fec(h, uid, xfer_plist_id);
            break;
        }
        case __GO_LEGEND__:
        {
            ret = export_handle_legend(h, uid, xfer_plist_id);
            break;
        }
        case __GO_TEXT__:
        {
            ret = export_handle_text(h, uid, xfer_plist_id);
            break;
        }
        case __GO_AXIS__:
        {
            ret = export_handle_axis(h, uid, xfer_plist_id);
            break;
        }
        case __GO_LIGHT__:
        {
            ret = export_handle_light(h, uid, xfer_plist_id);
            break;
        }
        case __GO_UIMENU__:
        {
            ret = export_handle_uimenu(h, uid, xfer_plist_id);
            break;
        }
        case __GO_UICONTEXTMENU__:
        {
            ret = export_handle_uicontextmenu(h, uid, xfer_plist_id);
            break;
        }
        case __GO_UICONTROL__:
        {
            ret = export_handle_uicontrol(h, uid, xfer_plist_id);
            break;
        }
        default:
        {
        }

    }

    return ret;
}

static bool export_handle_children(hid_t parent, int uid, hid_t xfer_plist_id)
{
    int count = 0;
    getHandleIntProperty(uid, __GO_CHILDREN_COUNT__, &count);
    hid_t node = openList6(parent, "children", g_SCILAB_CLASS_HANDLE);
    int* children = nullptr;

    if (count != 0)
    {
        getHandleIntVectorProperty(uid, __GO_CHILDREN__, &children);
    }

    int index = 0;
    for (int i = 0; i < count; ++i)
    {
        int child = children[i];
        int hidden = 0;
        getHandleBoolProperty(child, __GO_HIDDEN__, &hidden);
        if (hidden == 0)
        {
            if (export_handle(node, std::to_string(index), child, xfer_plist_id) == false)
            {
                releaseGraphicObjectProperty(__GO_CHILDREN__, children, jni_int_vector, count);
                closeList6(node);
                return false;
            }

            ++index;
        }
    }

    releaseGraphicObjectProperty(__GO_CHILDREN__, children, jni_int_vector, count);
    closeList6(node);
    return true;
}

int add_current_entity(hid_t dataset, int version)
{
    int type = 0;
    getHandleInt(dataset, "type", &type);

    switch (type)
    {
        case __GO_FIGURE__:
        {
            return import_handle(dataset, -1, version);
        }
        case __GO_AXES__:
        {
            //add handle to current figure
            getOrCreateDefaultSubwin();
            int iCurrentFigure = getCurrentFigure();
            return import_handle(dataset, iCurrentFigure, version);
        }
        default:
        {
            // add handle as child of current axes ( take care of compound ! )
            int axes = getOrCreateDefaultSubwin();
            return import_handle(dataset, axes, version);
        }
    }
}
