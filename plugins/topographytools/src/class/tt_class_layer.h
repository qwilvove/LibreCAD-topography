#ifndef TT_CLASS_LAYER_H
#define TT_CLASS_LAYER_H

#include "document_interface.h"

#include <QString>
#include <QTableWidgetItem>

class Layer
{
public:
    static const QMap<DPI::LineWidth, QString> LINE_WIDTHS;
    static const QMap<DPI::LineType, QString> LINE_TYPES;

    Layer(QString name, QColor colour = QColor("black"), DPI::LineWidth lineWidth = DPI::LineWidth::WidthDefault, DPI::LineType lineType = DPI::LineType::SolidLine);

    QString getName();
    QColor getColour();
    DPI::LineWidth getLineWidth();
    DPI::LineType getLineType();

    QTableWidgetItem* getQTableWidgetItemName();
    QTableWidgetItem* getQTableWidgetItemColour();
    QTableWidgetItem* getQTableWidgetItemLineWidth();
    QTableWidgetItem* getQTableWidgetItemLineType();

    void setName(QString name);
    void setColour(QColor colour);
    void setLineWidth(DPI::LineWidth lineWidth);
    void setLineType(DPI::LineType lineType);

private:
    QString name;
    QColor colour;
    DPI::LineWidth lineWidth;
    DPI::LineType lineType;
};

#endif // TT_CLASS_LAYER_H
