#ifndef TT_CLASS_BLOCK_H
#define TT_CLASS_BLOCK_H

#include <QString>

#include "src/class/tt_class_layer.h"

class Block
{
public:
    enum class TAB
    {
        VEGETATION,
        ELECTRICITY,
        ROAD
    };

    enum class INSERTION_TYPE
    {
        ALL,
        MULTI,

        P1,
        P2,
        P3,
        P2V
    };

    Block(QString file, INSERTION_TYPE type, TAB tab);

    static QString getTabName(TAB tab);
    static QList<Block> getBlocks();

    QString getFile();
    INSERTION_TYPE getType();
    TAB getTab();

private:
    static const QMap<Block::TAB, QString> TABS;
    static const QList<Block> BLOCKS;

    QString file;
    INSERTION_TYPE type;
    TAB tab;
};

#endif // TT_CLASS_BLOCK_H
