#include "tt_ui_dialog_layers_settings.h"
#include "ui_tt_ui_dialog_layers_settings.h"

#include <QMessageBox>

#include "src/ui/dialog/tt_ui_dialog_layer_add.h"
#include "src/ui/dialog/tt_ui_dialog_layer_edit.h"

TT_DialogLayersSettings::TT_DialogLayersSettings(QWidget *parent, TT::PluginSettings *settings):
    QDialog(parent),
    settings(settings),
    ui(new Ui::TT_DialogLayersSettings)
{
    ui->setupUi(this);

    displayLayers();
}

TT_DialogLayersSettings::~TT_DialogLayersSettings()
{
    delete ui;
}

void TT_DialogLayersSettings::displayLayers()
{
    ui->tw->clearContents();
    ui->tw->setRowCount(0);

    foreach (TT::Layer *layer, *settings->getLayers())
    {
        // Add one line at the end
        int lineNumber = ui->tw->rowCount() + 1;
        ui->tw->setRowCount(lineNumber);

        // Fill the row
        ui->tw->setItem(lineNumber - 1, 0, layer->getQTableWidgetItemName());
        ui->tw->setItem(lineNumber - 1, 1, layer->getQTableWidgetItemColour());
        ui->tw->setItem(lineNumber - 1, 2, layer->getQTableWidgetItemLineWidth());
        ui->tw->setItem(lineNumber - 1, 3, layer->getQTableWidgetItemLineType());
    }
}

void TT_DialogLayersSettings::slot_addLayer()
{
    TT_DialogLayerAdd layerAddDialog(this, settings);
    layerAddDialog.exec();
    displayLayers();
}

void TT_DialogLayersSettings::slot_removeLayer()
{
    if (ui->tw->selectedItems().isEmpty())
    {
        return;
    }

    int index = ui->tw->currentRow();

    if (!settings->removeLayerAt(index))
    {
        QMessageBox::warning(this, tr("Error!"), tr("Cannot remove layer \"%1\" due to its use by at least one code!").arg(settings->getLayers()->at(index)->getName()), QMessageBox::StandardButton::Ok);
        return;
    }

    displayLayers();
}

void TT_DialogLayersSettings::slot_editLayer()
{
    if (ui->tw->selectedItems().isEmpty())
    {
        return;
    }

    TT_DialogLayerEdit layerEditDialog(this, settings, ui->tw->currentRow());
    layerEditDialog.exec();
    displayLayers();
}
