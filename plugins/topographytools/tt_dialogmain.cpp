#include "tt_dialogmain.h"
#include "ui_tt_dialogmain.h"

#include "document_interface.h"
#include "topographytools.h"

#include <QFileDialog>
#include <QSettings>
#include <QMessageBox>

// Constructor
TT_DialogMain::TT_DialogMain(QWidget *parent, Document_Interface *doc) :
    QDialog(parent),
    ui(new Ui::TT_DialogMain),
    doc(doc)
{
    ui->setupUi(this);

    readSettings();

    if (!fileName.isEmpty())
    {
        ui->label->setText(QString("Active file : %1").arg(fileName));

        ui->pbSave->setEnabled(true);
        ui->pbImport->setEnabled(true);
        ui->pbDraw->setEnabled(true);

        loadPoints();
        displayPoints();
    }
}

// Destructor
TT_DialogMain::~TT_DialogMain()
{
    delete ui;
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

    QFile file(fileName);
    if (!file.exists())
    {
        QMessageBox::critical(this, tr("Error!"), tr("File does not exist!"));
        return -1;
    }
    if (!file.open(QIODevice::ReadOnly))
    {
        QMessageBox::critical(this, tr("Error!"), tr("Could not open file!"));
        return -1;
    }

    QDataStream stream(&file);
    if (stream.version() != QDataStream::Qt_6_0)
    {
        QMessageBox::critical(this, tr("Error!"), tr("Wrong data stream version!"));
        file.close();
        return -1;
    }

    int max;
    stream >> max;

    for (int i = 0; i < max; i++)
    {
        TT::Point point{};
        loadPoint(stream, point);
        points.append(point);
        nbPoints++;
    }

    file.close();

    return nbPoints;
}

// Read each information for a single point
void TT_DialogMain::loadPoint(QDataStream &stream, TT::Point &point)
{
    stream >> point.type;
    stream >> point.name;
    stream >> point.x;
    stream >> point.y;
    stream >> point.hasZ;
    stream >> point.z;
    stream >> point.ih;
    stream >> point.v0;
    stream >> point.ha;
    stream >> point.va;
    stream >> point.id;
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
    stream.setVersion(QDataStream::Qt_6_0);

    int len = points.size();
    stream << len;

    foreach (TT::Point point, points)
    {
        savePoint(stream, point);
        nbPoints++;
    }

    file.close();

    return nbPoints;
}

// Write each information for a single point
void TT_DialogMain::savePoint(QDataStream &stream, TT::Point &point)
{
    stream << point.type;
    stream << point.name;
    stream << point.x;
    stream << point.y;
    stream << point.hasZ;
    stream << point.z;
    stream << point.ih;
    stream << point.v0;
    stream << point.ha;
    stream << point.va;
    stream << point.id;
}

// Read a CSV file to add points to the project
int TT_DialogMain::importPoints()
{
    int nbPoints = 0;

    QString fileNameLocal = QFileDialog::getOpenFileName(this, tr("Select a field notebook"), "", tr("CSV (*.csv)"));
    if (fileNameLocal.isEmpty())
    {
        return -1;
    }

    QFile file(fileNameLocal);
    if (!file.exists())
    {
        QMessageBox::warning(this, tr("Error!"), tr("File not found!"));
        return -1;
    }
    if (!file.open(QIODevice::ReadOnly))
    {
        QMessageBox::critical(this, tr("Error!"), tr("Could not open file!"));
        return -1;
    }

    QTextStream stream(&file);
    while (!stream.atEnd())
    {
        QString line = stream.readLine();
        TT::Point point{};
        if(importPoint(line, point))
        {
            points.append(point);
            nbPoints++;
        }
    }

    file.close();

    return nbPoints;
}

// Read point info on a single CSV line
// Accepted formats :
// xxx.xx;y.yyyy
// OR
// xx.xxxx;y.y;zz.zzz
bool TT_DialogMain::importPoint(QString &line, TT::Point &point)
{
    bool ok = true;

    QStringList splitedLine =  line.split(";");
    if (splitedLine.size() < 2)
    {
        return false;
    }

    if (splitedLine.size() == 2)
    {
        double x = splitedLine.at(0).toDouble(&ok);
        if (!ok) return false;
        double y = splitedLine.at(1).toDouble(&ok);
        if (!ok) return false;

        point.x = x;
        point.y = y;

        return true;
    }
    if (splitedLine.size() == 3)
    {
        double x = splitedLine.at(0).toDouble(&ok);
        if (!ok) return false;
        double y = splitedLine.at(1).toDouble(&ok);
        if (!ok) return false;
        double z = splitedLine.at(2).toDouble(&ok);
        if (!ok) return false;

        point.x = x;
        point.y = y;
        point.hasZ = true;
        point.z = z;

        return true;
    }

    return false;
}

// Update tableWidget to show point list to the user
void TT_DialogMain::displayPoints()
{
    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(0);

    foreach (TT::Point point, points)
    {
        displayPoint(point);
    }
}

// Add a line to tableWidget and fill it with point info
void TT_DialogMain::displayPoint(TT::Point &point)
{
    int lineNumber = ui->tableWidget->rowCount() + 1;
    ui->tableWidget->setRowCount(lineNumber);

    // Fill the row
    // Line number
    QString infoLineNumber = QString("%1").arg(lineNumber);
    QTableWidgetItem *item = new QTableWidgetItem(infoLineNumber);
    ui->tableWidget->setItem(lineNumber - 1, 0, item);

    // Type
    QString infoType;
    if (point.type == TT::PTYPE::POINT)
    {
        infoType = tr("POINT");
    }
    else if (point.type == TT::PTYPE::STATION)
    {
        infoType = tr("STATION");
    }
    else if (point.type == TT::PTYPE::REFERENCE)
    {
        infoType = tr("REFERENCE");
    }
    else if (point.type == TT::PTYPE::MEASURE)
    {
        infoType = tr("MEASURE");
    }
    QTableWidgetItem *itemType = new QTableWidgetItem(infoType);
    ui->tableWidget->setItem(lineNumber - 1, 1, itemType);

    // Name
    QString infoName = point.name;
    QTableWidgetItem *itemName = new QTableWidgetItem(infoName);
    ui->tableWidget->setItem(lineNumber - 1, 2, itemName);

    // Parameters
    QString infoParameters;
    if (point.type == TT::PTYPE::POINT)
    {
        if (point.hasZ)
        {
            infoParameters = QString("X=%1 , Y=%2 , Z=%3").arg(point.x).arg(point.y).arg(point.z);
        }
        else
        {
            infoParameters = QString("X=%1 , Y=%2").arg(point.x).arg(point.y);
        }
    }
    QTableWidgetItem *itemParameters = new QTableWidgetItem(infoParameters);
    ui->tableWidget->setItem(lineNumber - 1, 3, itemParameters);
}

// Draw points on the current drawing
int TT_DialogMain::drawPoints()
{
    int nbPoints = 0;

    // Prepare layers
    QString currentLayer = this->doc->getCurrentLayer();

    int colour;
    DPI::LineWidth lineWidth;
    DPI::LineType lineType;

    this->doc->setLayer("TT_POINTS");
    this->doc->getCurrentLayerProperties(&colour, &lineWidth, &lineType);
    colour = 0;
    this->doc->setCurrentLayerProperties(colour, lineWidth, lineType);

    this->doc->setLayer("TT_NAME");
    this->doc->getCurrentLayerProperties(&colour, &lineWidth, &lineType);
    colour = 0x00FFFF;
    this->doc->setCurrentLayerProperties(colour, lineWidth, lineType);

    this->doc->setLayer("TT_ALTI");
    this->doc->getCurrentLayerProperties(&colour, &lineWidth, &lineType);
    colour = 0x00FF00;
    this->doc->setCurrentLayerProperties(colour, lineWidth, lineType);

    // Draw each point
    for (auto i = 0; i < points.size(); i++)
    {
        TT::Point currentPoint = points.at(i);
        drawPoint(currentPoint);
        nbPoints++;
    }

    this->doc->setLayer(currentLayer);

    return nbPoints;
}

// Draw a single point on the current drawing
void TT_DialogMain::drawPoint(TT::Point &point)
{
    this->doc->setLayer("TT_POINTS");
    QPointF insertionPoint(point.x, point.y);
    this->doc->addPoint(&insertionPoint);

    if (!point.name.isEmpty())
    {
        this->doc->setLayer("TT_NAME");
        QPointF textInsertionPoint(point.x + 1.0, point.y + 4.0);
        this->doc->addText(point.name, "standard", &textInsertionPoint, 12.0, 0.0, DPI::HAlignLeft, DPI::VAlignTop);
    }

    if (point.hasZ)
    {
        this->doc->setLayer("TT_ALTI");
        QString text = QString("%1").arg(point.z);
        QPointF textInsertionPoint(point.x + 1.0, point.y - 12.0 - 4.0);
        this->doc->addText(text, "standard", &textInsertionPoint, 12.0, 0.0, DPI::HAlignLeft, DPI::VAlignTop);
    }
}

void TT_DialogMain::on_pbNew_clicked()
{
    QString fileNameLocal = QFileDialog::getSaveFileName(this, tr("Create a TT file"), "", tr("TT files (*.tt)"));
    if (fileNameLocal.isEmpty())
    {
        return;
    }

    fileName = fileNameLocal;

    writeSettings();

    ui->label->setText(QString("Active file : %1").arg(fileName));

    ui->pbSave->setEnabled(true);
    ui->pbImport->setEnabled(true);
    ui->pbDraw->setEnabled(true);

    points.clear();
    displayPoints();
}

void TT_DialogMain::on_pbOpen_clicked()
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

    ui->label->setText(QString("Active file : %1").arg(fileName));

    ui->pbSave->setEnabled(true);
    ui->pbImport->setEnabled(true);
    ui->pbDraw->setEnabled(true);

    loadPoints();
    displayPoints();
}

void TT_DialogMain::on_pbSave_clicked()
{
    int nbPointsSaved = TT_DialogMain::savePoints();
    if (nbPointsSaved > -1)
    {
        ui->label->setText(QString("Active file : %1 | %2 points saved.").arg(fileName).arg(nbPointsSaved));
    }
}

void TT_DialogMain::on_pbImport_clicked()
{
    int nbPointsImported = TT_DialogMain::importPoints();
    if (nbPointsImported > -1)
    {
        ui->label->setText(QString("Active file : %1 | %2 points imported.").arg(fileName).arg(nbPointsImported));
        displayPoints();
    }
}

void TT_DialogMain::on_pbDraw_clicked()
{
    int nbPointsDrawn = TT_DialogMain::drawPoints();
    if (nbPointsDrawn > -1)
    {
        ui->label->setText(QString("Active file : %1 | %2 points drawn.").arg(fileName).arg(nbPointsDrawn));
    }
}

/*QPointF point;
point.setX(ui->leX->text().toDouble());
point.setY(ui->leY->text().toDouble());

// Prepare layers
QString currentLayer = this->doc->getCurrentLayer();
int colour;
DPI::LineWidth lineWidth;
DPI::LineType lineType;

this->doc->setLayer("TT_ALTI");
this->doc->getCurrentLayerProperties(&colour, &lineWidth, &lineType);
colour = 0x00FFFF;
this->doc->setCurrentLayerProperties(colour, lineWidth, lineType);

this->doc->setLayer("TT_POINTS");
this->doc->getCurrentLayerProperties(&colour, &lineWidth, &lineType);
colour = 0x00FF00;
this->doc->setCurrentLayerProperties(colour, lineWidth, lineType);

QList<Plug_Entity *> selectedEntities;
QHash<int, QVariant> dataOfAnEntity;

// Ask user a selection
//this->doc->getSelect(&selectedEntities, tr("Select an entity"));
this->doc->setLayer(currentLayer);

for (auto i = 0; i < selectedEntities.size(); i++)
{
    selectedEntities.value(i)->getData(&dataOfAnEntity);

    // Check if the selectioned entity is a point or not
    if (dataOfAnEntity.value(DPI::ETYPE) == DPI::POINT)
    {
        // If so, get its coordinates...
        double startX = dataOfAnEntity.value(DPI::STARTX).toDouble();
        double startY = dataOfAnEntity.value(DPI::STARTY).toDouble();

        // ...and add the z coordinate as a text next to it
        this->doc->setLayer("TT_POINTS");
        QPointF textInsertionPoint(startX + 1.0, startY + 4.0);
        this->doc->addText("123.456", "standard", &textInsertionPoint, 12.0, 0.0, DPI::HAlignLeft, DPI::VAlignTop);

        this->doc->setLayer("TT_ALTI");
        textInsertionPoint = QPointF(startX + 1.0, startY - 12.0 - 4.0);
        this->doc->addText("78", "standard", &textInsertionPoint, 12.0, 0.0, DPI::HAlignLeft, DPI::VAlignTop);
    }
}

this->doc->setLayer("TT_POINTS");
this->doc->addBlockfromFromdisk("tree1.dxf");
this->doc->addPoint(&point);*/
