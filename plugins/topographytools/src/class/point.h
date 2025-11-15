#ifndef TT_CLASS_POINT_H
#define TT_CLASS_POINT_H

#include <QString>
#include <QTableWidgetItem>

class Point
{
public:
    Point();
    ~Point();

    enum TYPE
    {
        POINT,
        STATION,
        REFERENCE,
        MEASURE
    };

    TYPE type;
    QString name;
    double x;  //                      - for POINT
    double y;  //                      - for POINT
    bool hasZ; //                      - for POINT
    double z;  //                      - for POINT
    double ih; // Instrument height    - for STATION
    double v0; // Start angle (grades) - for STATION
    double ph; // Prism height         - for REFERENCE & MEASURE
    double ha; // Horizontal angle     - for REFERENCE & MEASURE
    double va; // Vertical angle       - for REFERENCE & MEASURE
    double id; // Inclined distance    - for REFERENCE & MEASURE

    QTableWidgetItem* getQTableWidgetItemLineNumber(int lineNumber);
    QTableWidgetItem* getQTableWidgetItemType();
    QTableWidgetItem* getQTableWidgetItemName();
    QTableWidgetItem* getQTableWidgetItemParameters();

private:
    struct TypeProperties
    {
        QString name;
        QColor colour;
    };

    static const QMap<TYPE, TypeProperties> TYPES;
};

#endif // TT_CLASS_POINT_H
