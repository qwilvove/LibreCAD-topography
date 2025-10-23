#include "tt_dialogmain.h"
#include "ui_tt_dialogmain.h"

#include "document_interface.h"
#include "topographytools.h"
#include "tt_dialogedit.h"
#include "tt_dialogimport.h"
#include "tt_dialogadd.h"
#include "tt_dialogv0.h"
#include "tt_dialogpolygo.h"
#include "tt_dialogpoints.h"
#include "tt_dialogdrawblocks.h"
#include "tt_dialoggrid.h"

#include <QFileDialog>
#include <QMenuBar>
#include <QMessageBox>
#include <QSettings>
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

    readSettings();

    if ( loadPoints() > -1 )
    {
        displayPoints();
        enableAllTools();
    }
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
    QToolBar* tb = new QToolBar();
    QMenuBar* mb = new QMenuBar();
    QMenu *menu = new QMenu("Menu");

    for (auto it = ACTION_GROUPS.begin(); it != ACTION_GROUPS.end(); ++it) {
        foreach (ACTION action_id, it.value())
        {
            Action action = ACTIONS[action_id];
            QAction *a = new QAction(action.name);
            a->setEnabled(false);
            if (action_id == ACTION::NEW or action_id == ACTION::OPEN)
            {
                a->setEnabled(true);
            }
            a->setToolTip(action.tooltip);
            a->setIcon(QIcon(QString(":/icons/%1.svg").arg(action.iconName)));
            a->setShortcut(QKeySequence(action.shortcut));
            connect(a, &QAction::triggered, this, [this,action_id]{executeAction(action_id);});
            menu->addAction(a);
            tb->addAction(a);
        }

        if ( it.key() != ACTION_GROUP::DRAW )
        {
            menu->addSeparator();
            tb->addSeparator();
        }
    }

    mb->addMenu(menu);

    ui->vlToolBar->addWidget(mb);
    ui->vlToolBar->addWidget(tb);
}

// Read setting to find current .tt file
void TT_DialogMain::readSettings()
 {
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, "LibreCAD", "topographytools");
    fileName = settings.value("lastfilename", "").toString();
 }

// Save .tt file as current working file used
void TT_DialogMain::writeSettings()
{
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, "LibreCAD", "topographytools");
    settings.setValue("lastfilename", fileName);
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

// Fill points list reading .tt file
// Data is stored as follows :
//   - Number of points
//   - Point 1 data
//   - Point 2 data
//   - ...
int TT_DialogMain::loadPoints()
{
    int nbPoints = 0;

    points.clear();

    if (fileName.isEmpty())
    {
        return -1;
    }

    QFile file(fileName);
    if (!file.exists())
    {
        ui->label->setText(tr("Active file : none | %1 does not exist!").arg(fileName));
        return -1;
    }
    if (!file.open(QIODevice::ReadOnly))
    {
        QMessageBox::critical(this, tr("Error!"), tr("Could not open file!"));
        return -1;
    }

    QDataStream stream(&file);
    int max;
    stream >> max;

    for (int i = 0; i < max; i++)
    {
        TT::Point *point = new TT::Point();
        loadPoint(stream, point);
        points.append(point);
        nbPoints++;
    }

    file.close();

    ui->label->setText(tr("Active file : %1").arg(fileName));

    return nbPoints;
}

// Read each information for a single point
void TT_DialogMain::loadPoint(QDataStream &stream, TT::Point *point)
{
    stream >> point->type;
    stream >> point->name;
    stream >> point->x;
    stream >> point->y;
    stream >> point->hasZ;
    stream >> point->z;
    stream >> point->ih;
    stream >> point->v0;
    stream >> point->ph;
    stream >> point->ha;
    stream >> point->va;
    stream >> point->id;
}

// Save all points in the .tt file following the file structure
// cf loadPoints for file structure
int TT_DialogMain::savePoints()
{
    int nbPoints = 0;

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly))
    {
        QMessageBox::critical(this, tr("Error!"), tr("Could not save file!"));
        return -1;
    }

    QDataStream stream(&file);
    stream.setVersion(QDataStream::Qt_5_0);

    int len = points.size();
    stream << len;

    foreach (TT::Point *point, points)
    {
        savePoint(stream, point);
        nbPoints++;
    }

    file.close();

    return nbPoints;
}

// Write each information for a single point
void TT_DialogMain::savePoint(QDataStream &stream, TT::Point *point)
{
    stream << point->type;
    stream << point->name;
    stream << point->x;
    stream << point->y;
    stream << point->hasZ;
    stream << point->z;
    stream << point->ih;
    stream << point->v0;
    stream << point->ph;
    stream << point->ha;
    stream << point->va;
    stream << point->id;
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
    foreach (QAction *action, ui->vlToolBar->itemAt(1)->widget()->actions())
    {
        action->setEnabled(true);
    }
}

// Import points from file and add them to the project
void TT_DialogMain::importPoints()
{
    int nbPointsImported = -1;
    TT_DialogImport importDialog(this, points, nbPointsImported);
    if (importDialog.exec() == QDialog::Accepted && nbPointsImported > -1)
    {
        ui->label->setText(tr("Active file : %1 | %2 points imported.").arg(fileName).arg(nbPointsImported));
        displayPoints();
    }
}

// Add a point to points
void TT_DialogMain::addPoint()
{
    TT::Point *newPoint = new TT::Point();
    TT_DialogAdd addDialog(this, newPoint);
    if (addDialog.exec() == QDialog::Accepted)
    {
        points.append(newPoint);
        displayPoints();
    }
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
        points.removeAt(indexesToRemove[i]);
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
        if (currentPoint->type == TT::PTYPE::POINT)
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

void TT_DialogMain::executeAction(ACTION action)
{
    switch (action) {
    case ACTION::NEW:
        actionNew();
        break;
    case ACTION::OPEN:
        actionOpen();
        break;
    case ACTION::SAVE:
        actionSave();
        break;
    case ACTION::IMPORT:
        actionImport();
        break;
    case ACTION::ADD:
        actionAdd();
        break;
    case ACTION::REMOVE:
        actionRemove();
        break;
    case ACTION::EDIT:
        actionEdit();
        break;
    case ACTION::UP:
        actionUp();
        break;
    case ACTION::DOWN:
        actionDown();
        break;
    case ACTION::CALC_V0:
        actionV0();
        break;
    case ACTION::CALC_POLYGO:
        actionPolygo();
        break;
    case ACTION::CALC_POINTS:
        actionPoints();
        break;
    case ACTION::DRAW_POINTS:
        actionDraw();
        break;
    case ACTION::DRAW_BLOCKS:
        actionDrawBlocks();
        break;
    case ACTION::DRAW_GRID:
        actionGrid();
        break;
    default:
        break;
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

    writeSettings();

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

    writeSettings();

    if ( loadPoints() > -1 )
    {
        displayPoints();
        enableAllTools();
    }
}

void TT_DialogMain::actionSave()
{
    int nbPointsSaved = TT_DialogMain::savePoints();
    if (nbPointsSaved > -1)
    {
        ui->label->setText(tr("Active file : %1 | %2 points saved.").arg(fileName).arg(nbPointsSaved));
    }
}

void TT_DialogMain::actionImport()
{
    importPoints();
}

void TT_DialogMain::actionAdd()
{
    addPoint();
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

void TT_DialogMain::actionV0()
{
    TT_DialogV0 v0Dialog(this, points);
    if (v0Dialog.exec() == QDialog::Accepted)
    {
        displayPoints();
    }
}

void TT_DialogMain::actionPolygo()
{
    TT_DialogPolygo polygoDialog(this, points);
    if (polygoDialog.exec() == QDialog::Accepted)
    {
        displayPoints();
    }
}

void TT_DialogMain::actionPoints()
{
    TT_DialogPoints pointsDialog(this, points);
    if (pointsDialog.exec() == QDialog::Accepted)
    {
        displayPoints();
    }
}

void TT_DialogMain::actionDraw()
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

void TT_DialogMain::actionGrid()
{
    TT_DialogGrid gridDialog(this, doc);
    gridDialog.exec();
}

void TT_DialogMain::on_tableWidget_cellDoubleClicked(int row, int column)
{
    Q_UNUSED(column);

    editPoint(points.at(row));
}
