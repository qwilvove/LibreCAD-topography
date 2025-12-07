#ifndef TT_UI_DIALOG_PLUGIN_SETTINGS_H
#define TT_UI_DIALOG_PLUGIN_SETTINGS_H

#include <QDialog>

#include "src/tt.h"

namespace Ui {
class TT_DialogPluginSettings;
}

class TT_DialogPluginSettings : public QDialog
{
    Q_OBJECT

public:
    explicit TT_DialogPluginSettings(QWidget *parent, TT::PluginSettings *settings);
    ~TT_DialogPluginSettings();

private:
    TT::PluginSettings *settings;
    Ui::TT_DialogPluginSettings *ui;

    void initUi();
    void displayCurrentSettings();
    void saveSettings();

private slots:
    void slot_pbLayersSettingsPressed();
    void slot_pbCodesSettingsPressed();
    void slot_validateInputs();
};

#endif // TT_UI_DIALOG_PLUGIN_SETTINGS_H
