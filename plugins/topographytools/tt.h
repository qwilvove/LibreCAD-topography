#ifndef TT_H
#define TT_H

#include <QString>

namespace TT
{

enum PTYPE
{
    POINT,
    STATION,
    REFERENCE,
    MEASURE
};

struct Point
{
    PTYPE type;
    QString name;
    double x;
    double y;
    bool hasZ;
    double z;
    double ih; // Instrument height - for a STATION
    double v0; // Start angle (grades) - for a STATION
    double ph; // Prism height
    double ha; // Horizontal angle
    double va; // Vertical angle
    double id; // Inclined distance
};

}

#endif // TT_H
