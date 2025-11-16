#include "polygo.h"
#include "ui_polygo.h"

#include <QMessageBox>

TT_DialogPolygo::TT_DialogPolygo(QWidget *parent, QList<TT::Point *> &points) :
    QDialog(parent),
    ui(new Ui::TT_DialogPolygo),
    points(points)
{
    ui->setupUi(this);

    fillStationsAndReferences();
    fillItemsStations();
    displayItems();
}

TT_DialogPolygo::~TT_DialogPolygo()
{
    delete ui;
}

void TT_DialogPolygo::fillStationsAndReferences()
{
    for (int i = 0; i < points.size(); )
    {
        if (points.at(i)->type == TT::Point::TYPE::STATION)
        {
            TT::Point *currentStation = points.at(i);
            QList<TT::Point*> currentReferences = {};

            do
            {
                i++;
                if (i < points.size() && points.at(i)->type == TT::Point::TYPE::REFERENCE)
                {
                    currentReferences.append(points.at(i));
                }
            }
            while (i < points.size() && points.at(i)->type != TT::Point::TYPE::STATION);

            stations.append(currentStation);
            references.append(currentReferences);
        }
        else
        {
            i++;
        }
    }
}

void TT_DialogPolygo::fillItemsStations()
{
    for (int i = 0; i < stations.size(); i++)
    {
        itemsStations.append(stations.at(i));
    }
}

void TT_DialogPolygo::displayItems()
{
    ui->listStations->clear();
    for (int i = 0; i < itemsStations.size(); i++)
    {
        ui->listStations->addItem(itemsStations.at(i)->name);
    }

    ui->listPolygo->clear();
    for (int i = 0; i < itemsPolygo.size(); i++)
    {
        ui->listPolygo->addItem(itemsPolygo.at(i)->name);
    }
}

void TT_DialogPolygo::calculateAntennaPath()
{
    bool calculateWithZ = false;

    // List references used in the path
    QList<QList<TT::Point*>> polygoReferences = {};
    for (int i = 0; i < itemsPolygo.size(); i++)
    {
        for (int j = 0; j < stations.size(); j++)
        {
            if (itemsPolygo.at(i)->name == stations.at(j)->name)
            {
                polygoReferences.append(references.at(j));
            }
        }
    }

    // Check if the path has at least two stations
    if (itemsPolygo.size() < 2)
    {
        QMessageBox::warning(this, tr("Error!"), tr("The path must have at least 2 stations!"), QMessageBox::StandardButton::Ok);
        return;
    }

    // Check if the first station of the path has coordinates (at least X and Y)
    TT::Point *firstStationCoordinates = nullptr;
    bool found = false;
    for (int i = 0; i < points.size(); i++)
    {
        if (points.at(i)->type == TT::Point::TYPE::POINT && points.at(i)->name == itemsPolygo.at(0)->name)
        {
            firstStationCoordinates = points.at(i);
            calculateWithZ = points.at(i)->hasZ;
            found = true;
            break;
        }
    }
    if (!found)
    {
        QMessageBox::warning(this, tr("Error!"), tr("The first station of the path has no coordinates!"), QMessageBox::StandardButton::Ok);
        return;
    }

    // Check if the first station of the path has a V0
    if (itemsPolygo.at(0)->v0 < 0)
    {
        QMessageBox::warning(this, tr("Error!"), tr("The first station of the path has no V0!"), QMessageBox::StandardButton::Ok);
        return;
    }

    // Check if all stations of the path are linked by two views (front and back)
    QList<OrderedReference> orderedReferences = {};
    for (int i = 0; i < itemsPolygo.size(); i++)
    {
        OrderedReference orderedReference = {};
        // Check for corresponding front view
        if (i != itemsPolygo.size() - 1)
        {
            bool found = false;
            for (int j = 0; j < polygoReferences.at(i).size(); j++)
            {
                if (polygoReferences.at(i).at(j)->name == itemsPolygo.at(i+1)->name)
                {
                    found = true;
                    orderedReference.frontReference = polygoReferences.at(i).at(j);
                    break;
                }
            }
            if (!found)
            {
                QMessageBox::warning(this, tr("Error!"), tr("Missing front view between stations %1 and %2!").arg(itemsPolygo.at(i)->name).arg(itemsPolygo.at(i+1)->name), QMessageBox::StandardButton::Ok);
                return;
            }
        }

        // Check for corresponding back view
        if (i != 0)
        {
            bool found = false;
            for (int j = 0; j < polygoReferences.at(i).size(); j++)
            {
                if (polygoReferences.at(i).at(j)->name == itemsPolygo.at(i-1)->name)
                {
                    found = true;
                    orderedReference.backReference = polygoReferences.at(i).at(j);
                    break;
                }
            }
            if (!found)
            {
                QMessageBox::warning(this, tr("Error!"), tr("Missing back view between stations %1 and %2!").arg(itemsPolygo.at(i)->name).arg(itemsPolygo.at(i-1)->name), QMessageBox::StandardButton::Ok);
                return;
            }
        }
        orderedReferences.append(orderedReference);
    }

    // v0s (for each stations of the polygo)
    v0s.clear();
    v0s.append(itemsPolygo.at(0)->v0);
    // Horizontal distances (for each stations of the polygo, 0 for S1)
    hds.clear();
    hds.append(0.0);
    // X (for each stations of the polygo)
    xs.clear();
    xs.append(firstStationCoordinates->x);
    // Y (for each stations of the polygo)
    ys.clear();
    ys.append(firstStationCoordinates->y);
    // Z (for each stations of the polygo)
    zs.clear();
    if (calculateWithZ)
    {
        zs.append(firstStationCoordinates->z);
    }

    // For each station (except the first one)
    for (int i = 1; i < itemsPolygo.size(); i++)
    {
        // First, calculate v0
        double v0 = std::fmod(v0s.at(i-1) + orderedReferences.at(i-1).frontReference->ha - orderedReferences.at(i).backReference->ha + 200.0, 400.0);
        if (v0 < 0.0)
        {
            v0 += 400.0;
        }
        v0s.append(v0);

        // Then, calculate horizontal distance (front and back, then average)
        double va = orderedReferences.at(i-1).frontReference->va;
        double id = orderedReferences.at(i-1).frontReference->id;
        double horizontalDistanceFront = id * std::sin(va * M_PI / 200.0);

        va = orderedReferences.at(i).backReference->va;
        id = orderedReferences.at(i).backReference->id;
        double horizontalDistanceBack = id * std::sin(va * M_PI / 200.0);

        double horizontalDistanceAverage = ( horizontalDistanceFront + horizontalDistanceBack ) / 2.0;
        hds.append(horizontalDistanceAverage);

        // Calculate X
        double x = xs.at(i-1) + hds.at(i) * std::sin(( v0s.at(i-1) + orderedReferences.at(i-1).frontReference->ha ) * M_PI / 200.0);
        xs.append(x);

        // Calculate Y
        double y = ys.at(i-1) + hds.at(i) * std::cos(( v0s.at(i-1) + orderedReferences.at(i-1).frontReference->ha ) * M_PI / 200.0);
        ys.append(y);

        // If needed, calculate Z
        if (calculateWithZ)
        {
            va = orderedReferences.at(i-1).frontReference->va;
            id = orderedReferences.at(i-1).frontReference->id;
            double z = zs.at(i-1) + itemsPolygo.at(i-1)->ih + id * std::cos(va * M_PI / 200.0) - orderedReferences.at(i-1).frontReference->ph;
            zs.append(z);
        }
    }

    QMessageBox::information(this, tr("Success!"), tr("Polygonation calculated!"), QMessageBox::StandardButton::Ok);
    ui->buttonBox->setEnabled(true);
}

void TT_DialogPolygo::writeDataAfterCalculateAntennaPath()
{
    // Write v0s for each station (except the first one)
    for (int i = 1; i < itemsPolygo.size(); i++)
    {
        for (int j = 0; j < points.size(); j++)
        {
            if (points.at(j)->name == itemsPolygo.at(i)->name && points.at(j)->type == itemsPolygo.at(i)->type)
            {
                points.at(j)->v0 = v0s.at(i);
                break;
            }
        }
    }

    // Create points for each station (except the first one)
    for (int i = 1; i < itemsPolygo.size(); i++)
    {
        for (int j = 0; j < points.size(); j++)
        {
            if (points.at(j)->name == itemsPolygo.at(i)->name && points.at(j)->type == itemsPolygo.at(i)->type)
            {
                TT::Point *newPoint = new TT::Point();
                newPoint->type = TT::Point::TYPE::POINT;
                newPoint->name = itemsPolygo.at(i)->name;
                newPoint->x = xs.at(i);
                newPoint->y = ys.at(i);
                if (zs.size() > 0)
                {
                    newPoint->hasZ = true;
                    newPoint->z = zs.at(i);
                }
                points.append(newPoint);
                break;
            }
        }
    }
}

void TT_DialogPolygo::on_listStations_itemDoubleClicked(QListWidgetItem *item)
{
    Q_UNUSED(item);

    int currentRow = ui->listStations->currentRow();
    itemsPolygo.append(itemsStations.at(currentRow));
    itemsStations.remove(currentRow);
    displayItems();
    ui->buttonBox->setEnabled(false);
}

void TT_DialogPolygo::on_pbRight_clicked()
{
    int currentRow = ui->listStations->currentRow();
    if (currentRow >= 0)
    {
        itemsPolygo.append(itemsStations.at(currentRow));
        itemsStations.remove(currentRow);
        displayItems();
        ui->listStations->setCurrentRow(currentRow);
        ui->buttonBox->setEnabled(false);
    }
}

void TT_DialogPolygo::on_listPolygo_itemDoubleClicked(QListWidgetItem *item)
{
    Q_UNUSED(item);

    int currentRow = ui->listPolygo->currentRow();
    itemsStations.append(itemsPolygo.at(currentRow));
    itemsPolygo.remove(currentRow);
    displayItems();
    ui->buttonBox->setEnabled(false);
}

void TT_DialogPolygo::on_pbLeft_clicked()
{
    int currentRow = ui->listPolygo->currentRow();
    if (currentRow >= 0)
    {
        itemsStations.append(itemsPolygo.at(currentRow));
        itemsPolygo.remove(currentRow);
        displayItems();
        ui->listPolygo->setCurrentRow(currentRow);
        ui->buttonBox->setEnabled(false);
    }
}

void TT_DialogPolygo::on_pbUp_clicked()
{
    int index = ui->listPolygo->currentRow();
    if (index >= 1 && index < itemsPolygo.size())
    {
        itemsPolygo.move(index, index - 1);
        displayItems();
        ui->listPolygo->setCurrentRow(index - 1);
        ui->buttonBox->setEnabled(false);
    }
}

void TT_DialogPolygo::on_pbDown_clicked()
{
    int index = ui->listPolygo->currentRow();
    if (index >= 0 && index < itemsPolygo.size() - 1)
    {
        itemsPolygo.move(index, index + 1);
        displayItems();
        ui->listPolygo->setCurrentRow(index + 1);
        ui->buttonBox->setEnabled(false);
    }
}

void TT_DialogPolygo::on_cbPathType_currentIndexChanged(int index)
{
    Q_UNUSED(index);

    ui->buttonBox->setEnabled(false);
}

void TT_DialogPolygo::on_pbCalculate_clicked()
{
    int pathType = ui->cbPathType->currentIndex();
    if (pathType == 0) // Antenna path
    {
        calculateAntennaPath();
    }
    else if (pathType == 1) // Bounded path
    {
        QMessageBox::critical(this, tr("Error!"), tr("Function not implemented yet!"), QMessageBox::StandardButton::Ok);
        return;
    }
    else if (pathType == 2) // Closed path
    {
        QMessageBox::critical(this, tr("Error!"), tr("Function not implemented yet!"), QMessageBox::StandardButton::Ok);
        return;
    }
    else // Error : invalid path type
    {
        QMessageBox::warning(this, tr("Error!"), tr("No valid path type is selected!"), QMessageBox::StandardButton::Ok);
        return;
    }
}

void TT_DialogPolygo::on_buttonBox_accepted()
{
    int pathType = ui->cbPathType->currentIndex();
    if (pathType == 0) // Antenna path
    {
        writeDataAfterCalculateAntennaPath();
    }
    else if (pathType == 1) // Bounded path
    {

    }
    else if (pathType == 2) // Closed path
    {

    }
}
