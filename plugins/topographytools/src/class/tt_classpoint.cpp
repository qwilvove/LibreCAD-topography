#include "tt_classpoint.h"

Point::Point()
{
}

Point::~Point()
{
}

const QMap<PTYPE, Point::PTypeProperties> Point::PTYPES = {
    { POINT,     { QObject::tr("POINT"),     QColor(QString("dark green")) } },
    { STATION,   { QObject::tr("STATION"),   QColor(QString("dark red"))   } },
    { REFERENCE, { QObject::tr("REFERENCE"), QColor(QString("dark blue"))  } },
    { MEASURE,   { QObject::tr("MEASURE"),   QColor(QString("dark cyan"))  } }
};

QTableWidgetItem* Point::getQTableWidgetItemLineNumber(int lineNumber)
{
    QTableWidgetItem *twi = new QTableWidgetItem(QString("%1").arg(lineNumber));

    twi->setForeground(Point::PTYPES[this->type].colour);

    return twi;
}

QTableWidgetItem* Point::getQTableWidgetItemType()
{
    QTableWidgetItem *twi = new QTableWidgetItem(Point::PTYPES[this->type].name);

    twi->setForeground(Point::PTYPES[this->type].colour);

    return twi;
}

QTableWidgetItem* Point::getQTableWidgetItemName()
{
    QTableWidgetItem *twi = new QTableWidgetItem(this->name);

    twi->setForeground(Point::PTYPES[this->type].colour);

    return twi;
}

QTableWidgetItem* Point::getQTableWidgetItemParameters()
{
    QTableWidgetItem *twi = new QTableWidgetItem();

    twi->setForeground(Point::PTYPES[this->type].colour);

    QString parameters = QString("???");

    switch(this->type)
    {
    case PTYPE::POINT:
        parameters = QString("X = %1 , Y = %2").arg(this->x, 11, 'f', 3).arg(this->y, 11, 'f', 3);
        if (this->hasZ)
        {
            parameters += QString(" , Z = %1").arg(this->z, 8, 'f', 3);
        }
        break;
    case PTYPE::STATION:
        parameters = QString("IH = %1").arg(this->ih, 7, 'f', 3);
        if (this->v0 >= 0)
        {
            parameters += QString(" , V0 = %1").arg(this->v0, 9, 'f', 5);
        }
        break;
    case PTYPE::REFERENCE:
    case PTYPE::MEASURE:
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
