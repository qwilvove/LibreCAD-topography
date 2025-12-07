#include "tt_ui_dialog_layer_add.h"
#include "ui_tt_ui_dialog_layer_add.h"

#include <QMessageBox>

TT_DialogLayerAdd::TT_DialogLayerAdd(QWidget *parent, TT::PluginSettings *settings):
    QDialog(parent),
    settings(settings),
    ui(new Ui::TT_DialogLayerAdd)
{
    ui->setupUi(this);

    initUi();
}

TT_DialogLayerAdd::~TT_DialogLayerAdd()
{
    delete ui;
}

void TT_DialogLayerAdd::initUi()
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

bool TT_DialogLayerAdd::nameAlreadyExists(QString name)
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

void TT_DialogLayerAdd::slot_leColourTextChanged()
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

void TT_DialogLayerAdd::slot_validateInputs()
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
    else if (nameAlreadyExists(ui->le_name->text()))
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
        TT::Layer *new_layer = new Layer(
            ui->le_name->text(),
            QColor(ui->le_colour->text()),
            TT::Layer::LINE_WIDTHS.key(ui->cb_lineWidth->currentText()),
            TT::Layer::LINE_TYPES.key(ui->cb_lineType->currentText())
            );

        int previousIndex = 0;
        while (previousIndex < settings->getLayers()->size())
        {
            if (settings->getLayers()->at(previousIndex)->getName() > new_layer->getName())
            {
                break;
            }
            previousIndex++;
        }
        settings->getLayers()->insert(previousIndex, new_layer);

        this->accept();
    }
}
