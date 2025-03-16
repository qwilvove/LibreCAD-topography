#ifndef TT_DIALOGPOLYGO_H
#define TT_DIALOGPOLYGO_H

#include <QDialog>
#include <QListWidgetItem>

#include "tt.h"

namespace Ui {
class TT_DialogPolygo;
}

class TT_DialogPolygo : public QDialog
{
    Q_OBJECT

public:
    explicit TT_DialogPolygo(QWidget *parent, QList<TT::Point *> &points);
    ~TT_DialogPolygo();

private slots:
    void fillStationsAndReferences();
    void fillItemsStations();
    void displayItems();

    void calculateAntennaPath();
    void writeDataAfterCalculateAntennaPath();

    void on_listStations_itemDoubleClicked(QListWidgetItem *item);
    void on_pbRight_clicked();
    void on_listPolygo_itemDoubleClicked(QListWidgetItem *item);
    void on_pbLeft_clicked();
    void on_pbUp_clicked();
    void on_pbDown_clicked();
    void on_cbPathType_currentIndexChanged(int index);
    void on_pbCalculate_clicked();
    void on_buttonBox_accepted();

private:
    Ui::TT_DialogPolygo *ui;
    QList<TT::Point *> &points;

    QList<TT::Point*> stations;
    QList<QList<TT::Point*>> references;

    QList<TT::Point*> itemsStations;
    QList<TT::Point*> itemsPolygo;

    // v0s (for each stations of the polygo)
    QList<double> v0s = {};
    // Horizontal distances (for each stations of the polygo, 0 for S1)
    QList<double> hds = {};
    // X (for each stations of the polygo)
    QList<double> xs = {};
    // Y (for each stations of the polygo)
    QList<double> ys = {};
    // Z (for each stations of the polygo)
    QList<double> zs = {};
};

struct OrderedReference
{
    TT::Point* frontReference;
    TT::Point* backReference;
};

#endif // TT_DIALOGPOLYGO_H
