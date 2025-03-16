#ifndef TT_H
#define TT_H

#include "document_interface.h"

#include <QString>

namespace TT
{

enum PTYPE
{
    POINT,
    STATION,
    REFERENCE,
    MEASURE
};

struct Point
{
    PTYPE type;
    QString name;
    double x;
    double y;
    bool hasZ;
    double z;
    double ih; // Instrument height - for a STATION
    double v0; // Start angle (grades) - for a STATION
    double ph; // Prism height
    double ha; // Horizontal angle
    double va; // Vertical angle
    double id; // Inclined distance
};

enum LAYER
{
    POINTS,
    NAME,
    ALTI,

    VEGETATION,
    ELECTRICITY,
    ROAD
};

struct LayerProperties
{
    QString name;
    int colour;
    DPI::LineWidth lineWidth;
    DPI::LineType lineType;
};

inline const QMap<LAYER, LayerProperties> LAYERS = {
    { POINTS,      { QObject::tr("TT_POINTS"),      0x000000, DPI::LineWidth::WidthDefault, DPI::LineType::SolidLine } },
    { NAME,        { QObject::tr("TT_NAME"),        0x00FFFF, DPI::LineWidth::WidthDefault, DPI::LineType::SolidLine } },
    { ALTI,        { QObject::tr("TT_ALTI"),        0x00FF00, DPI::LineWidth::WidthDefault, DPI::LineType::SolidLine } },

    { VEGETATION,  { QObject::tr("TT_VEGETATION"),  0x00FF00, DPI::LineWidth::WidthDefault, DPI::LineType::SolidLine } },
    { ELECTRICITY, { QObject::tr("TT_ELECTRICITY"), 0xFF0000, DPI::LineWidth::WidthDefault, DPI::LineType::SolidLine } },
    { ROAD,        { QObject::tr("TT_ROAD"),        0x000000, DPI::LineWidth::WidthDefault, DPI::LineType::SolidLine } }
};

enum BLOCK_INSERTION_TYPE
{
    ALL,
    MULTI,

    P1,
    P2,
    P3,
    P2V
};

enum BLOCK
{
    TREE,

    ELECTRICAL_BOX,
    ELECTRICAL_BOX_MULTI,

    SIGN_TURN_LEFT,
    SIGN_TURN_RIGHT
};

struct BlockProperties
{
    QString file;
    QString tabName;
    LAYER layer;
    BLOCK_INSERTION_TYPE type;
};

inline const QMap<BLOCK, BlockProperties> BLOCKS = {
    { TREE,                 { "tree",                 QObject::tr("Vegetation"),  LAYER::VEGETATION,  BLOCK_INSERTION_TYPE::P1    } },
    { ELECTRICAL_BOX,       { "electrical_box",       QObject::tr("Electricity"), LAYER::ELECTRICITY, BLOCK_INSERTION_TYPE::P1    } },
    { ELECTRICAL_BOX_MULTI, { "electrical_box_multi", QObject::tr("Electricity"), LAYER::ELECTRICITY, BLOCK_INSERTION_TYPE::MULTI } },
    { SIGN_TURN_LEFT,       { "tt_sign_turn_left",    QObject::tr("Road"),        LAYER::ROAD,        BLOCK_INSERTION_TYPE::P1    } },
    { SIGN_TURN_RIGHT,      { "tt_sign_turn_right",   QObject::tr("Road"),        LAYER::ROAD,        BLOCK_INSERTION_TYPE::P1    } }
};

}

#endif // TT_H
