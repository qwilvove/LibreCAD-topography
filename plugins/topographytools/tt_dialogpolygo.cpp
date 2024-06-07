#include "tt_dialogpolygo.h"
#include "ui_tt_dialogpolygo.h"

TT_DialogPolygo::TT_DialogPolygo(QWidget *parent, QList<TT::Point> &points) :
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
        if (points.at(i).type == TT::PTYPE::STATION)
        {
            TT::Point *currentStation = &(points[i]);
            QList<TT::Point*> currentReferences = {};

            do
            {
                i++;
                if (points.at(i).type == TT::PTYPE::REFERENCE)
                {
                    currentReferences.append(&(points[i]));
                }
            }
            while (i < points.size() && points.at(i).type != TT::PTYPE::STATION);

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

void TT_DialogPolygo::on_pbRight_clicked()
{
    int currentRow = ui->listStations->currentRow();
    if (currentRow >= 0)
    {
        itemsPolygo.append(itemsStations.at(currentRow));
        itemsStations.remove(currentRow);
        displayItems();
    }
}

void TT_DialogPolygo::on_pbLeft_clicked()
{
    int currentRow = ui->listPolygo->currentRow();
    if (currentRow >= 0)
    {
        itemsStations.append(itemsPolygo.at(currentRow));
        itemsPolygo.remove(currentRow);
        displayItems();
    }
}

void TT_DialogPolygo::on_pbUp_clicked()
{
    int index = ui->listPolygo->currentRow();
    if (index >= 1 && index < itemsPolygo.size())
    {
        itemsPolygo.move(index, index - 1);
        displayItems();
    }
}

void TT_DialogPolygo::on_pbDown_clicked()
{
    int index = ui->listPolygo->currentRow();
    if (index >= 0 && index < itemsPolygo.size() - 1)
    {
        itemsPolygo.move(index, index + 1);
        displayItems();
    }
}
