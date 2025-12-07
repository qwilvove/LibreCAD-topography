#ifndef TT_CLASS_CODE_H
#define TT_CLASS_CODE_H

#include <QString>
#include <QTableWidgetItem>

#include "src/class/tt_class_layer.h"

class Code
{
public:
    enum class TYPE
    {
        NONE,

        // Lines
        LINE_SEGMENT_INIT,
        LINE_SEGMENT_CONTINUE,
        LINE_ARC_INIT,
        LINE_ARC_MIDDLE,
        LINE_ARC_CONTINUE,
        LINE_OFFSET,
        LINE_ORIENTATION,
        LINE_CLOSE,
        LINE_CLOSE_WITH_OFFSET,
        LINE_END,

        // Blocks
        BLOCK_P1,
        BLOCK_P2,
        BLOCK_P3,
        BLOCK_P2V
    };

    static const QMap<TYPE, QString> TYPES;

    Code(QString code, TYPE type, QString blockName, Layer *layer);

    static bool isLineType(TYPE t);
    static bool isBlockType(TYPE t);

    QString getCode();
    TYPE getType();
    QString getBlockName();
    Layer *getLayer();
    QTableWidgetItem *getTwiCode();
    QTableWidgetItem *getTwiType();
    QTableWidgetItem *getTwiBlockName();
    QTableWidgetItem *getTwiLayer();

    void setCode(QString code);
    void setType(TYPE type);
    void setBlockName(QString blockName);
    void setLayer(Layer *layer);

private:
    QString code;
    TYPE type;
    QString blockName;
    Layer *layer;
};

#endif // TT_CLASS_CODE_H
