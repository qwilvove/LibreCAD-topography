#ifndef TT_H
#define TT_H

#include "tt_classpoint.h"

#include "document_interface.h"

#include <QString>

namespace TT
{
// Add everything from tt_classpoint to namespace TT
using ::PTYPE;
using ::Point;

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

enum TAB
{
    TAB_VEGETATION,
    TAB_ELECTRICITY,
    TAB_ROAD
};

inline const QMap<TAB, QString> TABS = {
    { TAB_VEGETATION,  QObject::tr("Vegetation")  },
    { TAB_ELECTRICITY, QObject::tr("Electricity") },
    { TAB_ROAD,        QObject::tr("Road")        }
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
    TAB tab;
    LAYER layer;
    BLOCK_INSERTION_TYPE type;
};

inline const QMap<BLOCK, BlockProperties> BLOCKS = {
    { TREE,                 { "tree",                 TAB::TAB_VEGETATION,  LAYER::VEGETATION,  BLOCK_INSERTION_TYPE::P1    } },
    { ELECTRICAL_BOX,       { "electrical_box",       TAB::TAB_ELECTRICITY, LAYER::ELECTRICITY, BLOCK_INSERTION_TYPE::P1    } },
    { ELECTRICAL_BOX_MULTI, { "electrical_box_multi", TAB::TAB_ELECTRICITY, LAYER::ELECTRICITY, BLOCK_INSERTION_TYPE::MULTI } },
    { SIGN_TURN_LEFT,       { "tt_sign_turn_left",    TAB::TAB_ROAD,        LAYER::ROAD,        BLOCK_INSERTION_TYPE::P1    } },
    { SIGN_TURN_RIGHT,      { "tt_sign_turn_right",   TAB::TAB_ROAD,        LAYER::ROAD,        BLOCK_INSERTION_TYPE::P1    } }
};

}

#endif // TT_H
