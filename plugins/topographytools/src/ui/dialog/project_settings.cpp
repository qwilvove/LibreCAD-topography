#include "project_settings.h"
#include "ui_project_settings.h"

#include <QMessageBox>

TT_DialogProjectSettings::TT_DialogProjectSettings(QWidget *parent):
    QDialog(parent),
    ui(new Ui::TT_DialogProjectSettings)
{
    ui->setupUi(this);

    setVisibleCustomScaleSettings(false);
    loadCurrentSettings();
}

TT_DialogProjectSettings::~TT_DialogProjectSettings()
{
    delete ui;
}

void TT_DialogProjectSettings::setVisibleCustomScaleSettings(bool visible)
{
    ui->lb_customScale->setVisible(visible);
    ui->le_customScale->setVisible(visible);
}

void TT_DialogProjectSettings::loadCurrentSettings()
{

}

void TT_DialogProjectSettings::saveSettings()
{

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
