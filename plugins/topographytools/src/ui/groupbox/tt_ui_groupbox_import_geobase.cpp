#include "tt_ui_groupbox_import_geobase.h"
#include "ui_tt_ui_groupbox_import_geobase.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>

TT_GroupBoxImportGeobase::TT_GroupBoxImportGeobase(QWidget *parent):
    QGroupBox(parent),
    ui(new Ui::TT_GroupBoxImportGeobase)
{
    ui->setupUi(this);

    newPoints = new QList<TT::Point*>();

    twis = new QList<QTableWidgetItem*>();
}

TT_GroupBoxImportGeobase::~TT_GroupBoxImportGeobase()
{
    for (QTableWidgetItem *twi : *twis)
    {
        delete twi;
    }
    delete twis;
    delete newPoints;
    delete ui;
}

QList<TT::Point*> *TT_GroupBoxImportGeobase::getNewPoints()
{
    return newPoints;
}

void TT_GroupBoxImportGeobase::loadFilePreview(QTextStream &stream)
{
    stream.seek(0);
    ui->lw_preview->clear();

    while (!stream.atEnd())
    {
        QString line = stream.readLine();

        ui->lw_preview->addItem(line);
    }
}

void TT_GroupBoxImportGeobase::loadPointsPreview(QTextStream &stream)
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

bool TT_GroupBoxImportGeobase::loadPointPreview(QString &line)
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
        point->setType(TT::Point::TYPE::POINT);
        point->setName(subStringName);
        point->setX(line.mid(44, 14).trimmed().toDouble(&ok));
        if (!ok)
        {
            delete point;
            return false;
        }
        point->setY(line.mid(59, 14).trimmed().toDouble(&ok));
        if (!ok)
        {
            delete point;
            return false;
        }
        point->setZ(line.mid(74, 14).trimmed().toDouble(&ok));
        if (!ok)
        {
            delete point;
            return false;
        }
        point->setHasZ(true);
        if (point->getZ() == -999999)
        {
            point->setHasZ(false);
        }
    }
    else if (subStringType == "Station")
    {
        point->setType(TT::Point::TYPE::STATION);
        point->setName(subStringName);
        point->setIh(line.mid(43,  8).trimmed().toDouble(&ok));
        if (!ok)
        {
            delete point;
            return false;
        }
        point->setV0(line.mid(52, 11).trimmed().toDouble(&ok));
        if (!ok)
        {
            delete point;
            return false;
        }
    }
    else if (subStringType == "Reference")
    {
        point->setType(TT::Point::TYPE::REFERENCE);
        point->setName(subStringName);
        point->setPh(line.mid(43,  8).trimmed().toDouble(&ok));
        if (!ok)
        {
            delete point;
            return false;
        }
        point->setHa(line.mid(52, 11).trimmed().toDouble(&ok));
        if (!ok)
        {
            delete point;
            return false;
        }
        point->setVa(line.mid(64, 11).trimmed().toDouble(&ok));
        if (!ok)
        {
            delete point;
            return false;
        }
        point->setId(line.mid(76, 10).trimmed().toDouble(&ok));
        if (!ok)
        {
            delete point;
            return false;
        }
    }
    else if (subStringType == "Mesure")
    {
        point->setType(TT::Point::TYPE::MEASURE);
        point->setName(subStringName);
        point->setPh(line.mid(43,  8).trimmed().toDouble(&ok));
        if (!ok)
        {
            delete point;
            return false;
        }
        point->setHa(line.mid(52, 11).trimmed().toDouble(&ok));
        if (!ok)
        {
            delete point;
            return false;
        }
        point->setVa(line.mid(64, 11).trimmed().toDouble(&ok));
        if (!ok)
        {
            delete point;
            return false;
        }
        point->setId(line.mid(76, 10).trimmed().toDouble(&ok));
        if (!ok)
        {
            delete point;
            return false;
        }
    }
    else
    {
        delete point;
        return false;
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

void TT_GroupBoxImportGeobase::slot_tbOpenPressed()
{
    QString fileNameLocal = QFileDialog::getOpenFileName(this, tr("Select a Geobase file"), "", tr("Geobase (*.geo)"));
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
