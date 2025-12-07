#include "io.h"

#include <QFile>

namespace io
{

/* READ .TT FILE */
int readTtFile(const QString &fileName, TT::ProjectSettings *projectSettings, QList<TT::Point *> *points)
{
    if (fileName.isEmpty())
    {
        return -1;
    }

    QFile file(fileName);
    if (!file.exists())
    {
        return -2;
    }
    if (!file.open(QIODevice::ReadOnly))
    {
        return -3;
    }

    QDataStream stream(&file);

    bool correctFileVersion = readTtFileVersion(stream);
    if (!correctFileVersion)
    {
        return -4;
    }

    readTtFileSettings(stream, projectSettings);
    readTtFilePoints(stream, points);

    file.close();

    return 0;
}

bool readTtFileVersion(QDataStream &stream)
{
    int fileVersion;

    stream >> fileVersion;

    return fileVersion == TT_CURRENT_FILE_VERSION;
}

void readTtFileSettings(QDataStream &stream, TT::ProjectSettings *settings)
{
    double scale;

    stream >> scale;

    settings->setScale(scale);
}

void readTtFilePoints(QDataStream &stream, QList<TT::Point*> *points)
{
    points->clear();

    int max;
    stream >> max;

    for (int i = 0; i < max; i++)
    {
        TT::Point *point = new TT::Point();
        readTtFilePoint(stream, point);
        points->append(point);
    }
}

void readTtFilePoint(QDataStream &stream, TT::Point *point)
{
    int     i = 0;
    QString s = "";
    double  d = 0;
    bool    b = false;

    stream >> i; point->setType((TT::Point::TYPE)i);
    stream >> s; point->setName(s);
    stream >> s; point->setCode(s);
    stream >> d; point->setX(d);
    stream >> d; point->setY(d);
    stream >> b; point->setHasZ(b);
    stream >> d; point->setZ(d);
    stream >> d; point->setIh(d);
    stream >> d; point->setV0(d);
    stream >> d; point->setPh(d);
    stream >> d; point->setHa(d);
    stream >> d; point->setVa(d);
    stream >> d; point->setId(d);
}

/* WRITE .TT FILE */
int writeTtFile(const QString &fileName, TT::ProjectSettings *projectSettings, QList<TT::Point *> *points)
{
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly))
    {
        return -1;
    }

    QDataStream stream(&file);
    stream.setVersion(QDataStream::Qt_5_0);

    writeTtFileVersion(stream);
    writeTtFileSettings(stream, projectSettings);
    writeTtFilePoints(stream, points);

    file.close();

    return 0;
}

void writeTtFileVersion(QDataStream &stream)
{
    stream << TT_CURRENT_FILE_VERSION;
}

void writeTtFileSettings(QDataStream &stream, TT::ProjectSettings *settings)
{
    stream << settings->getScale();
}

void writeTtFilePoints(QDataStream &stream, QList<TT::Point*> *points)
{
    int len = points->size();
    stream << len;

    foreach (TT::Point *point, *points)
    {
        writeTtFilePoint(stream, point);
    }
}

void writeTtFilePoint(QDataStream &stream, TT::Point *point)
{
    stream << (int)point->getType();
    stream << point->getName();
    stream << point->getCode();
    stream << point->getX();
    stream << point->getY();
    stream << point->getHasZ();
    stream << point->getZ();
    stream << point->getIh();
    stream << point->getV0();
    stream << point->getPh();
    stream << point->getHa();
    stream << point->getVa();
    stream << point->getId();
}

} // namespace io
