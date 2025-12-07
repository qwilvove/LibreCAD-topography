#ifndef TT_UI_DIALOG_LAYERS_SETTINGS_H
#define TT_UI_DIALOG_LAYERS_SETTINGS_H

#include <QDialog>

#include "src/tt.h"

namespace Ui {
class TT_DialogLayersSettings;
}

class TT_DialogLayersSettings : public QDialog
{
    Q_OBJECT

public:
    explicit TT_DialogLayersSettings(QWidget *parent, TT::PluginSettings *settings);
    ~TT_DialogLayersSettings();

private:
    TT::PluginSettings *settings;
    Ui::TT_DialogLayersSettings *ui;

    void displayLayers();

private slots:
    void slot_addLayer();
    void slot_removeLayer();
    void slot_editLayer();
};

#endif // TT_UI_DIALOG_LAYERS_SETTINGS_H
