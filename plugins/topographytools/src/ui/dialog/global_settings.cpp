#include "global_settings.h"
#include "ui_global_settings.h"

TT_DialogGlobalSettings::TT_DialogGlobalSettings(QWidget *parent):
    QDialog(parent),
    ui(new Ui::TT_DialogGlobalSettings)
{
    ui->setupUi(this);
}

TT_DialogGlobalSettings::~TT_DialogGlobalSettings()
{
    delete ui;
}
