#include "tt_ui_dialog_code_edit.h"
#include "ui_tt_ui_dialog_code_edit.h"

#include <QMessageBox>

TT_DialogCodeEdit::TT_DialogCodeEdit(QWidget *parent, TT::PluginSettings *settings, int index):
    QDialog(parent),
    settings(settings),
    index(index),
    initialCode(settings->getCodes()->at(index)->getCode()),
    ui(new Ui::TT_DialogCodeEdit)
{
    ui->setupUi(this);

    initUi();
    loadCodeData();
}

TT_DialogCodeEdit::~TT_DialogCodeEdit()
{
    delete ui;
}

void TT_DialogCodeEdit::initUi()
{
    QStringList typeItems;
    QStringList layerItems;

    for (auto it = TT::Code::TYPES.keyValueBegin(); it != TT::Code::TYPES.keyValueEnd(); ++it)
    {
        typeItems.append(it->second);
    }

    for (TT::Layer *l : *settings->getLayers())
    {
        layerItems.append(l->getName());
    }

    ui->cb_type->addItems(typeItems);
    ui->cb_layer->addItems(layerItems);
}

void TT_DialogCodeEdit::loadCodeData()
{
    TT::Code *code = settings->getCodes()->at(index);

    ui->le_code->setText(code->getCode());
    ui->cb_type->setCurrentText(TT::Code::TYPES[code->getType()]);
    ui->le_blockName->setText(code->getBlockName());
    ui->cb_layer->setCurrentText(code->getLayer()->getName());
}

bool TT_DialogCodeEdit::codeAlreadyExists(QString code)
{
    bool exists = false;

    for (TT::Code *c : *settings->getCodes())
    {
        if (c->getCode() == code)
        {
            exists = true;
            break;
        }
    }

    return exists;
}

void TT_DialogCodeEdit::slot_cbTypeCurrentIndexChanged()
{
    if (TT::Code::isBlockType(TT::Code::TYPES.key(ui->cb_type->currentText())))
    {
        ui->le_blockName->setEnabled(true);
    }
    else
    {
        ui->le_blockName->setEnabled(false);
    }
}

void TT_DialogCodeEdit::slot_validateInputs()
{
    bool ok = false;

    if (ui->le_code->text().isEmpty())
    {
        QMessageBox::warning(this, tr("Error!"), tr("Code cannot be empty!"), QMessageBox::StandardButton::Ok);
    }
    else if (codeAlreadyExists(ui->le_code->text()) && ui->le_code->text() != initialCode)
    {
        QMessageBox::warning(this, tr("Error!"), tr("Code \"%1\" is already defined!").arg(ui->le_code->text()), QMessageBox::StandardButton::Ok);
    }
    else if (ui->cb_layer->count() == 0)
    {
        QMessageBox::warning(this, tr("Error!"), tr("Please define at least one layer to drawn code in!"), QMessageBox::StandardButton::Ok);
    }
    else
    {
        ok = true;
    }

    if (ok)
    {
        TT::Code *code = settings->getCodes()->at(index);

        code->setCode(ui->le_code->text());
        code->setType(TT::Code::TYPES.key(ui->cb_type->currentText()));
        code->setBlockName(ui->le_blockName->text());
        code->setLayer(settings->getLayerByName(ui->cb_layer->currentText()));

        // If renamed : assure right order
        if (code->getCode() != initialCode)
        {
            settings->getCodes()->removeAt(index);

            int previousIndex = 0;
            while (previousIndex < settings->getCodes()->size())
            {
                if (settings->getCodes()->at(previousIndex)->getCode() > code->getCode())
                {
                    break;
                }
                previousIndex++;
            }
            settings->getCodes()->insert(previousIndex, code);
        }

        this->accept();
    }
}
