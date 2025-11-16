#include "v0.h"
#include "ui_v0.h"

#include <QMessageBox>
#include <QCheckBox>

#include <QDebug>

double mod(double value, int angle)
{
    double mod = std::fmod(value, angle);

    if (mod < 0)
    {
        mod += angle;
    }
    else if (mod >= angle)
    {
        mod -= angle;
    }

    return mod;
}

TT_DialogV0::TT_DialogV0(QWidget *parent, QList<TT::Point *> &points) :
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
    for (int i = 0; i < points.size(); )
    {
        if (points.at(i)->type == TT::Point::TYPE::STATION)
        {
            currentStation = points.at(i);
            currentReferences = {};

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

void TT_DialogV0::setupComboBox()
{
    for (int i = 0; i < stations.size(); i++)
    {
        ui->cbStation->addItem(stations.at(i)->name);
    }

    ui->cbStation->setCurrentIndex(-1);
}

void TT_DialogV0::setupCheckBoxes(int index)
{
    for (int i = 0; i < references.at(index).size(); i++)
    {
        QCheckBox *cb = new QCheckBox(this);
        cb->setText(references.at(index).at(i)->name);
        ui->verticalLayout_2->addWidget(cb);
        checkBoxes.append(cb);
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

void TT_DialogV0::on_cbStation_currentIndexChanged(int index)
{
    // Init
    ui->pbCalculate->setEnabled(false);
    ui->buttonBox->setEnabled(false);
    clearCheckBoxes();

    if (index >= 0)
    {
        ui->pbCalculate->setEnabled(true);
        setupCheckBoxes(index);
        currentStation = stations.at(index);
        currentReferences = references.at(index);
    }
}

void TT_DialogV0::on_pbCalculate_clicked()
{
    // Check if the current station has coordinates (X and Y)
    TT::Point *currentStationCoordinates = new TT::Point();
    bool found = false;
    for (int i = 0; i < points.size(); i++)
    {
        if (points.at(i)->type == TT::Point::TYPE::POINT && points.at(i)->name == currentStation->name)
        {
            currentStationCoordinates = points.at(i);
            found = true;
            break;
        }
    }
    if (!found)
    {
        QMessageBox::warning(this, tr("Error!"), tr("The current station has no coordinates!"), QMessageBox::StandardButton::Ok);
        return;
    }

    // Check if the station has references
    if (currentReferences.isEmpty())
    {
        QMessageBox::warning(this, tr("Error!"), tr("The current station has no references!"), QMessageBox::StandardButton::Ok);
        return;
    }

    // Get checked references
    QList<TT::Point*> checkedReferences = {};
    for (int i = 0; i < checkBoxes.size(); i++)
    {
        QCheckBox *currentCheckBox = checkBoxes[i];
        if (currentCheckBox->isChecked())
        {
            checkedReferences.append(currentReferences.at(i));
        }
    }
    if (checkedReferences.isEmpty())
    {
        QMessageBox::warning(this, tr("Error!"), tr("Please check at least one reference!"), QMessageBox::StandardButton::Ok);
        return;
    }

    // Check if each checked reference as coordinates (X and Y)
    QList<TT::Point*> checkedReferencesCoordinates {};
    for (int i = 0; i < points.size(); i++)
    {
        for (int j = 0; j < checkedReferences.size(); j++)
        {
            if (points.at(i)->type == TT::Point::TYPE::POINT && points.at(i)->name == checkedReferences[j]->name)
            {
                checkedReferencesCoordinates.append(points.at(i));
                break;
            }
        }
    }
    if (checkedReferencesCoordinates.size() < checkedReferences.size())
    {
        QMessageBox::warning(this, tr("Error!"), tr("At least one of the checked references has no cooordinates!"), QMessageBox::StandardButton::Ok);
        return;
    }

    // For each checked reference
    TT::Point *a = currentStationCoordinates;
    for (int i = 0; i < checkedReferencesCoordinates.size(); i++)
    {
        // Check if distance between station and reference is not equal to zero
        TT::Point *b = checkedReferencesCoordinates.at(i);
        double distance = std::sqrt( std::pow(b->x - a->x, 2.0) + std::pow(b->y - a->y, 2.0) );
        if (distance == 0)
        {
            QMessageBox::warning(this, tr("Error!"), tr("At least one of the checked references has a distance with the station equal to zero!"), QMessageBox::StandardButton::Ok);
            return;
        }
    }

    // For each checked reference
    QList<double> v0s = {};
    QList<double> distances = {};
    double sumOfDistances = 0.0;
    for (int i = 0; i < checkedReferencesCoordinates.size(); i++)
    {
        // Calculate V0
        TT::Point *b = checkedReferencesCoordinates.at(i);
        double gAB = 2.0 * std::atan( ( b->x - a->x ) / ( std::sqrt( std::pow( b->x - a->x, 2.0 ) + std::pow( b->y - a->y, 2.0 ) ) + ( b->y - a->y ) ) ) * 200.0 / M_PI;
        double distance = std::sqrt( std::pow(b->x - a->x, 2.0) + std::pow(b->y - a->y, 2.0) );
        double v0 = mod(gAB - checkedReferences.at(i)->ha, 400);
        v0s.append(v0);
        distances.append(distance);
        sumOfDistances += distance;
    }

    // Calculate the average V0 weighted by distances
    double sumSin = 0.0;
    double sumCos = 0.0;

    for (auto i = 0; i < v0s.size(); ++i) {
        double dist = distances.at(i);
        double a = v0s.at(i) * M_PI / 200.0;
        sumSin += dist * std::sin(a);
        sumCos += dist * std::cos(a);
    }

    double averageV0 = mod(std::atan2(sumSin / sumOfDistances, sumCos / sumOfDistances) * 200.0 / M_PI, 400);

    ui->leV0->setText(QString("%1").arg(averageV0, 0, 'f', 5));
    ui->buttonBox->setEnabled(true);
}

void TT_DialogV0::on_buttonBox_accepted()
{
    double averageV0 = ui->leV0->text().toDouble();
    currentStation->v0 = averageV0;
}

