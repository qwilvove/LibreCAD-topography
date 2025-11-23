#include "io.h"

#include <QFile>
#include <QSettings>

namespace IO
{

bool readPluginSettings(PluginSettings *pluginSettings)
{
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, "LibreCAD", "topographytools");
    *(pluginSettings->fileName) = settings.value("lastfilename", "").toString();

    return true;
}

bool writePluginSettings(PluginSettings *pluginSettings)
{
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, "LibreCAD", "topographytools");
    settings.setValue("lastfilename", *(pluginSettings->fileName));

    return true;
}

/* READ .TT FILE */
int readTtFile(TtFileData *ttFileData)
{
    if (ttFileData->fileName->isEmpty())
    {
        return -1;
    }

    QFile file(*(ttFileData->fileName));
    if (!file.exists())
    {
        return -2;
    }
    if (!file.open(QIODevice::ReadOnly))
    {
        return -3;
    }

    QDataStream stream(&file);

    readTtFileSettings(stream, ttFileData->settings);
    readTtFilePoints(stream, ttFileData->points);

    file.close();

    return 0;
}

void readTtFileSettings(QDataStream &stream, TT::Settings *settings)
{

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
int writeTtFile(TtFileData *ttFileData)
{
    QFile file(*(ttFileData->fileName));
    if (!file.open(QIODevice::WriteOnly))
    {
        return -1;
    }

    QDataStream stream(&file);
    stream.setVersion(QDataStream::Qt_5_0);

    writeTtFileSettings(stream, ttFileData->settings);
    writeTtFilePoints(stream, ttFileData->points);

    file.close();

    return 0;
}

void writeTtFileSettings(QDataStream &stream, TT::Settings *settings)
{

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

} // namespace IO
