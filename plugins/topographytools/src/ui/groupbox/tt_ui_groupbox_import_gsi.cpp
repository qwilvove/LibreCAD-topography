#include "tt_ui_groupbox_import_gsi.h"
#include "ui_tt_ui_groupbox_import_gsi.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>

TT_GroupBoxImportGsi::TT_GroupBoxImportGsi(QWidget *parent):
    QGroupBox(parent),
    ui(new Ui::TT_GroupBoxImportGsi)
{
    ui->setupUi(this);

    newPoints = new QList<TT::Point*>();
    twis = new QList<QTableWidgetItem*>();
}

TT_GroupBoxImportGsi::~TT_GroupBoxImportGsi()
{
    for (QTableWidgetItem *twi : *twis)
    {
        delete twi;
    }
    delete twis;
    delete newPoints;
    delete ui;
}

QList<TT::Point*> *TT_GroupBoxImportGsi::getNewPoints()
{
    return newPoints;
}

void TT_GroupBoxImportGsi::loadFilePreview(QTextStream &stream)
{
    stream.seek(0);
    ui->lw_preview->clear();

    QString line = "";

    while (!stream.atEnd())
    {
        line = stream.readLine().trimmed();
        if (!line.isEmpty())
        {
            ui->lw_preview->addItem(line);
        }
    }
}

void TT_GroupBoxImportGsi::loadPointsPreview(QTextStream &stream)
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

bool TT_GroupBoxImportGsi::loadPointPreview(QString line)
{
    bool ok = true;

    line = line.trimmed();
    if (line.startsWith("*"))
    {
        line.remove(0, 1);
    }

    QStringList words = line.trimmed().split(" ");

    // Create point
    TT::Point *point = new TT::Point();
    point->setType(TT::Point::TYPE::STATION);

    for (int i = 0; i < words.size(); i++)
    {
        QString word = words.at(i);

        QString w_index = word.mid(0, 2);
        int index = w_index.toInt(&ok);
        if (!ok)
        {
            delete point;
            return false;
        }

        int dataSize = 8; // Gsi 8
        if (words.at(0).size() == 23) // Gsi 16
        {
            dataSize = 16;
        }

        switch (index)
        {
        case 11: // Name
            point->setName(word.mid(7, dataSize).remove(QRegularExpression("^0+(?!$)")));
            break;
        case 21: // Ha
            point->setType(TT::Point::TYPE::MEASURE);
            point->setHa(convertToDouble(word.mid(5, dataSize+2)));
            break;
        case 22: // Va
            point->setType(TT::Point::TYPE::MEASURE);
            point->setVa(convertToDouble(word.mid(5, dataSize+2)));
            break;
        case 31: // Id
            point->setType(TT::Point::TYPE::MEASURE);
            point->setId(convertToDouble(word.mid(5, dataSize+2)));
            break;
        case 32: // Horizontal distance : ignored
            break;
        case 41: // Code
            break;
        case 51: // Prism constant
            break;
        case 71: //
            break;
        case 81: // X : ignored
            break;
        case 82: // Y : ignored
            break;
        case 83: // Z : ignored
            break;
        case 87: // Ph
            point->setType(TT::Point::TYPE::MEASURE);
            point->setPh(convertToDouble(word.mid(5, dataSize+2)));
            break;
        case 88: // Ih
            break;
        default:
            break;
        }
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

double TT_GroupBoxImportGsi::convertToDouble(QString str)
{
    double value = 0;
    bool ok = false;
    int precision = -1; // Number of decimal places

    // Units
    // 0 : 10^-3 m
    // 2 : 10^-5 gon
    // 3 : 10^-5 °
    // 4 : 10^-5 ° (base 60)
    // 6 : 10^-4 m
    // 8 : 10^-5 m
    QString unit = str.mid(0, 1);
    if (unit == "0") // 1/1000 precision
    {
        precision = 3;
    }
    else if (unit == "6") // 1/10000 precision
    {
        precision = 4;
    }
    else if (unit == "2" || unit == "3" || unit == "4" || unit == "8") // 1/100000 precision
    {
        precision = 5;
    }
    else
    {
        return 0;
    }

    QString strValue = str.mid(1, str.length() - 1); // Value without unit (e.g. 2+00032000 -> +00032000)
    QString left = strValue.mid(0, strValue.length() - precision);
    QString right = strValue.mid(strValue.length() - precision, precision);
    if (unit == "4") // base 60
    {
        QString wa = right.mid(0, 2);
        QString wb = right.mid(2, 2);
        double a = wa.toDouble() / 60.0;
        double b = wb.toDouble() / 3600.0;
        value = QString(left).toDouble(&ok);
        value += (a + b);
    }
    else
    {
        value = QString(left + "." + right).toDouble(&ok);
    }

    if (unit == "3" || unit == "4") // ° -> gon
    {
        value = value * 100.0 / 90.0;
    }

    if (!ok)
    {
        return 0;
    }

    return value;
}

void TT_GroupBoxImportGsi::slot_tbOpenPressed()
{
    QString fileNameLocal = QFileDialog::getOpenFileName(this, tr("Select a Gsi file"), "", tr("Gsi (*.gsi)"));
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
