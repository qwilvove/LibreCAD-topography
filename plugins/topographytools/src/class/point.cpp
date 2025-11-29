#include "point.h"

const QMap<Point::TYPE, Point::TypeProperties> Point::TYPES = {
    { Point::TYPE::POINT,     { QObject::tr("POINT"),     QColor(QString("dark green")) } },
    { Point::TYPE::STATION,   { QObject::tr("STATION"),   QColor(QString("dark red"))   } },
    { Point::TYPE::REFERENCE, { QObject::tr("REFERENCE"), QColor(QString("dark blue"))  } },
    { Point::TYPE::MEASURE,   { QObject::tr("MEASURE"),   QColor(QString("dark cyan"))  } }
};

Point::Point()
{
}

Point::~Point()
{
}

QTableWidgetItem* Point::getQTableWidgetItemType()
{
    QTableWidgetItem *twi = new QTableWidgetItem(Point::TYPES[this->type].name);

    twi->setForeground(Point::TYPES[this->type].colour);

    return twi;
}

QTableWidgetItem* Point::getQTableWidgetItemName()
{
    QTableWidgetItem *twi = new QTableWidgetItem(this->name);

    twi->setForeground(Point::TYPES[this->type].colour);

    return twi;
}

QTableWidgetItem* Point::getQTableWidgetItemCode()
{
    QTableWidgetItem *twi = new QTableWidgetItem(this->code);

    twi->setForeground(QColor(QString("#606060")));

    return twi;
}

QTableWidgetItem* Point::getQTableWidgetItemParameters()
{
    QTableWidgetItem *twi = new QTableWidgetItem();

    twi->setForeground(Point::TYPES[this->type].colour);

    QString parameters = QString("???");

    switch(this->type)
    {
    case TYPE::POINT:
        parameters = QString("X = %1 , Y = %2").arg(this->x, 11, 'f', 3).arg(this->y, 11, 'f', 3);
        if (this->hasZ)
        {
            parameters += QString(" , Z = %1").arg(this->z, 8, 'f', 3);
        }
        break;
    case TYPE::STATION:
        parameters = QString("IH = %1").arg(this->ih, 7, 'f', 3);
        if (this->v0 >= 0)
        {
            parameters += QString(" , V0 = %1").arg(this->v0, 9, 'f', 5);
        }
        break;
    case TYPE::REFERENCE:
    case TYPE::MEASURE:
        parameters = QString("PH = %1 , HA = %2 , VA = %3 , ID = %4")
            .arg(this->ph, 7, 'f', 3)
            .arg(this->ha, 9, 'f', 5)
            .arg(this->va, 9, 'f', 5)
            .arg(this->id, 7, 'f', 3);
    default:
        break;
    }

    twi->setText(parameters);

    return twi;
}
