#ifndef TT_CLASS_PLUGIN_SETTINGS_H
#define TT_CLASS_PLUGIN_SETTINGS_H

#include <QMap>
#include <QString>

#include "src/class/tt_class_code.h"
#include "src/class/tt_class_layer.h"

class PluginSettings
{
public:
    PluginSettings();
    ~PluginSettings();

    bool read();
    bool write();

    QString getFileName();
    bool getAutoSaveOnQuit();
    QString getInsertionLayerPoint();
    QString getInsertionLayerName();
    QString getInsertionLayerAlti();
    QList<Layer *> *getLayers();
    Layer *getLayerByName(QString name);
    QList<Code *> *getCodes();
    Code *getCodeByCode(QString code);

    void setFileName(QString fileName);
    void setAutoSaveOnQuit(bool autoSaveOnQuit);
    void setInsertionLayerPoint(QString insertionLayerPoint);
    void setInsertionLayerName(QString insertionLayerName);
    void setInsertionLayerAlti(QString insertionLayerAlti);

    bool removeLayerAt(int index);
    bool removeCodeAt(int index);

private:
    QString fileName;
    bool autoSaveOnQuit;
    QString insertionLayerPoint;
    QString insertionLayerName;
    QString insertionLayerAlti;
    QList<Layer*> *layers;
    QList<Code*> *codes;
};

#endif // TT_CLASS_PLUGIN_SETTINGS_H
