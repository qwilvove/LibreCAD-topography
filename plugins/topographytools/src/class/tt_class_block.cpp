#include "tt_class_block.h"

const QMap<Block::TAB, QString> Block::TABS = {
    { Block::TAB::VEGETATION,  QObject::tr("Vegetation")  },
    { Block::TAB::ELECTRICITY, QObject::tr("Electricity") },
    { Block::TAB::ROAD,        QObject::tr("Road")        }
};

const QList<Block> Block::BLOCKS = {
    Block("tree",                 Block::INSERTION_TYPE::P1,    Block::TAB::VEGETATION  ),
    Block("electrical_box",       Block::INSERTION_TYPE::P1,    Block::TAB::ELECTRICITY ),
    Block("electrical_box_multi", Block::INSERTION_TYPE::MULTI, Block::TAB::ELECTRICITY ),
    Block("tt_sign_turn_left",    Block::INSERTION_TYPE::P1,    Block::TAB::ROAD        ),
    Block("tt_sign_turn_right",   Block::INSERTION_TYPE::P1,    Block::TAB::ROAD        )
};

Block::Block(QString file, INSERTION_TYPE type, TAB tab)
{
    this->file = file;
    this->type = type;
    this->tab  = tab;
}

QString Block::getTabName(Block::TAB tab)
{
    return Block::TABS[tab];
}

QList<Block> Block::getBlocks()
{
    return Block::BLOCKS;
}

QString Block::getFile()
{
    return this->file;
}

Block::INSERTION_TYPE Block::getType()
{
    return this->type;
}

Block::TAB Block::getTab()
{
    return this->tab;
}
