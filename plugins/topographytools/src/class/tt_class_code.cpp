#include "tt_class_code.h"

const QMap<Code::TYPE, QString> Code::TYPES = {
    { Code::TYPE::NONE                   , QObject::tr("None"                                          ) },

    { Code::TYPE::LINE_SEGMENT_INIT      , QObject::tr("Line : Segment init"                           ) },
    { Code::TYPE::LINE_SEGMENT_CONTINUE  , QObject::tr("Line : Segment continue"                       ) },
    { Code::TYPE::LINE_ARC_INIT          , QObject::tr("Line : Arc init"                               ) },
    { Code::TYPE::LINE_ARC_MIDDLE        , QObject::tr("Line : Arc middle"                             ) },
    { Code::TYPE::LINE_ARC_CONTINUE      , QObject::tr("Line : Arc continue"                           ) },
    { Code::TYPE::LINE_OFFSET            , QObject::tr("Line : Offset"                                 ) },
    { Code::TYPE::LINE_ORIENTATION       , QObject::tr("Line : Orientation"                            ) },
    { Code::TYPE::LINE_CLOSE             , QObject::tr("Line : Close"                                  ) },
    { Code::TYPE::LINE_CLOSE_WITH_OFFSET , QObject::tr("Line : Close with offset"                      ) },
    { Code::TYPE::LINE_END               , QObject::tr("Line : End without closing"                    ) },

    { Code::TYPE::BLOCK_P1               , QObject::tr("Block inserted with 1 reference point"         ) },
    { Code::TYPE::BLOCK_P2               , QObject::tr("Block inserted with 2 reference points"        ) },
    { Code::TYPE::BLOCK_P3               , QObject::tr("Block inserted with 3 reference points"        ) },
    { Code::TYPE::BLOCK_P2V              , QObject::tr("Block inserted with 2 reference points + value") }
};

Code::Code(QString code, TYPE type, QString blockName, Layer *layer)
{
    this->code = code;
    this->type = type;
    this->blockName = blockName;
    this->layer = layer;
}

bool Code::isLineType(TYPE t)
{
    bool isLineType = false;

    if (t == TYPE::LINE_SEGMENT_INIT      ||
        t == TYPE::LINE_SEGMENT_CONTINUE  ||
        t == TYPE::LINE_ARC_INIT          ||
        t == TYPE::LINE_ARC_MIDDLE        ||
        t == TYPE::LINE_ARC_CONTINUE      ||
        t == TYPE::LINE_OFFSET            ||
        t == TYPE::LINE_ORIENTATION       ||
        t == TYPE::LINE_CLOSE             ||
        t == TYPE::LINE_CLOSE_WITH_OFFSET ||
        t == TYPE::LINE_END               )
    {
        isLineType = true;
    }

    return isLineType;
}

bool Code::isBlockType(TYPE t)
{
    bool isBlockType = false;

    if (t == TYPE::BLOCK_P1  ||
        t == TYPE::BLOCK_P2  ||
        t == TYPE::BLOCK_P3  ||
        t == TYPE::BLOCK_P2V )
    {
        isBlockType = true;
    }

    return isBlockType;
}

QString Code::getCode()
{
    return code;
}

Code::TYPE Code::getType()
{
    return type;
}

QString Code::getBlockName()
{
    return blockName;
}

Layer *Code::getLayer()
{
    return layer;
}

QTableWidgetItem *Code::getTwiCode()
{
    QTableWidgetItem *twi = new QTableWidgetItem(code);

    return twi;
}

QTableWidgetItem *Code::getTwiType()
{
    QTableWidgetItem *twi = new QTableWidgetItem(TYPES[type]);

    return twi;
}

QTableWidgetItem *Code::getTwiBlockName()
{
    QTableWidgetItem *twi = new QTableWidgetItem(blockName);

    return twi;
}

QTableWidgetItem *Code::getTwiLayer()
{
    QTableWidgetItem *twi = new QTableWidgetItem(layer->getName());

    return twi;
}

void Code::setCode(QString code)
{
    this->code = code;
}

void Code::setType(TYPE type)
{
    this->type = type;
}

void Code::setBlockName(QString blockName)
{
    this->blockName = blockName;
}

void Code::setLayer(Layer *layer)
{
    this->layer = layer;
}
