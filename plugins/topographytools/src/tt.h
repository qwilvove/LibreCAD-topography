#ifndef TT_H
#define TT_H

#include "class/point.h"
#include "class/settings.h"

#include "document_interface.h"

#include <QString>

namespace TT
{
// Add everything from classes to namespace TT
using ::Point;
using ::Settings;

enum class LAYER
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
    { LAYER::POINTS,      { QObject::tr("TT_POINTS"),      0x000000, DPI::LineWidth::WidthDefault, DPI::LineType::SolidLine } },
    { LAYER::NAME,        { QObject::tr("TT_NAME"),        0x00FFFF, DPI::LineWidth::WidthDefault, DPI::LineType::SolidLine } },
    { LAYER::ALTI,        { QObject::tr("TT_ALTI"),        0x00FF00, DPI::LineWidth::WidthDefault, DPI::LineType::SolidLine } },

    { LAYER::VEGETATION,  { QObject::tr("TT_VEGETATION"),  0x00FF00, DPI::LineWidth::WidthDefault, DPI::LineType::SolidLine } },
    { LAYER::ELECTRICITY, { QObject::tr("TT_ELECTRICITY"), 0xFF0000, DPI::LineWidth::WidthDefault, DPI::LineType::SolidLine } },
    { LAYER::ROAD,        { QObject::tr("TT_ROAD"),        0x000000, DPI::LineWidth::WidthDefault, DPI::LineType::SolidLine } }
};

enum class TAB
{
    VEGETATION,
    ELECTRICITY,
    ROAD
};

inline const QMap<TAB, QString> TABS = {
    { TAB::VEGETATION,  QObject::tr("Vegetation")  },
    { TAB::ELECTRICITY, QObject::tr("Electricity") },
    { TAB::ROAD,        QObject::tr("Road")        }
};

enum class BLOCK_INSERTION_TYPE
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
    { TREE,                 { "tree",                 TAB::VEGETATION,  LAYER::VEGETATION,  BLOCK_INSERTION_TYPE::P1    } },
    { ELECTRICAL_BOX,       { "electrical_box",       TAB::ELECTRICITY, LAYER::ELECTRICITY, BLOCK_INSERTION_TYPE::P1    } },
    { ELECTRICAL_BOX_MULTI, { "electrical_box_multi", TAB::ELECTRICITY, LAYER::ELECTRICITY, BLOCK_INSERTION_TYPE::MULTI } },
    { SIGN_TURN_LEFT,       { "tt_sign_turn_left",    TAB::ROAD,        LAYER::ROAD,        BLOCK_INSERTION_TYPE::P1    } },
    { SIGN_TURN_RIGHT,      { "tt_sign_turn_right",   TAB::ROAD,        LAYER::ROAD,        BLOCK_INSERTION_TYPE::P1    } }
};

}

#endif // TT_H
