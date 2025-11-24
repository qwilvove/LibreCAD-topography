#ifndef TT_CLASS_PLUGIN_SETTINGS_H
#define TT_CLASS_PLUGIN_SETTINGS_H

#include <QMap>
#include <QString>

class PluginSettings
{
public:
    enum class CODE
    {
        NONE,

        LINE_INIT,
        LINE_CONTINUE,
        ARC_INIT,
        ARC_MIDDLE,
        ARC_CONTINUE,
        OFFSET,
        ORIENTATION,
        CLOSE_WITH_OFFSET,
        CLOSE,
        END
    };

    PluginSettings();
    ~PluginSettings();

    bool read();
    bool write();

    QString getFileName();
    bool getAutoSaveOnQuit();
    CODE getCode(QString codeValue);

    void setFileName(QString fileName);
    void setAutoSaveOnQuit(bool autoSaveOnQuit);
    void setCode(QString codeValue, CODE codeAction);

private:
    static const QMap<QString, CODE> DEFAULT_CODES;

    QString fileName;
    bool autoSaveOnQuit;
    QMap<QString, CODE> *codes;
};

#endif // TT_CLASS_PLUGIN_SETTINGS_H
