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
    stream >> point->type;
    stream >> point->name;
    stream >> point->code;
    stream >> point->x;
    stream >> point->y;
    stream >> point->hasZ;
    stream >> point->z;
    stream >> point->ih;
    stream >> point->v0;
    stream >> point->ph;
    stream >> point->ha;
    stream >> point->va;
    stream >> point->id;
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
    stream << point->type;
    stream << point->name;
    stream << point->code;
    stream << point->x;
    stream << point->y;
    stream << point->hasZ;
    stream << point->z;
    stream << point->ih;
    stream << point->v0;
    stream << point->ph;
    stream << point->ha;
    stream << point->va;
    stream << point->id;
}

} // namespace io
