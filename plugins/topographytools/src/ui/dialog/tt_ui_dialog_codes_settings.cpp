#include "tt_ui_dialog_codes_settings.h"
#include "ui_tt_ui_dialog_codes_settings.h"

#include <QMessageBox>

#include "src/ui/dialog/tt_ui_dialog_code_add.h"
#include "src/ui/dialog/tt_ui_dialog_code_edit.h"

TT_DialogCodesSettings::TT_DialogCodesSettings(QWidget *parent, TT::PluginSettings *settings):
    QDialog(parent),
    settings(settings),
    ui(new Ui::TT_DialogCodesSettings)
{
    ui->setupUi(this);

    displayCodes();
}

TT_DialogCodesSettings::~TT_DialogCodesSettings()
{
    delete ui;
}

void TT_DialogCodesSettings::displayCodes()
{
    ui->tw->clearContents();
    ui->tw->setRowCount(0);

    foreach (TT::Code *code, *settings->getCodes())
    {
        // Add one line at the end
        int newIndex = ui->tw->rowCount();
        ui->tw->setRowCount(newIndex + 1);

        // Fill the row
        ui->tw->setItem(newIndex, 0, code->getTwiCode());
        ui->tw->setItem(newIndex, 1, code->getTwiType());
        ui->tw->setItem(newIndex, 2, code->getTwiBlockName());
        ui->tw->setItem(newIndex, 3, code->getTwiLayer());
    }
}

void TT_DialogCodesSettings::slot_addCode()
{
    TT_DialogCodeAdd codeAddDialog(this, settings);
    codeAddDialog.exec();
    displayCodes();
}

void TT_DialogCodesSettings::slot_removeCode()
{
    if (ui->tw->selectedItems().isEmpty())
    {
        return;
    }

    int index = ui->tw->currentRow();

    if (!settings->removeCodeAt(index))
    {
        QMessageBox::warning(this, tr("Error!"), tr("Cannot remove code \"%1\"!").arg(settings->getCodes()->at(index)->getCode()), QMessageBox::StandardButton::Ok);
        return;
    }

    displayCodes();
}

void TT_DialogCodesSettings::slot_editCode()
{
    if (ui->tw->selectedItems().isEmpty())
    {
        return;
    }

    TT_DialogCodeEdit codeEditDialog(this, settings, ui->tw->currentRow());
    codeEditDialog.exec();
    displayCodes();
}
