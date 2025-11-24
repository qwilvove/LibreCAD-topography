#include "tt_ui_dialog_plugin_settings.h"
#include "ui_tt_ui_dialog_plugin_settings.h"

TT_DialogPluginSettings::TT_DialogPluginSettings(QWidget *parent, PluginSettings *settings):
    QDialog(parent),
    settings(settings),
    ui(new Ui::TT_DialogPluginSettings)
{
    ui->setupUi(this);

    displayCurrentSettings();
}

TT_DialogPluginSettings::~TT_DialogPluginSettings()
{
    delete ui;
}

void TT_DialogPluginSettings::displayCurrentSettings()
{
    ui->ck_autoSave->setChecked(settings->getAutoSaveOnQuit());
    ui->cb_code0->setCurrentIndex((int)settings->getCode("0"));
    ui->cb_code1->setCurrentIndex((int)settings->getCode("1"));
    ui->cb_code2->setCurrentIndex((int)settings->getCode("2"));
    ui->cb_code3->setCurrentIndex((int)settings->getCode("3"));
    ui->cb_code4->setCurrentIndex((int)settings->getCode("4"));
    ui->cb_code5->setCurrentIndex((int)settings->getCode("5"));
    ui->cb_code6->setCurrentIndex((int)settings->getCode("6"));
    ui->cb_code7->setCurrentIndex((int)settings->getCode("7"));
    ui->cb_code8->setCurrentIndex((int)settings->getCode("8"));
    ui->cb_code9->setCurrentIndex((int)settings->getCode("9"));
}

void TT_DialogPluginSettings::saveSettings()
{
    settings->setAutoSaveOnQuit(ui->ck_autoSave->isChecked());
    settings->setCode("0", (PluginSettings::CODE)ui->cb_code0->currentIndex());
    settings->setCode("1", (PluginSettings::CODE)ui->cb_code1->currentIndex());
    settings->setCode("2", (PluginSettings::CODE)ui->cb_code2->currentIndex());
    settings->setCode("3", (PluginSettings::CODE)ui->cb_code3->currentIndex());
    settings->setCode("4", (PluginSettings::CODE)ui->cb_code4->currentIndex());
    settings->setCode("5", (PluginSettings::CODE)ui->cb_code5->currentIndex());
    settings->setCode("6", (PluginSettings::CODE)ui->cb_code6->currentIndex());
    settings->setCode("7", (PluginSettings::CODE)ui->cb_code7->currentIndex());
    settings->setCode("8", (PluginSettings::CODE)ui->cb_code8->currentIndex());
    settings->setCode("9", (PluginSettings::CODE)ui->cb_code9->currentIndex());
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
        this->accept();
    }
}
