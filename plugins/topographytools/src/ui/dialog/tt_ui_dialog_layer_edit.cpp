#include "tt_ui_dialog_layer_edit.h"
#include "ui_tt_ui_dialog_layer_edit.h"

#include <QMessageBox>

TT_DialogLayerEdit::TT_DialogLayerEdit(QWidget *parent, TT::PluginSettings *settings, int index):
    QDialog(parent),
    settings(settings),
    index(index),
    initialName(settings->getLayers()->at(index)->getName()),
    ui(new Ui::TT_DialogLayerEdit)
{
    ui->setupUi(this);

    initUi();
    loadLayerData();
}

TT_DialogLayerEdit::~TT_DialogLayerEdit()
{
    delete ui;
}

void TT_DialogLayerEdit::initUi()
{
    QStringList lwItems;
    QStringList ltItems;

    for (auto it = TT::Layer::LINE_WIDTHS.keyValueBegin(); it != TT::Layer::LINE_WIDTHS.keyValueEnd(); ++it)
    {
        lwItems.append(it->second);
    }

    for (auto it = TT::Layer::LINE_TYPES.keyValueBegin(); it != TT::Layer::LINE_TYPES.keyValueEnd(); ++it)
    {
        ltItems.append(it->second);
    }

    ui->cb_lineWidth->addItems(lwItems);
    ui->cb_lineType->addItems(ltItems);
}

void TT_DialogLayerEdit::loadLayerData()
{
    TT::Layer *layer = settings->getLayers()->at(index);

    ui->le_name->setText(layer->getName());
    ui->le_colour->setText(layer->getColour().name().toUpper());
    ui->cb_lineWidth->setCurrentText(TT::Layer::LINE_WIDTHS[layer->getLineWidth()]);
    ui->cb_lineType->setCurrentText(TT::Layer::LINE_TYPES[layer->getLineType()]);
}

bool TT_DialogLayerEdit::nameAlreadyExists(QString name)
{
    bool exists = false;

    for (TT::Layer *layer : *settings->getLayers())
    {
        if (layer->getName() == name)
        {
            exists = true;
            break;
        }
    }

    return exists;
}

void TT_DialogLayerEdit::slot_leColourTextChanged()
{
    QString text = ui->le_colour->text();

    // Match hex colour codes formats #FFFFFF and #FFF (both lowercase and uppercase)
    QRegularExpression format(R"(^#([0-9A-Fa-f]{6}|[0-9A-Fa-f]{3})$)");
    QRegularExpressionMatch match = format.match(text);
    if (match.hasMatch())
    {
        ui->lb_colourPreview->clear();
        ui->lb_colourPreview->setStyleSheet("background-color: " + text + ";");
    }
    else
    {
        ui->lb_colourPreview->setText("?");
        ui->lb_colourPreview->setStyleSheet("");
    }
}

void TT_DialogLayerEdit::slot_validateInputs()
{
    bool ok = false;

    QRegularExpression colourFormat(R"(^#([0-9A-Fa-f]{6}|[0-9A-Fa-f]{3})$)");

    if (ui->le_name->text().isEmpty())
    {
        QMessageBox::warning(this, tr("Error!"), tr("Layer name cannot be empty!"), QMessageBox::StandardButton::Ok);
    }
    else if (ui->le_name->text() == "0")
    {
        QMessageBox::warning(this, tr("Error!"), tr("Layer name cannot be 0!"), QMessageBox::StandardButton::Ok);
    }
    else if (nameAlreadyExists(ui->le_name->text()) && ui->le_name->text() != initialName)
    {
        QMessageBox::warning(this, tr("Error!"), tr("Layer name is already used by an existing layer!"), QMessageBox::StandardButton::Ok);
    }
    else if (!colourFormat.match(ui->le_colour->text()).hasMatch())
    {
        QMessageBox::warning(this, tr("Error!"), tr("Layer colour is not valid!"), QMessageBox::StandardButton::Ok);
    }
    else
    {
        ok = true;
    }

    if (ok)
    {
        TT::Layer *layer = settings->getLayers()->at(index);

        layer->setName(ui->le_name->text());
        layer->setColour(QColor(ui->le_colour->text()));
        layer->setLineWidth(TT::Layer::LINE_WIDTHS.key(ui->cb_lineWidth->currentText()));
        layer->setLineType(TT::Layer::LINE_TYPES.key(ui->cb_lineType->currentText()));

        // If renamed : assure right order
        if (layer->getName() != initialName)
        {
            settings->getLayers()->removeAt(index);

            int previousIndex = 0;
            while (previousIndex < settings->getLayers()->size())
            {
                if (settings->getLayers()->at(previousIndex)->getName() > layer->getName())
                {
                    break;
                }
                previousIndex++;
            }
            settings->getLayers()->insert(previousIndex, layer);
        }

        this->accept();
    }
}
