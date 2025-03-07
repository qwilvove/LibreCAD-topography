#include "tt_dialogpoints.h"
#include "ui_tt_dialogpoints.h"

#include <QCheckBox>

TT_DialogPoints::TT_DialogPoints(QWidget *parent, QList<TT::Point> &points) :
    QDialog(parent),
    ui(new Ui::TT_DialogPoints),
    points(points)
{
    ui->setupUi(this);

    identifyStationsAndMeasures();
    setupCheckBoxes();
}

TT_DialogPoints::~TT_DialogPoints()
{
    delete ui;
}

void TT_DialogPoints::identifyStationsAndMeasures()
{
    for (int i = 0; i < points.size(); )
    {
        if (points.at(i).type == TT::PTYPE::STATION)
        {
            TT::Point *currentStation = &(points[i]);
            QList<TT::Point*> currentMeasures = {};

            do
            {
                i++;
                if (points.at(i).type == TT::PTYPE::MEASURE)
                {
                    currentMeasures.append(&(points[i]));
                }
            }
            while (i < points.size() && points.at(i).type != TT::PTYPE::STATION);

            stations.append(currentStation);
            measures.append(currentMeasures);
        }
        else
        {
            i++;
        }
    }
}

void TT_DialogPoints::setupCheckBoxes()
{
    for (int i = 0; i < stations.size(); i++)
    {
        QCheckBox *cb = new QCheckBox(this);
        cb->setText(stations.at(i)->name);
        ui->verticalLayout_2->addWidget(cb);
    }
}
