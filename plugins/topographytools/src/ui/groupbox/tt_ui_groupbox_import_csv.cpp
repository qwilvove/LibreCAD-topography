#include "tt_ui_groupbox_import_csv.h"
#include "ui_tt_ui_groupbox_import_csv.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>

TT_GroupBoxImportCsv::TT_GroupBoxImportCsv(QWidget *parent):
    QGroupBox(parent),
    ui(new Ui::TT_GroupBoxImportCsv)
{
    ui->setupUi(this);

    newPoints = new QList<TT::Point*>();

    twis = new QList<QTableWidgetItem*>();
}

TT_GroupBoxImportCsv::~TT_GroupBoxImportCsv()
{
    for (QTableWidgetItem *twi : *twis)
    {
        delete twi;
    }
    delete twis;
    delete newPoints;
    delete ui;
}

QList<TT::Point*> *TT_GroupBoxImportCsv::getNewPoints()
{
    return newPoints;
}

void TT_GroupBoxImportCsv::loadFilePreview(QTextStream &stream)
{
    stream.seek(0);
    ui->lw_preview->clear();

    while (!stream.atEnd())
    {
        QString line = stream.readLine();

        ui->lw_preview->addItem(line);
    }
}

void TT_GroupBoxImportCsv::loadPointsPreview(QTextStream &stream)
{
    stream.seek(0);
    ui->tw_points->clearContents();
    ui->tw_points->setRowCount(0);

    while (!stream.atEnd())
    {
        QString line = stream.readLine();
        loadPointPreview(line);
    }

    ui->lb_nbPoints->setText(tr("Those %1 points will be added").arg(newPoints->size()));
}

bool TT_GroupBoxImportCsv::loadPointPreview(QString &line)
{
    bool ok = true;

    double x, y, z = 0;
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
    if (!ok)
    {
        return false;
    }
    y = splitedLine.at(1).trimmed().toDouble(&ok);
    if (!ok)
    {
        return false;
    }
    if (hasZ)
    {
        z = splitedLine.at(2).trimmed().toDouble(&ok);
        if (!ok)
        {
            return false;
        }
    }

    // Create point (Only import points for now)
    TT::Point *point = new TT::Point();

    point->setType(TT::Point::TYPE::POINT);
    point->setName("?");
    point->setX(x);
    point->setY(y);
    point->setHasZ(hasZ);
    if (point->getHasZ())
    {
        point->setZ(z);
    }

    int newIndex = ui->tw_points->rowCount();
    ui->tw_points->setRowCount(newIndex + 1);

    QTableWidgetItem *twiType = point->getTwiType();
    twis->append(twiType);
    QTableWidgetItem *twiName = point->getTwiName();
    twis->append(twiName);
    QTableWidgetItem *twiParameters = point->getTwiParameters();
    twis->append(twiParameters);

    ui->tw_points->setItem(newIndex, 0, twiType);
    ui->tw_points->setItem(newIndex, 1, twiName);
    ui->tw_points->setItem(newIndex, 2, twiParameters);

    newPoints->append(point);

    return true;
}

void TT_GroupBoxImportCsv::slot_tbOpenPressed()
{
    QString fileNameLocal = QFileDialog::getOpenFileName(this, tr("Select a CSV file"), "", tr("CSV (*.csv)"));
    if (fileNameLocal.isEmpty())
    {
        return;
    }

    QFile file(fileNameLocal);
    if (!file.exists())
    {
        QMessageBox::warning(this, tr("Error!"), tr("File not found!"));
        return;
    }
    if (!file.open(QIODevice::ReadOnly))
    {
        QMessageBox::critical(this, tr("Error!"), tr("Could not open file!"));
        return;
    }

    ui->le_file->setText(fileNameLocal);

    for (QTableWidgetItem *twi : *twis)
    {
        delete twi;
    }
    twis->clear();
    for (TT::Point *p : *newPoints)
    {
        delete p;
    }
    newPoints->clear();

    QTextStream stream(&file);
    loadFilePreview(stream);
    loadPointsPreview(stream);

    file.close();
}
