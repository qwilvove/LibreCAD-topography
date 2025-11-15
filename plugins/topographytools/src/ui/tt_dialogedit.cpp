#include "tt_dialogedit.h"
#include "ui_tt_dialogedit.h"

TT_DialogEdit::TT_DialogEdit(QWidget *parent, TT::Point *point) :
    QDialog(parent),
    ui(new Ui::TT_DialogEdit),
    point(point)
{
    ui->setupUi(this);

    tempPoint = {};

    ui->cbType->addItem(tr("POINT"));
    ui->cbType->addItem(tr("STATION"));
    ui->cbType->addItem(tr("REFERENCE"));
    ui->cbType->addItem(tr("MEASURE"));
    ui->cbType->setCurrentIndex(-1);

    loadData();
}

TT_DialogEdit::~TT_DialogEdit()
{
    delete ui;
}

void TT_DialogEdit::loadData()
{
    ui->cbType->setCurrentIndex(point->type);

    if (ui->cbType->currentIndex() == TT::PTYPE::POINT)
    {
        ui->leName->setText(point->name);
        ui->leX->setText(QString("%1").arg(point->x, 0, 'f', 3));
        ui->leY->setText(QString("%1").arg(point->y, 0, 'f', 3));
        ui->cbHasZ->setChecked(point->hasZ);
        if (point->hasZ)
        {
            ui->leZ->setText(QString("%1").arg(point->z, 0, 'f', 3));
        }
    }
    else if (ui->cbType->currentIndex() == TT::PTYPE::STATION)
    {
        ui->leName->setText(point->name);
        ui->leIh->setText(QString("%1").arg(point->ih, 0, 'f', 3));
        if (point->v0 >= 0)
        {
            ui->leV0->setText(QString("%1").arg(point->v0, 0, 'f', 5));
        }
    }
    else if (ui->cbType->currentIndex() == TT::PTYPE::REFERENCE ||
             ui->cbType->currentIndex() == TT::PTYPE::MEASURE)
    {
        ui->leName->setText(point->name);
        ui->lePh->setText(QString("%1").arg(point->ph, 0, 'f', 3));
        ui->leHa->setText(QString("%1").arg(point->ha, 0, 'f', 5));
        ui->leVa->setText(QString("%1").arg(point->va, 0, 'f', 5));
        ui->leId->setText(QString("%1").arg(point->id, 0, 'f', 3));
    }

    ui->buttonBox->setEnabled(true);
}

void TT_DialogEdit::saveData()
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

void TT_DialogEdit::on_cbType_currentIndexChanged(int index)
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

    if (index == TT::PTYPE::POINT)
    {
        ui->leName->setEnabled(true);
        ui->leX->setEnabled(true);
        ui->leY->setEnabled(true);
        ui->cbHasZ->setEnabled(true);
    }
    else if (index == TT::PTYPE::STATION)
    {
        ui->leName->setEnabled(true);
        ui->leIh->setEnabled(true);
        ui->leV0->setEnabled(true);
    }
    else if (index == TT::PTYPE::REFERENCE || index == TT::PTYPE::MEASURE)
    {
        ui->leName->setEnabled(true);
        ui->lePh->setEnabled(true);
        ui->leHa->setEnabled(true);
        ui->leVa->setEnabled(true);
        ui->leId->setEnabled(true);
    }
}

void TT_DialogEdit::on_leName_textChanged(const QString &arg1)
{
    ui->buttonBox->setEnabled(false);

    if (!arg1.trimmed().isEmpty())
    {
        ui->buttonBox->setEnabled(true);
    }
}

void TT_DialogEdit::on_cbHasZ_stateChanged(int arg1)
{
    ui->leZ->setEnabled(arg1);
}

void TT_DialogEdit::on_buttonBox_accepted()
{
    saveData();
}

