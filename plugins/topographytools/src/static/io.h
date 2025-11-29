#ifndef TT_STATIC_IO_H
#define TT_STATIC_IO_H

#include "src/tt.h"

namespace io
{
    static const int TT_CURRENT_FILE_VERSION = 1;

    int readTtFile(QString const &fileName, TT::ProjectSettings *projectSettings, QList<TT::Point*> *points);
    bool readTtFileVersion(QDataStream &stream);
    void readTtFileSettings(QDataStream &stream, TT::ProjectSettings *settings);
    void readTtFilePoints(QDataStream &stream, QList<TT::Point*> *points);
    void readTtFilePoint(QDataStream &stream, TT::Point *point);

    int writeTtFile(QString const &fileName, TT::ProjectSettings *projectSettings, QList<TT::Point*> *points);
    void writeTtFileVersion(QDataStream &stream);
    void writeTtFileSettings(QDataStream &stream, TT::ProjectSettings *settings);
    void writeTtFilePoints(QDataStream &stream, QList<TT::Point*> *points);
    void writeTtFilePoint(QDataStream &stream, TT::Point *point);
}

#endif // TT_STATIC_IO_H
