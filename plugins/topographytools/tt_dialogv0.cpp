#include "tt_dialogv0.h"
#include "ui_tt_dialogv0.h"

#include <QMenuBar>
#include <QCheckBox>

TT_DialogV0::TT_DialogV0(QWidget *parent, QList<TT::Point> *points) :
    QDialog(parent),
    ui(new Ui::TT_DialogV0),
    points(points)
{
    ui->setupUi(this);

    identifyStationsAndReferences();
    setupComboBox();
}

TT_DialogV0::~TT_DialogV0()
{
    delete ui;
}

void TT_DialogV0::identifyStationsAndReferences()
{
    TT::Point currentStation {};
    QList<TT::Point> currentReferences {};

    for (int i = 0; i < points->size(); )
    {
        if (points->at(i).type == TT::PTYPE::STATION)
        {
            currentStation = points->at(i);
            currentReferences = {};

            do
            {
                i++;
                if (points->at(i).type == TT::PTYPE::REFERENCE)
                {
                    currentReferences.append(points->at(i));
                }
            }
            while (i < points->size() && points->at(i).type != TT::PTYPE::STATION);

            stationList.append(currentStation);
            referencesList.append(currentReferences);
        }
        else
        {
            i++;
        }
    }
}

void TT_DialogV0::setupComboBox()
{
    for (int i = 0; i < stationList.size(); i++)
    {
        ui->cbStation->addItem(stationList.at(i).name);
    }

    ui->cbStation->setCurrentIndex(-1);
}

void TT_DialogV0::setupCheckBoxes(int index)
{
    for (int i = 0; i < referencesList.at(index).size(); i++)
    {
        QCheckBox *cb = new QCheckBox(this);
        cb->setText(referencesList.at(index).at(i).name);
        ui->verticalLayout_2->addWidget(cb);
        checkBoxes.append(cb);
        //connect(cb, SIGNAL(toggled(bool)), this, SLOT(testCheckBoxes(bool)));
    }
}

void TT_DialogV0::clearCheckBoxes()
{
    QLayoutItem *child;
    while ((child = ui->verticalLayout_2->takeAt(0)) != nullptr)
    {
        delete child->widget();
        delete child;
    }

    checkBoxes.clear();
}

/*void TT_DialogV0::testCheckBoxes(bool state)
{
    QCheckBox* cb = qobject_cast<QCheckBox*>(sender());



}*/

void TT_DialogV0::on_cbStation_currentIndexChanged(int index)
{
    // Init
    ui->pbCalculate->setEnabled(false);
    ui->buttonBox->setEnabled(false);
    clearCheckBoxes();

    if (index >= 0)
    {
        //ui->pbCalculate->setEnabled(true);
        //ui->buttonBox->setEnabled(true);
        setupCheckBoxes(index);
    }
}

