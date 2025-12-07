#include "point.h"

const QMap<Point::TYPE, Point::TypeProperties> Point::TYPES = {
    { Point::TYPE::POINT,     { QObject::tr("POINT"),     QColor(QString("dark green")) } },
    { Point::TYPE::STATION,   { QObject::tr("STATION"),   QColor(QString("dark red"))   } },
    { Point::TYPE::REFERENCE, { QObject::tr("REFERENCE"), QColor(QString("dark blue"))  } },
    { Point::TYPE::MEASURE,   { QObject::tr("MEASURE"),   QColor(QString("dark cyan"))  } }
};

Point::Point()
{
    this->type = TYPE::POINT;
    this->name = "";
    this->code = "";
    this->x = 0;
    this->y = 0;
    this->hasZ = false;
    this->z = 0;
    this->ih = 0;
    this->v0 = -1;
    this->ph = 0;
    this->ha = 0;
    this->va = 0;
    this->id = 0;
}

QString Point::getTypeName(TYPE type)
{
    return TYPES[type].name;
}

QColor Point::getTypeColour(TYPE type)
{
    return TYPES[type].colour;
}

Point::TYPE Point::getType()
{
    return type;
}

QString Point::getName()
{
    return name;
}

QString Point::getCode()
{
    return code;
}

double Point::getX()
{
    return x;
}

double Point::getY()
{
    return y;
}

bool Point::getHasZ()
{
    return hasZ;
}

double Point::getZ()
{
    return z;
}

double Point::getIh()
{
    return ih;
}

double Point::getV0()
{
    return v0;
}

double Point::getPh()
{
    return ph;
}

double Point::getHa()
{
    return ha;
}

double Point::getVa()
{
    return va;
}

double Point::getId()
{
    return id;
}

QTableWidgetItem* Point::getTwiType()
{
    QTableWidgetItem *twi = new QTableWidgetItem(TYPES[type].name);
    twi->setForeground(TYPES[type].colour);

    return twi;
}

QTableWidgetItem* Point::getTwiName()
{
    QTableWidgetItem *twi = new QTableWidgetItem(name);
    twi->setForeground(TYPES[type].colour);

    return twi;
}

QTableWidgetItem* Point::getTwiCode()
{
    QTableWidgetItem *twi = new QTableWidgetItem(code);
    twi->setForeground(QColor(QString("#606060")));

    return twi;
}

QTableWidgetItem* Point::getTwiParameters()
{
    QString parameters = QString("???");
    switch(type)
    {
    case TYPE::POINT:
        parameters = QString("X = %1 , Y = %2").arg(x, 11, 'f', 3).arg(y, 11, 'f', 3);
        if (hasZ)
        {
            parameters += QString(" , Z = %1").arg(z, 8, 'f', 3);
        }
        break;
    case TYPE::STATION:
        parameters = QString("IH = %1").arg(ih, 7, 'f', 3);
        if (v0 >= 0)
        {
            parameters += QString(" , V0 = %1").arg(v0, 9, 'f', 5);
        }
        break;
    case TYPE::REFERENCE:
    case TYPE::MEASURE:
        parameters = QString("PH = %1 , HA = %2 , VA = %3 , ID = %4").arg(ph, 7, 'f', 3).arg(ha, 9, 'f', 5).arg(va, 9, 'f', 5).arg(id, 7, 'f', 3);
    default:
        break;
    }

    QTableWidgetItem *twi = new QTableWidgetItem(parameters);
    twi->setForeground(TYPES[type].colour);

    return twi;
}

void Point::setType(TYPE type)
{
    this->type = type;
}

void Point::setName(QString name)
{
    this->name = name;
}

void Point::setCode(QString code)
{
    this->code = code;
}

void Point::setX(double x)
{
    this->x = x;
}

void Point::setY(double y)
{
    this->y = y;
}

void Point::setHasZ(bool hasZ)
{
    this->hasZ = hasZ;
}

void Point::setZ(double z)
{
    this->z = z;
}

void Point::setIh(double ih)
{
    this->ih = ih;
}

void Point::setV0(double v0)
{
    this->v0 = v0;
}

void Point::setPh(double ph)
{
    this->ph = ph;
}

void Point::setHa(double ha)
{
    this->ha = ha;
}

void Point::setVa(double va)
{
    this->va = va;
}

void Point::setId(double id)
{
    this->id = id;
}
