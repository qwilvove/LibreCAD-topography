#include "tt_ui_dialog_code_add.h"
#include "ui_tt_ui_dialog_code_add.h"

#include <QMessageBox>

TT_DialogCodeAdd::TT_DialogCodeAdd(QWidget *parent, TT::PluginSettings *settings):
    QDialog(parent),
    settings(settings),
    ui(new Ui::TT_DialogCodeAdd)
{
    ui->setupUi(this);

    initUi();
}

TT_DialogCodeAdd::~TT_DialogCodeAdd()
{
    delete ui;
}

void TT_DialogCodeAdd::initUi()
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

bool TT_DialogCodeAdd::codeAlreadyExists(QString code)
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

void TT_DialogCodeAdd::slot_cbTypeCurrentIndexChanged()
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

void TT_DialogCodeAdd::slot_validateInputs()
{
    bool ok = false;

    if (ui->le_code->text().isEmpty())
    {
        QMessageBox::warning(this, tr("Error!"), tr("Code cannot be empty!"), QMessageBox::StandardButton::Ok);
    }
    else if (codeAlreadyExists(ui->le_code->text()))
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
        TT::Code *new_code = new Code(
            ui->le_code->text(),
            TT::Code::TYPES.key(ui->cb_type->currentText()),
            ui->le_blockName->text(),
            settings->getLayerByName(ui->cb_layer->currentText())
            );

        int previousIndex = 0;
        while (previousIndex < settings->getCodes()->size())
        {
            if (settings->getCodes()->at(previousIndex)->getCode() > new_code->getCode())
            {
                break;
            }
            previousIndex++;
        }
        settings->getCodes()->insert(previousIndex, new_code);

        this->accept();
    }
}
