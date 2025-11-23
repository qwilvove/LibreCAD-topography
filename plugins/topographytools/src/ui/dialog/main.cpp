#include "main.h"
#include "ui_main.h"

#include "document_interface.h"

#include "src/topographytools.h"
#include "src/static/io.h"
#include "src/ui/dialog/add.h"
#include "src/ui/dialog/draw_blocks.h"
#include "src/ui/dialog/edit.h"
#include "src/ui/dialog/global_settings.h"
#include "src/ui/dialog/grid.h"
#include "src/ui/dialog/import.h"
#include "src/ui/dialog/points.h"
#include "src/ui/dialog/polygo.h"
#include "src/ui/dialog/project_settings.h"
#include "src/ui/dialog/v0.h"

#include <QFileDialog>
#include <QMenuBar>
#include <QMessageBox>
#include <QTimer>
#include <QToolBar>

// Constructor
TT_DialogMain::TT_DialogMain(QWidget *parent, Document_Interface *doc) :
    QDialog(parent),
    ui(new Ui::TT_DialogMain),
    doc(doc)
{
    this->isRunning = true;
    connect(this, &QDialog::rejected, this, [this]{this->isRunning = false;});

    ui->setupUi(this);
    initMenuBarAndToolbar();

    IO::PluginSettings ps = { &fileName };
    IO::readPluginSettings(&ps); // Find current fileName

    loadTtFile();
}

// Destructor
TT_DialogMain::~TT_DialogMain()
{
    delete ui;
}

void TT_DialogMain::showEvent(QShowEvent *event)
{
    QDialog::showEvent(event);
    QTimer::singleShot(0, this, SLOT(loadPreviousState()));
}

void TT_DialogMain::initMenuBarAndToolbar()
{
    /* DISABLE PROJECT RELATED ACTIONS BY DEFAULT */
    //ui->actionNew->setDisabled(true);
    //ui->actionOpen->setDisabled(true);
    ui->actionSave->setDisabled(true);
    ui->actionImport->setDisabled(true);
    ui->actionAdd->setDisabled(true);
    ui->actionRemove->setDisabled(true);
    ui->actionEdit->setDisabled(true);
    ui->actionUp->setDisabled(true);
    ui->actionDown->setDisabled(true);
    ui->actionCalculateV0->setDisabled(true);
    ui->actionCalculatePolygonation->setDisabled(true);
    ui->actionCalculatePoints->setDisabled(true);
    ui->actionDrawPoints->setDisabled(true);
    ui->actionDrawBlocks->setDisabled(true);
    ui->actionDrawGrid->setDisabled(true);
    //ui->actionGlobalSettings->setDisabled(true);
    ui->actionProjectSettings->setDisabled(true);

    /* MENUBAR */
    QMenuBar* mb = new QMenuBar();

    QMenu *menu = new QMenu(tr("Menu"));
    menu->addAction(ui->actionNew);
    menu->addAction(ui->actionOpen);
    menu->addAction(ui->actionSave);
    menu->addSeparator();
    menu->addAction(ui->actionImport);
    menu->addSeparator();
    menu->addAction(ui->actionAdd);
    menu->addAction(ui->actionRemove);
    menu->addAction(ui->actionEdit);
    menu->addAction(ui->actionUp);
    menu->addAction(ui->actionDown);
    menu->addSeparator();
    menu->addAction(ui->actionCalculateV0);
    menu->addAction(ui->actionCalculatePolygonation);
    menu->addAction(ui->actionCalculatePoints);
    menu->addSeparator();
    menu->addAction(ui->actionDrawPoints);
    menu->addAction(ui->actionDrawBlocks);
    menu->addAction(ui->actionDrawGrid);
    mb->addMenu(menu);

    QMenu *menu_settings = new QMenu(tr("Settings"));
    menu_settings->addAction(ui->actionGlobalSettings);
    menu_settings->addAction(ui->actionProjectSettings);
    mb->addMenu(menu_settings);

    ui->vlToolBar->addWidget(mb);

    /* TOOLBAR */
    QHBoxLayout *hbl_toolbars = new QHBoxLayout();

    QToolBar* tb = new QToolBar();
    tb->addAction(ui->actionNew);
    tb->addAction(ui->actionOpen);
    tb->addAction(ui->actionSave);
    tb->addSeparator();
    tb->addAction(ui->actionImport);
    tb->addSeparator();
    tb->addAction(ui->actionAdd);
    tb->addAction(ui->actionRemove);
    tb->addAction(ui->actionEdit);
    tb->addAction(ui->actionUp);
    tb->addAction(ui->actionDown);
    tb->addSeparator();
    tb->addAction(ui->actionCalculateV0);
    tb->addAction(ui->actionCalculatePolygonation);
    tb->addAction(ui->actionCalculatePoints);
    tb->addSeparator();
    tb->addAction(ui->actionDrawPoints);
    tb->addAction(ui->actionDrawBlocks);
    tb->addAction(ui->actionDrawGrid);
    hbl_toolbars->addWidget(tb);

    QSpacerItem *spacer = new QSpacerItem(1, 0, QSizePolicy::Expanding);
    hbl_toolbars->addSpacerItem(spacer);

    QToolBar *tb_settings = new QToolBar();
    tb_settings->addAction(ui->actionGlobalSettings);
    tb_settings->addAction(ui->actionProjectSettings);
    hbl_toolbars->addWidget(tb_settings);

    ui->vlToolBar->addLayout(hbl_toolbars);
}

void TT_DialogMain::loadTtFile()
{
    IO::TtFileData ttfd = { &fileName, nullptr, &points };
    int returnedValue = IO::readTtFile(&ttfd);
    if (returnedValue == 0)
    {
        ui->label->setText(tr("Active file : %1").arg(fileName));
    }
    else if (returnedValue == -2)
    {
        ui->label->setText(tr("Active file : none | %1 does not exist!").arg(fileName));
    }
    else if (returnedValue == -3)
    {
        QMessageBox::critical(this, tr("Error!"), tr("Could not open file!"));
    }

    if ( points.size() > -1 )
    {
        displayPoints();
        enableAllTools();
    }
}

void TT_DialogMain::savePreviousState(DIALOG dialog, int tabIndex, int insertTypeIndex, TT::BLOCK_INSERTION_TYPE insertType)
{
    this->previousState.dialog = dialog;
    this->previousState.tabIndex = tabIndex;
    this->previousState.insertTypeIndex = insertTypeIndex;
    this->previousState.insertType = insertType;
}

void TT_DialogMain::loadPreviousState()
{
    switch (this->previousState.dialog)
    {
    case DIALOG::NONE:
        break;
    case DIALOG::DRAW_BLOCKS:
    {
        TT_DialogDrawBlocks drawBlocksDialog(this, doc);
        drawBlocksDialog.loadPreviousState(this->previousState.tabIndex, this->previousState.insertTypeIndex, this->previousState.insertType);
        drawBlocksDialog.exec();
        break;
    }
    case DIALOG::DRAW_GRID:
        break;
    default:
        break;
    }
}

// Update tableWidget to show point list to the user
void TT_DialogMain::displayPoints()
{
    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(0);

    foreach (TT::Point *point, points)
    {
        displayPoint(point);
    }
}

// Add a line to tableWidget and fill it with point info
void TT_DialogMain::displayPoint(TT::Point *point)
{
    // Add one line at the end
    int lineNumber = ui->tableWidget->rowCount() + 1;
    ui->tableWidget->setRowCount(lineNumber);

    // Fill the row
    ui->tableWidget->setItem(lineNumber - 1, 0, point->getQTableWidgetItemLineNumber(lineNumber));
    ui->tableWidget->setItem(lineNumber - 1, 1, point->getQTableWidgetItemType());
    ui->tableWidget->setItem(lineNumber - 1, 2, point->getQTableWidgetItemName());
    ui->tableWidget->setItem(lineNumber - 1, 3, point->getQTableWidgetItemParameters());
}

// Enable all tools in the top toolbar
void TT_DialogMain::enableAllTools()
{
    //ui->actionNew->setEnabled(true);
    //ui->actionOpen->setEnabled(true);
    ui->actionSave->setEnabled(true);
    ui->actionImport->setEnabled(true);
    ui->actionAdd->setEnabled(true);
    ui->actionRemove->setEnabled(true);
    ui->actionEdit->setEnabled(true);
    ui->actionUp->setEnabled(true);
    ui->actionDown->setEnabled(true);
    ui->actionCalculateV0->setEnabled(true);
    ui->actionCalculatePolygonation->setEnabled(true);
    ui->actionCalculatePoints->setEnabled(true);
    ui->actionDrawPoints->setEnabled(true);
    ui->actionDrawBlocks->setEnabled(true);
    ui->actionDrawGrid->setEnabled(true);
    //ui->actionGlobalSettings->setEnabled(true);
    ui->actionProjectSettings->setEnabled(true);
}

// Remove points according to the list of indexes
void TT_DialogMain::removePoints(QList<int> &indexesToRemove)
{
    // Sort the list (ex: 1 2 5 8 9 ...)
    std::sort(indexesToRemove.begin(), indexesToRemove.end());

    // Remove indexes from the end to the begin
    // in order to keep indexes correct
    for (int i = indexesToRemove.size() - 1; i >= 0; i--)
    {
        points.remove(indexesToRemove[i]);
    }

    displayPoints();
}

// Edit a single point attributes
void TT_DialogMain::editPoint(TT::Point *point)
{
    TT_DialogEdit editDialog(this, point);
    if (editDialog.exec() == QDialog::Accepted)
    {
        displayPoints();
    }
}

// Move points[index] one position up in points QList
void TT_DialogMain::movePointUp(int index)
{
    points.move(index, index - 1);

    displayPoints();

    ui->tableWidget->setCurrentCell(index - 1, 0);
}

// Move points[index] one position down in points QList
void TT_DialogMain::movePointDown(int index)
{
    points.move(index, index + 1);

    displayPoints();

    ui->tableWidget->setCurrentCell(index + 1, 0);
}

// Draw points on the current drawing
int TT_DialogMain::drawPoints()
{
    int nbPoints = 0;

    // Check if layers already exist
    QStringList layers = this->doc->getAllLayer();

    bool hasTtPointsLayer = layers.contains(TT::LAYERS[TT::LAYER::POINTS].name);
    bool hasTtNameLayer = layers.contains(TT::LAYERS[TT::LAYER::NAME].name);
    bool hasTtAltiLayer = layers.contains(TT::LAYERS[TT::LAYER::ALTI].name);

    // If at least one of the following layers exists, do not draw points
    if (hasTtPointsLayer || hasTtNameLayer || hasTtAltiLayer)
    {
        return -1;
    }

    // Prepare layers
    QString currentLayer = this->doc->getCurrentLayer();

    this->doc->setLayer(TT::LAYERS[TT::LAYER::POINTS].name);
    this->doc->setCurrentLayerProperties(TT::LAYERS[TT::LAYER::POINTS].colour, TT::LAYERS[TT::LAYER::POINTS].lineWidth, TT::LAYERS[TT::LAYER::POINTS].lineType);

    this->doc->setLayer(TT::LAYERS[TT::LAYER::NAME].name);
    this->doc->setCurrentLayerProperties(TT::LAYERS[TT::LAYER::NAME].colour, TT::LAYERS[TT::LAYER::NAME].lineWidth, TT::LAYERS[TT::LAYER::NAME].lineType);

    this->doc->setLayer(TT::LAYERS[TT::LAYER::ALTI].name);
    this->doc->setCurrentLayerProperties(TT::LAYERS[TT::LAYER::ALTI].colour, TT::LAYERS[TT::LAYER::ALTI].lineWidth, TT::LAYERS[TT::LAYER::ALTI].lineType);

    // Draw each point
    for (auto i = 0; i < points.size(); i++)
    {
        TT::Point *currentPoint = points.at(i);
        if (currentPoint->type == TT::Point::TYPE::POINT)
        {
            drawPoint(currentPoint);
            nbPoints++;
        }
    }

    this->doc->setLayer(currentLayer);

    return nbPoints;
}

// Draw a single point on the current drawing
void TT_DialogMain::drawPoint(TT::Point *point)
{
    this->doc->setLayer(TT::LAYERS[TT::LAYER::POINTS].name);
    QPointF insertionPoint(point->x, point->y);
    this->doc->addPoint(&insertionPoint);

    if (!point->name.isEmpty())
    {
        this->doc->setLayer(TT::LAYERS[TT::LAYER::NAME].name);
        QPointF textInsertionPoint(point->x + 1.0, point->y + 4.0);
        this->doc->addText(point->name, "standard", &textInsertionPoint, 12.0, 0.0, DPI::HAlignLeft, DPI::VAlignTop);
    }

    if (point->hasZ)
    {
        this->doc->setLayer(TT::LAYERS[TT::LAYER::ALTI].name);
        QString text = QString("%1").arg(point->z);
        QPointF textInsertionPoint(point->x + 1.0, point->y - 12.0 - 4.0);
        this->doc->addText(text, "standard", &textInsertionPoint, 12.0, 0.0, DPI::HAlignLeft, DPI::VAlignTop);
    }
}

void TT_DialogMain::actionNew()
{
    QString fileNameLocal = QFileDialog::getSaveFileName(this, tr("Create a TT file"), "", tr("TT files (*.tt)"));
    if (fileNameLocal.isEmpty())
    {
        return;
    }

    fileName = fileNameLocal;

    IO::PluginSettings ps = { &fileName };
    IO::writePluginSettings(&ps);

    ui->label->setText(tr("Active file : %1").arg(fileName));

    enableAllTools();

    points.clear();
    displayPoints();
}

void TT_DialogMain::actionOpen()
{
    QString fileNameLocal = QFileDialog::getOpenFileName(this, tr("Open a TT file"), "", tr("TT files (*.tt)"));
    if (fileNameLocal.isEmpty())
    {
        return;
    }

    QFile file(fileNameLocal);
    if (!file.exists())
    {
        return;
    }

    fileName = fileNameLocal;

    IO::PluginSettings ps = { &fileName };
    IO::writePluginSettings(&ps);

    loadTtFile();
}

void TT_DialogMain::actionSave()
{
    IO::TtFileData ttfd = { &fileName, nullptr, &points };
    int returnedValue = IO::writeTtFile(&ttfd);
    if (returnedValue == 0)
    {
        ui->label->setText(tr("Active file : %1 | %2 points saved.").arg(fileName).arg(points.size()));
    }
    else if (returnedValue == -1)
    {
        QMessageBox::critical(this, tr("Error!"), tr("Could not save file!"));
    }
}

void TT_DialogMain::actionImport()
{
    int nbPointsImported = -1;
    TT_DialogImport importDialog(this, points, nbPointsImported);
    if (importDialog.exec() == QDialog::Accepted && nbPointsImported > -1)
    {
        ui->label->setText(tr("Active file : %1 | %2 points imported.").arg(fileName).arg(nbPointsImported));
        displayPoints();
    }
}

void TT_DialogMain::actionAdd()
{
    TT::Point *newPoint = new TT::Point();
    TT_DialogAdd addDialog(this, newPoint);
    if (addDialog.exec() == QDialog::Accepted)
    {
        points.append(newPoint);
        displayPoints();
    }
}

void TT_DialogMain::actionRemove()
{
    // Only proceed if there are selected items
    if (!ui->tableWidget->selectedItems().isEmpty())
    {
        QList<int> indexesToRemove;

        // For each row
        for (int i = 0; i < ui->tableWidget->selectedItems().size(); i += ui->tableWidget->columnCount())
        {
            QTableWidgetItem *qtwi = ui->tableWidget->selectedItems().at(i);
            indexesToRemove.append(qtwi->row());
        }
        removePoints(indexesToRemove);
    }
}

void TT_DialogMain::actionEdit()
{
    if (ui->tableWidget->currentRow() >= 0 && ui->tableWidget->currentRow() < points.size())
    {
        editPoint(points.at(ui->tableWidget->currentRow()));
    }
}

void TT_DialogMain::actionUp()
{
    if (ui->tableWidget->currentRow() >= 1 && ui->tableWidget->currentRow() < points.size())
    {
        movePointUp(ui->tableWidget->currentRow());
    }
}

void TT_DialogMain::actionDown()
{
    if (ui->tableWidget->currentRow() >= 0 && ui->tableWidget->currentRow() < points.size() - 1)
    {
        movePointDown(ui->tableWidget->currentRow());
    }
}

void TT_DialogMain::actionCalculateV0()
{
    TT_DialogV0 v0Dialog(this, points);
    if (v0Dialog.exec() == QDialog::Accepted)
    {
        displayPoints();
    }
}

void TT_DialogMain::actionCalculatePolygonation()
{
    TT_DialogPolygo polygoDialog(this, points);
    if (polygoDialog.exec() == QDialog::Accepted)
    {
        displayPoints();
    }
}

void TT_DialogMain::actionCalculatePoints()
{
    TT_DialogPoints pointsDialog(this, points);
    if (pointsDialog.exec() == QDialog::Accepted)
    {
        displayPoints();
    }
}

void TT_DialogMain::actionDrawPoints()
{
    int nbPointsDrawn = TT_DialogMain::drawPoints();
    if (nbPointsDrawn > -1)
    {
        ui->label->setText(tr("Active file : %1 | %2 points drawn.").arg(fileName).arg(nbPointsDrawn));
    }
    else
    {
        ui->label->setText(tr("Active file : %1 | No points drawn.").arg(fileName));
    }
}

void TT_DialogMain::actionDrawBlocks()
{
    TT_DialogDrawBlocks drawBlocksDialog(this, doc);
    drawBlocksDialog.exec();
}

void TT_DialogMain::actionDrawGrid()
{
    TT_DialogGrid gridDialog(this, doc);
    gridDialog.exec();
}

void TT_DialogMain::actionGlobalSettings()
{
    TT_DialogGlobalSettings globalSettingsDialog(this);
    globalSettingsDialog.exec();
}

void TT_DialogMain::actionProjectSettings()
{
    TT_DialogProjectSettings projectSettingsDialog(this);
    projectSettingsDialog.exec();
}

void TT_DialogMain::tableWidgetCellDoubleClicked(int row, int column)
{
    Q_UNUSED(column);

    editPoint(points.at(row));
}
