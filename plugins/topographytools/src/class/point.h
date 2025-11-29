#ifndef TT_CLASS_POINT_H
#define TT_CLASS_POINT_H

#include <QString>
#include <QTableWidgetItem>

class Point
{
public:
    enum TYPE
    {
        POINT,
        STATION,
        REFERENCE,
        MEASURE
    };

    struct TypeProperties
    {
        QString name;
        QColor colour;
    };

    static const QMap<TYPE, TypeProperties> TYPES;

    Point();
    ~Point();

    TYPE type;
    QString name;
    QString code;
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

    QTableWidgetItem* getQTableWidgetItemType();
    QTableWidgetItem* getQTableWidgetItemName();
    QTableWidgetItem* getQTableWidgetItemCode();
    QTableWidgetItem* getQTableWidgetItemParameters();
};

#endif // TT_CLASS_POINT_H
