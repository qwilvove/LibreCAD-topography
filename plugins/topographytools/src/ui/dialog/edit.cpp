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
    ui->cbType->setCurrentIndex(point->getType());
    ui->leName->setText(point->getName());
    ui->leCode->setText(point->getCode());

    if (ui->cbType->currentIndex() == TT::Point::TYPE::POINT)
    {
        ui->leX->setText(QString("%1").arg(point->getX(), 0, 'f', 3));
        ui->leY->setText(QString("%1").arg(point->getY(), 0, 'f', 3));
        ui->cbHasZ->setChecked(point->getHasZ());
        if (point->getHasZ())
        {
            ui->leZ->setText(QString("%1").arg(point->getZ(), 0, 'f', 3));
        }
    }
    else if (ui->cbType->currentIndex() == TT::Point::TYPE::STATION)
    {
        ui->leIh->setText(QString("%1").arg(point->getIh(), 0, 'f', 3));
        if (point->getV0() >= 0)
        {
            ui->leV0->setText(QString("%1").arg(point->getV0(), 0, 'f', 5));
        }
    }
    else if (ui->cbType->currentIndex() == TT::Point::TYPE::REFERENCE ||
             ui->cbType->currentIndex() == TT::Point::TYPE::MEASURE)
    {
        ui->lePh->setText(QString("%1").arg(point->getPh(), 0, 'f', 3));
        ui->leHa->setText(QString("%1").arg(point->getHa(), 0, 'f', 5));
        ui->leVa->setText(QString("%1").arg(point->getVa(), 0, 'f', 5));
        ui->leId->setText(QString("%1").arg(point->getId(), 0, 'f', 3));
    }

    ui->buttonBox->setEnabled(true);
}

void TT_DialogEdit::saveData()
{
    point->setName(ui->leName->text().trimmed());
    point->setCode(ui->leCode->text().trimmed());

    if (ui->cbType->currentIndex() == TT::Point::TYPE::POINT)
    {
        point->setType(TT::Point::TYPE::POINT);
        point->setX(ui->leX->text().toDouble());
        point->setY(ui->leY->text().toDouble());
        point->setHasZ(ui->cbHasZ->isChecked());
        if (ui->cbHasZ->isChecked())
        {
            point->setZ(ui->leZ->text().toDouble());
        }
    }
    else if (ui->cbType->currentIndex() == TT::Point::TYPE::STATION)
    {
        bool ok = true;
        point->setType(TT::Point::TYPE::STATION);
        point->setIh(ui->leIh->text().toDouble());
        point->setV0(ui->leV0->text().toDouble(&ok));
        if (!ok)
        {
            point->setV0(-1);
        }
    }
    else if (ui->cbType->currentIndex() == TT::Point::TYPE::REFERENCE)
    {
        point->setType(TT::Point::TYPE::REFERENCE);
        point->setPh(ui->lePh->text().toDouble());
        point->setHa(ui->leHa->text().toDouble());
        point->setVa(ui->leVa->text().toDouble());
        point->setId(ui->leId->text().toDouble());
    }
    else // ui->cbType->currentIndex() == TT::Point::TYPE::MEASURE
    {
        point->setType(TT::Point::TYPE::MEASURE);
        point->setPh(ui->lePh->text().toDouble());
        point->setHa(ui->leHa->text().toDouble());
        point->setVa(ui->leVa->text().toDouble());
        point->setId(ui->leId->text().toDouble());
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

