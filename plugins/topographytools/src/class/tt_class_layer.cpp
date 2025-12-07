#include "tt_class_layer.h"

const QMap<DPI::LineWidth, QString> Layer::LINE_WIDTHS = {
    { DPI::LineWidth::Width00      , QObject::tr("0.00mm"  ) },
    { DPI::LineWidth::Width01      , QObject::tr("0.05mm"  ) },
    { DPI::LineWidth::Width02      , QObject::tr("0.09mm"  ) },
    { DPI::LineWidth::Width03      , QObject::tr("0.13mm"  ) },
    { DPI::LineWidth::Width04      , QObject::tr("0.15mm"  ) },
    { DPI::LineWidth::Width05      , QObject::tr("0.18mm"  ) },
    { DPI::LineWidth::Width06      , QObject::tr("0.20mm"  ) },
    { DPI::LineWidth::Width07      , QObject::tr("0.25mm"  ) },
    { DPI::LineWidth::Width08      , QObject::tr("0.30mm"  ) },
    { DPI::LineWidth::Width09      , QObject::tr("0.35mm"  ) },
    { DPI::LineWidth::Width10      , QObject::tr("0.40mm"  ) },
    { DPI::LineWidth::Width11      , QObject::tr("0.50mm"  ) },
    { DPI::LineWidth::Width12      , QObject::tr("0.53mm"  ) },
    { DPI::LineWidth::Width13      , QObject::tr("0.60mm"  ) },
    { DPI::LineWidth::Width14      , QObject::tr("0.70mm"  ) },
    { DPI::LineWidth::Width15      , QObject::tr("0.80mm"  ) },
    { DPI::LineWidth::Width16      , QObject::tr("0.90mm"  ) },
    { DPI::LineWidth::Width17      , QObject::tr("1.00mm"  ) },
    { DPI::LineWidth::Width18      , QObject::tr("1.06mm"  ) },
    { DPI::LineWidth::Width19      , QObject::tr("1.20mm"  ) },
    { DPI::LineWidth::Width20      , QObject::tr("1.40mm"  ) },
    { DPI::LineWidth::Width21      , QObject::tr("1.58mm"  ) },
    { DPI::LineWidth::Width22      , QObject::tr("2.00mm"  ) },
    { DPI::LineWidth::Width23      , QObject::tr("2.11mm"  ) },
    { DPI::LineWidth::WidthDefault , QObject::tr("Default" ) }
};

const QMap<DPI::LineType, QString> Layer::LINE_TYPES = {
    { DPI::LineType::NoPen         , QObject::tr("No line"                    ) },
    { DPI::LineType::SolidLine     , QObject::tr("Solid"                      ) },
    { DPI::LineType::DotLine       , QObject::tr("Dot line"                   ) },
    { DPI::LineType::DotLine2      , QObject::tr("Dot line small"             ) },
    { DPI::LineType::DotLineX2     , QObject::tr("Dot line large"             ) },
    { DPI::LineType::DashLine      , QObject::tr("Dashed line"                ) },
    { DPI::LineType::DashLine2     , QObject::tr("Dashed line small"          ) },
    { DPI::LineType::DashLineX2    , QObject::tr("Dashed line large"          ) },
    { DPI::LineType::DashDotLine   , QObject::tr("Dash & dot line"            ) },
    { DPI::LineType::DashDotLine2  , QObject::tr("Dash & dot line small"      ) },
    { DPI::LineType::DashDotLineX2 , QObject::tr("Dash & dot line large"      ) },
    { DPI::LineType::DivideLine    , QObject::tr("Dash, dot, dot line"        ) },
    { DPI::LineType::DivideLine2   , QObject::tr("Dash, dot, dot line small"  ) },
    { DPI::LineType::DivideLineX2  , QObject::tr("Dash, dot, dot line large"  ) },
    { DPI::LineType::CenterLine    , QObject::tr("Dash, small dash line"      ) },
    { DPI::LineType::CenterLine2   , QObject::tr("Dash, small dash line small") },
    { DPI::LineType::CenterLineX2  , QObject::tr("Dash, small dash line large") },
    { DPI::LineType::BorderLine    , QObject::tr("Dash, dash, dot line"       ) },
    { DPI::LineType::BorderLine2   , QObject::tr("Dash, dash, dot line small" ) },
    { DPI::LineType::BorderLineX2  , QObject::tr("Dash, dash, dot line large" ) }
};

Layer::Layer(QString name, QColor colour, DPI::LineWidth lineWidth, DPI::LineType lineType)
{
    this->name = name;
    this->colour = colour;
    this->lineWidth = lineWidth;
    this->lineType = lineType;
}

QString Layer::getName()
{
    return name;
}

QColor Layer::getColour()
{
    return colour;
}

DPI::LineWidth Layer::getLineWidth()
{
    return lineWidth;
}

DPI::LineType Layer::getLineType()
{
    return lineType;
}

QTableWidgetItem* Layer::getQTableWidgetItemName()
{
    QTableWidgetItem *twi = new QTableWidgetItem(name);

    return twi;
}

QTableWidgetItem* Layer::getQTableWidgetItemColour()
{
    QTableWidgetItem *twi = new QTableWidgetItem(colour.name());

    twi->setForeground(colour);
    twi->setBackground(colour);

    return twi;
}

QTableWidgetItem* Layer::getQTableWidgetItemLineWidth()
{
    QTableWidgetItem *twi = new QTableWidgetItem(LINE_WIDTHS[lineWidth]);

    return twi;
}

QTableWidgetItem* Layer::getQTableWidgetItemLineType()
{
    QTableWidgetItem *twi = new QTableWidgetItem(LINE_TYPES[lineType]);

    return twi;
}

void Layer::setName(QString name)
{
    this->name = name;
}

void Layer::setColour(QColor colour)
{
    this->colour = colour;
}

void Layer::setLineWidth(DPI::LineWidth lineWidth)
{
    this->lineWidth = lineWidth;
}

void Layer::setLineType(DPI::LineType lineType)
{
    this->lineType = lineType;
}
