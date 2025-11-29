#include "edit.h"
#include "ui_edit.h"

TT_DialogEdit::TT_DialogEdit(QWidget *parent, TT::Point *point) :
    QDialog(parent),
    ui(new Ui::TT_DialogEdit),
    point(point)
{
    ui->setupUi(this);

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
    ui->leName->setText(point->name);
    ui->leCode->setText(point->code);

    if (ui->cbType->currentIndex() == TT::Point::TYPE::POINT)
    {
        ui->leX->setText(QString("%1").arg(point->x, 0, 'f', 3));
        ui->leY->setText(QString("%1").arg(point->y, 0, 'f', 3));
        ui->cbHasZ->setChecked(point->hasZ);
        if (point->hasZ)
        {
            ui->leZ->setText(QString("%1").arg(point->z, 0, 'f', 3));
        }
    }
    else if (ui->cbType->currentIndex() == TT::Point::TYPE::STATION)
    {
        ui->leIh->setText(QString("%1").arg(point->ih, 0, 'f', 3));
        if (point->v0 >= 0)
        {
            ui->leV0->setText(QString("%1").arg(point->v0, 0, 'f', 5));
        }
    }
    else if (ui->cbType->currentIndex() == TT::Point::TYPE::REFERENCE ||
             ui->cbType->currentIndex() == TT::Point::TYPE::MEASURE)
    {
        ui->lePh->setText(QString("%1").arg(point->ph, 0, 'f', 3));
        ui->leHa->setText(QString("%1").arg(point->ha, 0, 'f', 5));
        ui->leVa->setText(QString("%1").arg(point->va, 0, 'f', 5));
        ui->leId->setText(QString("%1").arg(point->id, 0, 'f', 3));
    }

    ui->buttonBox->setEnabled(true);
}

void TT_DialogEdit::saveData()
{
    point->name = ui->leName->text().trimmed();
    point->code = ui->leCode->text().trimmed();

    if (ui->cbType->currentIndex() == TT::Point::TYPE::POINT)
    {
        point->type = TT::Point::TYPE::POINT;
        point->x    = ui->leX->text().toDouble();
        point->y    = ui->leY->text().toDouble();
        point->hasZ = ui->cbHasZ->isChecked();
        if (point->hasZ)
        {
            point->z = ui->leZ->text().toDouble();
        }
    }
    else if (ui->cbType->currentIndex() == TT::Point::TYPE::STATION)
    {
        bool ok = true;
        point->type = TT::Point::TYPE::STATION;
        point->ih   = ui->leIh->text().toDouble();
        point->v0   = ui->leV0->text().toDouble(&ok);
        if (!ok)
        {
            point->v0 = -1;
        }
    }
    else if (ui->cbType->currentIndex() == TT::Point::TYPE::REFERENCE)
    {
        point->type = TT::Point::TYPE::REFERENCE;
        point->ph   = ui->lePh->text().toDouble();
        point->ha   = ui->leHa->text().toDouble();
        point->va   = ui->leVa->text().toDouble();
        point->id   = ui->leId->text().toDouble();
    }
    else // ui->cbType->currentIndex() == TT::Point::TYPE::MEASURE
    {
        point->type = TT::Point::TYPE::MEASURE;
        point->ph   = ui->lePh->text().toDouble();
        point->ha   = ui->leHa->text().toDouble();
        point->va   = ui->leVa->text().toDouble();
        point->id   = ui->leId->text().toDouble();
    }
}

void TT_DialogEdit::on_cbType_currentIndexChanged(int index)
{
    // Init
    ui->leName->setEnabled(true);
    ui->leCode->setEnabled(true);

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

    if (index == TT::Point::TYPE::POINT)
    {
        ui->leX->setEnabled(true);
        ui->leY->setEnabled(true);
        ui->cbHasZ->setEnabled(true);
        ui->leZ->setEnabled(ui->cbHasZ->isChecked());
    }
    else if (index == TT::Point::TYPE::STATION)
    {
        ui->leIh->setEnabled(true);
        ui->leV0->setEnabled(true);
    }
    else if (index == TT::Point::TYPE::REFERENCE || index == TT::Point::TYPE::MEASURE)
    {
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

