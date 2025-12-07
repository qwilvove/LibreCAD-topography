#include "tt_ui_dialog_plugin_settings.h"
#include "ui_tt_ui_dialog_plugin_settings.h"

#include "src/ui/dialog/tt_ui_dialog_codes_settings.h"
#include "src/ui/dialog/tt_ui_dialog_layers_settings.h"

TT_DialogPluginSettings::TT_DialogPluginSettings(QWidget *parent, PluginSettings *settings):
    QDialog(parent),
    settings(settings),
    ui(new Ui::TT_DialogPluginSettings)
{
    ui->setupUi(this);

    initUi();
    displayCurrentSettings();
}

TT_DialogPluginSettings::~TT_DialogPluginSettings()
{
    delete ui;
}

void TT_DialogPluginSettings::initUi()
{
    ui->cb_layerPoint->clear();
    ui->cb_layerName->clear();
    ui->cb_layerAlti->clear();

    for (TT::Layer *layer : *settings->getLayers())
    {
        ui->cb_layerPoint->addItem(layer->getName());
        ui->cb_layerName->addItem(layer->getName());
        ui->cb_layerAlti->addItem(layer->getName());
    }
}

void TT_DialogPluginSettings::displayCurrentSettings()
{
    ui->ck_autoSave->setChecked(settings->getAutoSaveOnQuit());

    ui->cb_layerPoint->setCurrentText(settings->getInsertionLayerPoint());
    ui->cb_layerName->setCurrentText(settings->getInsertionLayerName());
    ui->cb_layerAlti->setCurrentText(settings->getInsertionLayerAlti());
}

void TT_DialogPluginSettings::saveSettings()
{
    settings->setAutoSaveOnQuit(ui->ck_autoSave->isChecked());

    if (ui->cb_layerPoint->count() == 0)
    {
        settings->setInsertionLayerPoint("");
    }
    else
    {
        settings->setInsertionLayerPoint(ui->cb_layerPoint->currentText());
    }

    if (ui->cb_layerName->count() == 0)
    {
        settings->setInsertionLayerName("");
    }
    else
    {
        settings->setInsertionLayerName(ui->cb_layerName->currentText());
    }

    if (ui->cb_layerAlti->count() == 0)
    {
        settings->setInsertionLayerAlti("");
    }
    else
    {
        settings->setInsertionLayerAlti(ui->cb_layerAlti->currentText());
    }
}

void TT_DialogPluginSettings::slot_pbLayersSettingsPressed()
{
    TT_DialogLayersSettings layersSettingsDialog(this, settings);
    layersSettingsDialog.exec();
    initUi();
    displayCurrentSettings();
}

void TT_DialogPluginSettings::slot_pbCodesSettingsPressed()
{
    TT_DialogCodesSettings codesSettingsDialog(this, settings);
    codesSettingsDialog.exec();
}

void TT_DialogPluginSettings::slot_validateInputs()
{
    bool ok = false;

    if (1)
    {
        ok = true;
    }

    if (ok)
    {
        saveSettings();
        settings->write();
    }
}
