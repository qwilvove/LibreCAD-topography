#ifndef TT_DIALOGPOLYGO_H
#define TT_DIALOGPOLYGO_H

#include <QDialog>

#include "topographytools.h"

namespace Ui {
class TT_DialogPolygo;
}

class TT_DialogPolygo : public QDialog
{
    Q_OBJECT

public:
    explicit TT_DialogPolygo(QWidget *parent, QList<TT::Point> &points);
    ~TT_DialogPolygo();

private slots:
    void fillStationsAndReferences();
    void fillItemsStations();
    void displayItems();

    void on_pbRight_clicked();
    void on_pbLeft_clicked();
    void on_pbUp_clicked();
    void on_pbDown_clicked();

private:
    Ui::TT_DialogPolygo *ui;
    QList<TT::Point> &points;

    QList<TT::Point*> stations;
    QList<QList<TT::Point*>> references;

    QList<TT::Point*> itemsStations;
    QList<TT::Point*> itemsPolygo;
};

#endif // TT_DIALOGPOLYGO_H
