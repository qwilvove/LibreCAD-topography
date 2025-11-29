#include "tt_ui_dialog_project_settings.h"
#include "ui_tt_ui_dialog_project_settings.h"

#include <QMessageBox>

TT_DialogProjectSettings::TT_DialogProjectSettings(QWidget *parent, TT::ProjectSettings *settings):
    QDialog(parent),
    settings(settings),
    ui(new Ui::TT_DialogProjectSettings)
{
    ui->setupUi(this);

    setVisibleCustomScaleSettings(false);
    displayCurrentSettings();
}

TT_DialogProjectSettings::~TT_DialogProjectSettings()
{
    delete ui;
}

void TT_DialogProjectSettings::displayCurrentSettings()
{
    double scale = settings->getScale();
    if (scale == 1.0/100)
    {
        ui->cb_scale->setCurrentIndex(0);
    }
    else if (scale == 1.0/200)
    {
        ui->cb_scale->setCurrentIndex(1);
    }
    else if (scale == 1.0/500)
    {
        ui->cb_scale->setCurrentIndex(2);
    }
    else
    {
        ui->cb_scale->setCurrentIndex(3);
        ui->le_customScale->setText("1:" + QString::number(1.0/settings->getScale(), 'f', 0));
    }
}

void TT_DialogProjectSettings::saveSettings()
{
    int currentIndex = ui->cb_scale->currentIndex();
    if (currentIndex == 0)
    {
        settings->setScale(1.0/100);
    }
    else if (currentIndex == 1)
    {
        settings->setScale(1.0/200);
    }
    else if (currentIndex == 2)
    {
        settings->setScale(1.0/500);
    }
    else
    {
        QString customScale = ui->le_customScale->text();
        int rightPart = customScale.remove(0,2).toInt(); // Keep everything after 1: (or 1/) e.g. "750" for "1:750"
        settings->setScale(1.0/rightPart);
    }
}

void TT_DialogProjectSettings::setVisibleCustomScaleSettings(bool visible)
{
    ui->lb_customScale->setVisible(visible);
    ui->le_customScale->setVisible(visible);
}

void TT_DialogProjectSettings::slot_cbScaleIndexChanged(int index)
{
    bool visibility = index == ui->cb_scale->count() - 1; // Which means "Custom" is selected (last index)

    setVisibleCustomScaleSettings(visibility);
}

void TT_DialogProjectSettings::slot_leCustomScaleTextChanged(QString text)
{
    Q_UNUSED(text)

    ui->le_customScale->setStyleSheet("");
}

void TT_DialogProjectSettings::slot_validateInputs()
{
    bool ok = false;

    if (ui->cb_scale->currentIndex() == ui->cb_scale->count() - 1) // "Custom" selected
    {
        // Custom scale must be between 1:1 and 1:100000 ( / is also accepted instead of : )
        QRegularExpression format(R"(^1([:/])(100000|([1-9][0-9]{0,4}|[1-9][0-9]?))$)");
        QRegularExpressionMatch match = format.match(ui->le_customScale->text());
        if (match.hasMatch())
        {
            ok = true;
        }
        else
        {
            ui->le_customScale->setStyleSheet("color: red;");
            QMessageBox::warning(this, tr("Error!"), tr("Custom scale format is not valid!"), QMessageBox::StandardButton::Ok);
        }
    }
    else
    {
        ok = true;
    }

    if (ok)
    {
        saveSettings();
        this->accept();
    }
}
