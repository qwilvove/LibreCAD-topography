#include "plugin_settings.h"

#include <QSettings>

PluginSettings::PluginSettings()
{
    this->fileName = "";
    this->autoSaveOnQuit = true;
    this->insertionLayerPoint = "";
    this->insertionLayerName = "";
    this->insertionLayerAlti = "";
    this->codes = new QList<Code*>();
    this->layers = new QList<Layer*>();
}

PluginSettings::~PluginSettings()
{
    for (Code *c : *this->codes)
    {
        delete c;
    }
    delete this->codes;

    for (Layer *l : *this->layers)
    {
        delete l;
    }
    delete this->layers;
}

bool PluginSettings::read()
{
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, "LibreCAD", "topographytools");
    fileName = settings.value("fileName", fileName).toString();
    autoSaveOnQuit = settings.value("autoSaveOnQuit", autoSaveOnQuit).toBool();
    insertionLayerPoint = settings.value("insertionLayerPoint", insertionLayerPoint).toString();
    insertionLayerName = settings.value("insertionLayerName", insertionLayerName).toString();
    insertionLayerAlti = settings.value("insertionLayerAlti", insertionLayerAlti).toString();

    layers->clear();
    foreach (const QString &key, settings.allKeys())
    {
        if (key.startsWith("Layers/"))
        {
            QString name = key;
            name.remove(0,7); // e.g. Layers/Foo to Foo
            QStringList list = settings.value(key, "").toString().split(",");
            layers->append(new Layer(name, QColor(list.at(0)), (DPI::LineWidth)list.at(1).toInt(), (DPI::LineType)list.at(2).toInt()));
        }
    }

    codes->clear();
    foreach (const QString &key, settings.allKeys())
    {
        if (key.startsWith("Codes/"))
        {
            QString code = key;
            code.remove(0,6); // e.g. Codes/10 to 10
            QStringList list = settings.value(key, "").toString().split(",");
            Layer *layer = getLayerByName(list.at(2));
            codes->append(new Code(code, (Code::TYPE)list.at(0).toInt(), list.at(1), layer));
        }
    }

    return true;
}

bool PluginSettings::write()
{
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, "LibreCAD", "topographytools");
    settings.clear();
    settings.setValue("fileName", fileName);
    settings.setValue("autoSaveOnQuit", autoSaveOnQuit);
    settings.setValue("insertionLayerPoint", insertionLayerPoint);
    settings.setValue("insertionLayerName", insertionLayerName);
    settings.setValue("insertionLayerAlti", insertionLayerAlti);

    settings.beginGroup("Layers");
    for (Layer *l : *layers)
    {
        settings.setValue(l->getName(), QString("%1,%2,%3").arg(l->getColour().name(), QString::number((int)l->getLineWidth()), QString::number((int)l->getLineType())));
    }
    settings.endGroup();

    settings.beginGroup("Codes");
    for (Code *c : *codes)
    {
        settings.setValue(c->getCode(), QString("%1,%2,%3").arg(QString::number((int)(c->getType())), c->getBlockName(), c->getLayer()->getName()));
    }
    settings.endGroup();

    return true;
}

QString PluginSettings::getFileName()
{
    return fileName;
}

bool PluginSettings::getAutoSaveOnQuit()
{
    return autoSaveOnQuit;
}

QString PluginSettings::getInsertionLayerPoint()
{
    return insertionLayerPoint;
}

QString PluginSettings::getInsertionLayerName()
{
    return insertionLayerName;
}

QString PluginSettings::getInsertionLayerAlti()
{
    return insertionLayerAlti;
}

QList<Layer*> *PluginSettings::getLayers()
{
    return layers;
}

Layer *PluginSettings::getLayerByName(QString name)
{
    Layer *layer = nullptr;

    for (Layer *l : *layers)
    {
        if (l->getName() == name)
        {
            layer = l;
            break;
        }
    }

    return layer;
}

QList<Code *> *PluginSettings::getCodes()
{
    return codes;
}

Code *PluginSettings::getCodeByCode(QString code)
{
    Code *returned_code = nullptr;

    for (Code *c : *codes)
    {
        if (c->getCode() == code)
        {
            returned_code = c;
            break;
        }
    }

    return returned_code;
}

void PluginSettings::setFileName(QString fileName)
{
    this->fileName = fileName;
}

void PluginSettings::setAutoSaveOnQuit(bool autoSaveOnQuit)
{
    this->autoSaveOnQuit = autoSaveOnQuit;
}

void PluginSettings::setInsertionLayerPoint(QString insertionLayerPoint)
{
    this->insertionLayerPoint = insertionLayerPoint;
}

void PluginSettings::setInsertionLayerName(QString insertionLayerName)
{
    this->insertionLayerName = insertionLayerName;
}

void PluginSettings::setInsertionLayerAlti(QString insertionLayerAlti)
{
    this->insertionLayerAlti = insertionLayerAlti;
}

bool PluginSettings::removeLayerAt(int index)
{
    if (index < 0 || index >= layers->size())
    {
        return false;
    }

    Layer *layer = layers->at(index);

    for (Code *c : *codes)
    {
        if (c->getLayer() == layer)
        {
            return false;
        }
    }

    layers->removeAt(index);
    delete layer;

    return true;
}

bool PluginSettings::removeCodeAt(int index)
{
    if (index < 0 || index >= codes->size())
    {
        return false;
    }

    Code *code = codes->at(index);

    codes->removeAt(index);
    delete code;

    return true;
}
