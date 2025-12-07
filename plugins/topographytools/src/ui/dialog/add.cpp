#include "add.h"
#include "ui_add.h"

TT_DialogAdd::TT_DialogAdd(QWidget *parent, TT::Point *point) :
    QDialog(parent),
    ui(new Ui::TT_DialogAdd),
    point(point)
{
    ui->setupUi(this);

    tempPoint = {};

    ui->cbType->addItem(tr("POINT"));
    ui->cbType->addItem(tr("STATION"));
    ui->cbType->addItem(tr("REFERENCE"));
    ui->cbType->addItem(tr("MEASURE"));
    ui->cbType->setCurrentIndex(-1);
}

TT_DialogAdd::~TT_DialogAdd()
{
    delete ui;
}

void TT_DialogAdd::saveData()
{
    if (ui->cbType->currentIndex() == TT::Point::TYPE::POINT)
    {
        tempPoint.setType(TT::Point::TYPE::POINT);
        tempPoint.setName(ui->leName->text().trimmed());
        tempPoint.setX(ui->leX->text().toDouble());
        tempPoint.setY(ui->leY->text().toDouble());
        tempPoint.setHasZ(ui->cbHasZ->isChecked());
        if (ui->cbHasZ->isChecked())
        {
            tempPoint.setZ(ui->leZ->text().toDouble());
        }
    }
    else if (ui->cbType->currentIndex() == TT::Point::TYPE::STATION)
    {
        bool ok = true;
        tempPoint.setType(TT::Point::TYPE::STATION);
        tempPoint.setName(ui->leName->text().trimmed());
        tempPoint.setIh(ui->leIh->text().toDouble());
        tempPoint.setV0(ui->leV0->text().toDouble(&ok));
        if (!ok)
        {
            tempPoint.setV0(-1);
        }
    }
    else if (ui->cbType->currentIndex() == TT::Point::TYPE::REFERENCE)
    {
        tempPoint.setType(TT::Point::TYPE::REFERENCE);
        tempPoint.setName(ui->leName->text().trimmed());
        tempPoint.setPh(ui->lePh->text().toDouble());
        tempPoint.setHa(ui->leHa->text().toDouble());
        tempPoint.setVa(ui->leVa->text().toDouble());
        tempPoint.setId(ui->leId->text().toDouble());
    }
    else // ui->cbType->currentIndex() == TT::Point::TYPE::MEASURE
    {
        tempPoint.setType(TT::Point::TYPE::MEASURE);
        tempPoint.setName(ui->leName->text().trimmed());
        tempPoint.setPh(ui->lePh->text().toDouble());
        tempPoint.setHa(ui->leHa->text().toDouble());
        tempPoint.setVa(ui->leVa->text().toDouble());
        tempPoint.setId(ui->leId->text().toDouble());
    }

    *point = tempPoint;
}

void TT_DialogAdd::on_cbType_currentIndexChanged(int index)
{
    // Init
    ui->leName->setEnabled(false);
    ui->leX->setEnabled(false);
    ui->leY->setEnabled(false);
    ui->cbHasZ->setEnabled(false);
    ui->leZ->setEnabled(false);
    ui->leIh->setEnabled(false);
    ui->leV0->setEnabled(false);
    ui->lePh->setEnabled(false);
    ui->leHa->setEnabled(false);
    ui->leVa->setEnabled(false);
    ui->leId->setEnabled(false);
    ui->buttonBox->setEnabled(false);

    if (index == TT::Point::TYPE::POINT)
    {
        ui->leName->setEnabled(true);
        ui->leX->setEnabled(true);
        ui->leY->setEnabled(true);
        ui->cbHasZ->setEnabled(true);
        //ui->buttonBox->setEnabled(true);
    }
    else if (index == TT::Point::TYPE::STATION)
    {
        ui->leName->setEnabled(true);
        ui->leIh->setEnabled(true);
        ui->leV0->setEnabled(true);
        //ui->buttonBox->setEnabled(true);
    }
    else if (index == TT::Point::TYPE::REFERENCE || index == TT::Point::TYPE::MEASURE)
    {
        ui->leName->setEnabled(true);
        ui->lePh->setEnabled(true);
        ui->leHa->setEnabled(true);
        ui->leVa->setEnabled(true);
        ui->leId->setEnabled(true);
        //ui->buttonBox->setEnabled(true);
    }
}

void TT_DialogAdd::on_leName_textChanged(const QString &arg1)
{
    ui->buttonBox->setEnabled(false);

    if (!arg1.trimmed().isEmpty())
    {
        ui->buttonBox->setEnabled(true);
    }
}

void TT_DialogAdd::on_cbHasZ_stateChanged(int arg1)
{
    ui->leZ->setEnabled(arg1);
}

void TT_DialogAdd::on_buttonBox_accepted()
{
    saveData();
}

