#include "tt_dialogpoints.h"
#include "ui_tt_dialogpoints.h"

#include <QCheckBox>
#include <QMessageBox>

TT_DialogPoints::TT_DialogPoints(QWidget *parent, QList<TT::Point *> &points) :
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
        if (points.at(i)->type == TT::PTYPE::STATION)
        {
            TT::Point *currentStation = points.at(i);
            QList<TT::Point*> currentMeasures = {};

            do
            {
                i++;
                if (points.at(i)->type == TT::PTYPE::MEASURE)
                {
                    currentMeasures.append(points.at(i));
                }
            }
            while (i < points.size() && points.at(i)->type != TT::PTYPE::STATION);

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

void TT_DialogPoints::on_pbSelectAll_clicked()
{
    for (int i = 0; i < ui->verticalLayout_2->count(); i++)
    {
        ((QCheckBox*)ui->verticalLayout_2->itemAt(i)->widget())->setChecked(true);
    }
}

void TT_DialogPoints::on_pbUnselectAll_clicked()
{
    for (int i = 0; i < ui->verticalLayout_2->count(); i++)
    {
        ((QCheckBox*)ui->verticalLayout_2->itemAt(i)->widget())->setChecked(false);
    }
}

void TT_DialogPoints::on_pbCalculatePoints_clicked()
{
    // Check if at least one station is selected (get selected stations and corresponding measures)
    QList<TT::Point*> selectedStations = {};
    QList<QList<TT::Point*>> selectedMeasures = {};
    for (int i = 0; i < ui->verticalLayout_2->count(); i++)
    {
        if ( ((QCheckBox*)ui->verticalLayout_2->itemAt(i)->widget())->isChecked() )
        {
            selectedStations.append(stations.at(i));
            selectedMeasures.append(measures.at(i));
        }
    }
    if (selectedStations.size() < 1)
    {
        QMessageBox::warning(this, tr("Error!"), tr("At least one station must be selected!"), QMessageBox::StandardButton::Ok);
        return;
    }

    // Check if each checked station has coordinates (X and Y at least)
    QList<TT::Point*> stationsCoordinates = {};
    QList<bool> calculateWithZ = {};
    for (int i = 0; i < selectedStations.size(); i++)
    {
        bool found = false;
        for (int j = 0; j < points.size(); j++)
        {
            if (points.at(j)->type == TT::PTYPE::POINT && points.at(j)->name == selectedStations.at(i)->name)
            {
                stationsCoordinates.append(points.at(j));
                calculateWithZ.append(points.at(j)->hasZ);
                found = true;
                break;
            }
        }
        if (!found)
        {
            QMessageBox::warning(this, tr("Error!"), tr("Station %1 has no coordinates!").arg(selectedStations.at(i)->name), QMessageBox::StandardButton::Ok);
            return;
        }
    }

    // Check if each checked station has v0
    for (int i = 0; i < selectedStations.size(); i++)
    {
        if (selectedStations.at(i)->v0 < 0)
        {
            QMessageBox::warning(this, tr("Error!"), tr("Station %1 has no v0!").arg(selectedStations.at(i)->name), QMessageBox::StandardButton::Ok);
            return;
        }
    }

    // For each selected stations, calculate measures
    int nbMeasuresCalculated = 0;
    for (int i = 0; i < selectedStations.size(); i++)
    {
        QList<TT::Point*> newPointsForTheStation = {};
        for (int j = 0; j < selectedMeasures.at(i).size(); j++)
        {
            // Calculate horizontal distance
            double va = selectedMeasures.at(i).at(j)->va;
            double id = selectedMeasures.at(i).at(j)->id;
            double horizontalDistance = id * std::sin(va * M_PI / 200.0);

            // Calculate X
            double x = stationsCoordinates.at(i)->x + horizontalDistance * std::sin(( selectedStations.at(i)->v0 + selectedMeasures.at(i).at(j)->ha ) * M_PI / 200.0);

            // Calculate Y
            double y = stationsCoordinates.at(i)->y + horizontalDistance * std::cos(( selectedStations.at(i)->v0 + selectedMeasures.at(i).at(j)->ha ) * M_PI / 200.0);

            TT::Point* newPoint = new TT::Point();
            newPoint->type = TT::PTYPE::POINT;
            newPoint->name = selectedMeasures.at(i).at(j)->name;
            newPoint->x = x;
            newPoint->y = y;
            newPoint->hasZ = false;
            newPoint->z = 0.0;

            // If needed, calculate Z
            if (calculateWithZ.at(i))
            {
                double z = stationsCoordinates.at(i)->z + selectedStations.at(i)->ih + id * std::cos(va * M_PI / 200.0) - selectedMeasures.at(i).at(j)->ph;

                newPoint->hasZ = true;
                newPoint->z = z;
            }
            newPointsForTheStation.append(newPoint);
            nbMeasuresCalculated++;
        }
        newPoints.append(newPointsForTheStation);
    }

    QMessageBox::information(this, tr("Success!"), tr("%1 points calculated!").arg(nbMeasuresCalculated), QMessageBox::StandardButton::Ok);
    ui->buttonBox->setEnabled(true);
}

void TT_DialogPoints::on_buttonBox_accepted()
{
    // Write newPoints
    for (int i = 0; i < newPoints.size(); i++)
    {
        for (int j = 0; j < newPoints.at(i).size(); j++)
        {
            points.append(newPoints.at(i).at(j));
        }
    }
}

