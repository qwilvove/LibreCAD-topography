#ifndef TT_STATIC_IO_H
#define TT_STATIC_IO_H

#include "src/tt.h"

namespace io
{
    int readTtFile(QString const &fileName, TT::ProjectSettings *projectSettings, QList<TT::Point*> *points);
    void readTtFileSettings(QDataStream &stream, TT::ProjectSettings *settings);
    void readTtFilePoints(QDataStream &stream, QList<TT::Point*> *points);
    void readTtFilePoint(QDataStream &stream, TT::Point *point);

    int writeTtFile(QString const &fileName, TT::ProjectSettings *projectSettings, QList<TT::Point*> *points);
    void writeTtFileSettings(QDataStream &stream, TT::ProjectSettings *settings);
    void writeTtFilePoints(QDataStream &stream, QList<TT::Point*> *points);
    void writeTtFilePoint(QDataStream &stream, TT::Point *point);
}

#endif // TT_STATIC_IO_H
