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

    Point();

    static QString getTypeName(TYPE type);
    static QColor getTypeColour(TYPE type);

    TYPE getType();
    QString getName();
    QString getCode();
    double getX();
    double getY();
    bool getHasZ();
    double getZ();
    double getIh();
    double getV0();
    double getPh();
    double getHa();
    double getVa();
    double getId();
    QTableWidgetItem* getTwiType();
    QTableWidgetItem* getTwiName();
    QTableWidgetItem* getTwiCode();
    QTableWidgetItem* getTwiParameters();

    void setType(TYPE type);
    void setName(QString name);
    void setCode(QString code);
    void setX(double x);
    void setY(double y);
    void setHasZ(bool hasZ);
    void setZ(double z);
    void setIh(double ih);
    void setV0(double v0);
    void setPh(double ph);
    void setHa(double ha);
    void setVa(double va);
    void setId(double id);

private:
    struct TypeProperties
    {
        QString name;
        QColor colour;
    };

    static const QMap<TYPE, TypeProperties> TYPES;

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
};

#endif // TT_CLASS_POINT_H
