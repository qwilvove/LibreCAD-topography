#include "tt_dialogadd.h"
#include "ui_tt_dialogadd.h"

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
    if (ui->cbType->currentIndex() == TT::PTYPE::POINT)
    {
        tempPoint.type = TT::PTYPE::POINT;
        tempPoint.name = ui->leName->text().trimmed();
        tempPoint.x    = ui->leX->text().toDouble();
        tempPoint.y    = ui->leY->text().toDouble();
        tempPoint.hasZ = ui->cbHasZ->isChecked();
        if (tempPoint.hasZ)
        {
            tempPoint.z = ui->leZ->text().toDouble();
        }
    }
    else if (ui->cbType->currentIndex() == TT::PTYPE::STATION)
    {
        bool ok = true;
        tempPoint.type = TT::PTYPE::STATION;
        tempPoint.name = ui->leName->text().trimmed();
        tempPoint.ih   = ui->leIh->text().toDouble();
        tempPoint.v0   = ui->leV0->text().toDouble(&ok);
        if (!ok)
        {
            tempPoint.v0 = -1;
        }
    }
    else if (ui->cbType->currentIndex() == TT::PTYPE::REFERENCE)
    {
        tempPoint.type = TT::PTYPE::REFERENCE;
        tempPoint.name = ui->leName->text().trimmed();
        tempPoint.ph   = ui->lePh->text().toDouble();
        tempPoint.ha   = ui->leHa->text().toDouble();
        tempPoint.va   = ui->leVa->text().toDouble();
        tempPoint.id   = ui->leId->text().toDouble();
    }
    else // ui->cbType->currentIndex() == TT::PTYPE::MEASURE
    {
        tempPoint.type = TT::PTYPE::MEASURE;
        tempPoint.name = ui->leName->text().trimmed();
        tempPoint.ph   = ui->lePh->text().toDouble();
        tempPoint.ha   = ui->leHa->text().toDouble();
        tempPoint.va   = ui->leVa->text().toDouble();
        tempPoint.id   = ui->leId->text().toDouble();
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

    if (index == TT::PTYPE::POINT)
    {
        ui->leName->setEnabled(true);
        ui->leX->setEnabled(true);
        ui->leY->setEnabled(true);
        ui->cbHasZ->setEnabled(true);
        //ui->buttonBox->setEnabled(true);
    }
    else if (index == TT::PTYPE::STATION)
    {
        ui->leName->setEnabled(true);
        ui->leIh->setEnabled(true);
        ui->leV0->setEnabled(true);
        //ui->buttonBox->setEnabled(true);
    }
    else if (index == TT::PTYPE::REFERENCE || index == TT::PTYPE::MEASURE)
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

