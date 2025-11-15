#include "tt_dialogimport.h"
#include "ui_tt_dialogimport.h"

#include <QFileDialog>
#include <QMessageBox>

TT_DialogImport::TT_DialogImport(QWidget *parent, QList<TT::Point *> &points, int &nbPointsImported) :
    QDialog(parent),
    ui(new Ui::TT_DialogImport),
    points(points),
    nbPointsImported(nbPointsImported)
{
    ui->setupUi(this);
    ui->gbGeobase->hide();
    this->resize(this->geometry().width(), 600);
}

TT_DialogImport::~TT_DialogImport()
{
    delete ui;
}

void TT_DialogImport::loadCsvFilePreview(QTextStream &stream)
{
    ui->lwPreviewCsv->clear();

    while (!stream.atEnd())
    {
        QString line = stream.readLine();

        ui->lwPreviewCsv->addItem(line);
    }

    stream.seek(0);
}

void TT_DialogImport::loadCsvPointsPreview(QTextStream &stream)
{
    ui->twPointsCsv->clearContents();
    ui->twPointsCsv->setRowCount(0);

    newPointsFromCsv.clear();

    while (!stream.atEnd())
    {
        QString line = stream.readLine();
        loadCsvPointPreview(line);
    }

    ui->lbNbPointsCsv->setText(tr("Those %1 points will be added").arg(newPointsFromCsv.size()));

    if (!newPointsFromCsv.isEmpty())
    {
        ui->buttonBox->setEnabled(true);
    }

    stream.seek(0);
}

bool TT_DialogImport::loadCsvPointPreview(QString &line)
{
    bool ok = true;

    double x, y, z;
    bool hasZ = false;

    QStringList splitedLine =  line.split(";");
    if (splitedLine.size() < 2 or splitedLine.size() > 3)
    {
        return false;
    }

    if (splitedLine.size() == 3)
    {
        hasZ = true;
    }
    x = splitedLine.at(0).trimmed().toDouble(&ok);
    if (!ok) { return false; }
    y = splitedLine.at(1).trimmed().toDouble(&ok);
    if (!ok) { return false; }
    if (hasZ)
    {
        z = splitedLine.at(2).trimmed().toDouble(&ok);
        if (!ok) { return false; }
    }

    // Create point (Only import points for now)
    TT::Point *point = new TT::Point();

    point->type = TT::Point::TYPE::POINT;
    point->name = "?";
    point->x = x;
    point->y = y;
    point->hasZ = hasZ;
    if (point->hasZ)
    {
        point->z = z;
    }

    int lineNumber = ui->twPointsCsv->rowCount() + 1;
    ui->twPointsCsv->setRowCount(lineNumber);

    ui->twPointsCsv->setItem(lineNumber - 1, 0, point->getQTableWidgetItemType());
    ui->twPointsCsv->setItem(lineNumber - 1, 1, point->getQTableWidgetItemName());
    ui->twPointsCsv->setItem(lineNumber - 1, 2, point->getQTableWidgetItemParameters());

    newPointsFromCsv.append(point);

    return true;
}

int TT_DialogImport::importPointsFromCsv()
{
    int nbPoints = 0;

    QString fileNameLocal = QFileDialog::getOpenFileName(this, tr("Select a CSV file"), "", tr("CSV (*.csv)"));
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

    ui->leCsv->setText(fileNameLocal);

    QTextStream stream(&file);
    loadCsvFilePreview(stream);
    loadCsvPointsPreview(stream);

    file.close();

    return nbPoints;
}

void TT_DialogImport::loadGeobaseFilePreview(QTextStream &stream)
{
    ui->lwPreviewGeobase->clear();

    while (!stream.atEnd())
    {
        QString line = stream.readLine();

        ui->lwPreviewGeobase->addItem(line);
    }

    stream.seek(0);
}

void TT_DialogImport::loadGeobasePointsPreview(QTextStream &stream)
{
    ui->twPointsGeobase->clearContents();
    ui->twPointsGeobase->setRowCount(0);

    newPointsFromGeobase.clear();

    while (!stream.atEnd())
    {
        QString line = stream.readLine();
        loadGeobasePointPreview(line);
    }

    ui->lbNbPointsGeobase->setText(tr("Those %1 points will be added").arg(newPointsFromGeobase.size()));

    if (!newPointsFromGeobase.isEmpty())
    {
        ui->buttonBox->setEnabled(true);
    }

    stream.seek(0);
}

bool TT_DialogImport::loadGeobasePointPreview(QString &line)
{
    bool ok = true;

    // Create point
    TT::Point *point = new TT::Point();

    //QString subStringLineNumber = line.mid(0,   6).trimmed();
    QString subStringType       = line.mid(8,  13).trimmed();
    QString subStringName       = line.mid(25, 16).trimmed();

    if (subStringType.startsWith("Option"))
    {
        delete point;
        return false;
    }
    else if (subStringType == "Point")
    {
        point->type = TT::Point::TYPE::POINT;
        point->name = subStringName;
        point->x = line.mid(44, 14).trimmed().toDouble(&ok);
        if (!ok) { return false; }
        point->y = line.mid(59, 14).trimmed().toDouble(&ok);
        if (!ok) { return false; }
        point->z = line.mid(74, 14).trimmed().toDouble(&ok);
        if (!ok) { return false; }
        point->hasZ = true;
        if (point->z == -999999)
        {
            point->hasZ = false;
        }
    }
    else if (subStringType == "Station")
    {
        point->type = TT::Point::TYPE::STATION;
        point->name = subStringName;
        point->ih = line.mid(43,  8).trimmed().toDouble(&ok);
        if (!ok) { return false; }
        point->v0 = line.mid(52, 11).trimmed().toDouble(&ok);
        if (!ok) { return false; }
    }
    else if (subStringType == "Reference")
    {
        point->type = TT::Point::TYPE::REFERENCE;
        point->name = subStringName;
        point->ph = line.mid(43,  8).trimmed().toDouble(&ok);
        if (!ok) { return false; }
        point->ha = line.mid(52, 11).trimmed().toDouble(&ok);
        if (!ok) { return false; }
        point->va = line.mid(64, 11).trimmed().toDouble(&ok);
        if (!ok) { return false; }
        point->id = line.mid(76, 10).trimmed().toDouble(&ok);
        if (!ok) { return false; }
    }
    else if (subStringType == "Mesure")
    {
        point->type = TT::Point::TYPE::MEASURE;
        point->name = subStringName;
        point->ph = line.mid(43,  8).trimmed().toDouble(&ok);
        if (!ok) { return false; }
        point->ha = line.mid(52, 11).trimmed().toDouble(&ok);
        if (!ok) { return false; }
        point->va = line.mid(64, 11).trimmed().toDouble(&ok);
        if (!ok) { return false; }
        point->id = line.mid(76, 10).trimmed().toDouble(&ok);
        if (!ok) { return false; }
    }
    else
    {
        delete point;
        return false;
    }

    int lineNumber = ui->twPointsGeobase->rowCount() + 1;
    ui->twPointsGeobase->setRowCount(lineNumber);

    ui->twPointsGeobase->setItem(lineNumber - 1, 0, point->getQTableWidgetItemType());
    ui->twPointsGeobase->setItem(lineNumber - 1, 1, point->getQTableWidgetItemName());
    ui->twPointsGeobase->setItem(lineNumber - 1, 2, point->getQTableWidgetItemParameters());

    newPointsFromGeobase.append(point);

    return true;
}

int TT_DialogImport::importPointsFromGeobase()
{
    int nbPoints = 0;

    QString fileNameLocal = QFileDialog::getOpenFileName(this, tr("Select a Geobase file"), "", tr("Geobase (*.geo)"));
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

    ui->leGeobase->setText(fileNameLocal);

    QTextStream stream(&file);
    loadGeobaseFilePreview(stream);
    loadGeobasePointsPreview(stream);

    file.close();

    return nbPoints;
}

void TT_DialogImport::on_comboBox_currentIndexChanged(int index)
{
    ui->gbCsv->hide();
    ui->gbGeobase->hide();
    ui->buttonBox->setEnabled(false);

    switch (index) {
    case 0:
        ui->gbCsv->show();
        if (!newPointsFromCsv.isEmpty())
        {
            ui->buttonBox->setEnabled(true);
        }
        break;
    case 1:
        ui->gbGeobase->show();
        if (!newPointsFromGeobase.isEmpty())
        {
            ui->buttonBox->setEnabled(true);
        }
        break;
    default:
        break;
    }
}

void TT_DialogImport::on_tbOpenCsv_clicked()
{
    importPointsFromCsv();
}

void TT_DialogImport::on_tbOpenGeobase_clicked()
{
    importPointsFromGeobase();
}

void TT_DialogImport::on_buttonBox_accepted()
{
    switch (ui->comboBox->currentIndex()) {
    case 0:
        points.append(newPointsFromCsv);
        nbPointsImported = newPointsFromCsv.size();
        break;
    case 1:
        points.append(newPointsFromGeobase);
        nbPointsImported = newPointsFromGeobase.size();
        break;
    default:
        break;
    }
}
