#include "plugin_settings.h"

#include <QSettings>

const QMap<QString, PluginSettings::CODE> PluginSettings::DEFAULT_CODES = {
    { "0", PluginSettings::CODE::LINE_INIT         },
    { "1", PluginSettings::CODE::LINE_CONTINUE     },
    { "2", PluginSettings::CODE::ARC_INIT          },
    { "3", PluginSettings::CODE::ARC_MIDDLE        },
    { "4", PluginSettings::CODE::ARC_CONTINUE      },
    { "5", PluginSettings::CODE::OFFSET            },
    { "6", PluginSettings::CODE::ORIENTATION       },
    { "7", PluginSettings::CODE::CLOSE_WITH_OFFSET },
    { "8", PluginSettings::CODE::CLOSE             },
    { "9", PluginSettings::CODE::END               }
};

PluginSettings::PluginSettings()
{
    this->fileName = "";
    this->autoSaveOnQuit = false;
    this->codes = new QMap(PluginSettings::DEFAULT_CODES);
}

PluginSettings::~PluginSettings()
{
    delete this->codes;
}

bool PluginSettings::read()
{
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, "LibreCAD", "topographytools");
    fileName = settings.value("fileName", fileName).toString();
    autoSaveOnQuit = settings.value("autoSaveOnQuit", autoSaveOnQuit).toBool();

    bool mapCleared = false;
    foreach (const QString &key, settings.allKeys())
    {
        if (key.startsWith("Codes/"))
        {
            int value = settings.value(key, 0).toInt();
            if ((CODE)value == CODE::NONE || DEFAULT_CODES.values().contains((CODE)value))
            {
                if (!mapCleared) // Clear Map only if at least one valid code is set in settings, otherwise don't clear (use defaults)
                {
                    codes->clear();
                    mapCleared = true;
                }

                QString editedKey(key);
                editedKey.remove(0,6); // e.g. Codes/1 to 1
                codes->insert(editedKey, (CODE)value);
            }
        }
    }

    return true;
}

bool PluginSettings::write()
{
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, "LibreCAD", "topographytools");
    settings.setValue("fileName", fileName);
    settings.setValue("autoSaveOnQuit", autoSaveOnQuit);

    settings.beginGroup("Codes");
    for (auto it = codes->keyValueBegin(); it != codes->keyValueEnd(); ++it)
    {
        settings.setValue(it->first, (int)it->second);
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

PluginSettings::CODE PluginSettings::getCode(QString codeValue)
{
    return codes->value(codeValue, CODE::NONE);
}

void PluginSettings::setFileName(QString fileName)
{
    this->fileName = fileName;
}

void PluginSettings::setAutoSaveOnQuit(bool autoSaveOnQuit)
{
    this->autoSaveOnQuit = autoSaveOnQuit;
}

void PluginSettings::setCode(QString codeValue, PluginSettings::CODE codeAction)
{
    this->codes->insert(codeValue, codeAction);
}
