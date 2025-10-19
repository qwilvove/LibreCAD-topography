#ifndef TT_CLASSPOINT_H
#define TT_CLASSPOINT_H

#include <QString>
#include <QTableWidgetItem>

enum PTYPE
{
    POINT,
    STATION,
    REFERENCE,
    MEASURE
};

class Point
{
public:
    Point();
    ~Point();

    PTYPE type;
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
    struct PTypeProperties
    {
        QString name;
        QColor colour;
    };

    static const QMap<PTYPE, PTypeProperties> PTYPES;
};

#endif // TT_CLASSPOINT_H
