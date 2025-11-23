#ifndef TT_STATIC_IO_H
#define TT_STATIC_IO_H

#include "src/tt.h"

namespace IO
{
    struct PluginSettings
    {
        QString *fileName;
    };

    struct TtFileData
    {
        QString const *fileName;
        TT::Settings *settings;
        QList<TT::Point*> *points;
    };

    bool readPluginSettings(PluginSettings *pluginSettings);
    bool writePluginSettings(PluginSettings *pluginSettings);

    int readTtFile(TtFileData *ttFileData);
    void readTtFileSettings(QDataStream &stream, TT::Settings *settings);
    void readTtFilePoints(QDataStream &stream, QList<TT::Point*> *points);
    void readTtFilePoint(QDataStream &stream, TT::Point *point);

    int writeTtFile(TtFileData *ttFileData);
    void writeTtFileSettings(QDataStream &stream, TT::Settings *settings);
    void writeTtFilePoints(QDataStream &stream, QList<TT::Point*> *points);
    void writeTtFilePoint(QDataStream &stream, TT::Point *point);
}

#endif // TT_STATIC_IO_H
