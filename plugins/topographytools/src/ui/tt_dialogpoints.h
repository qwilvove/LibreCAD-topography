#ifndef TT_DIALOGPOINTS_H
#define TT_DIALOGPOINTS_H

#include <QDialog>

#include "src/tt.h"

namespace Ui {
class TT_DialogPoints;
}

class TT_DialogPoints : public QDialog
{
    Q_OBJECT

public:
    explicit TT_DialogPoints(QWidget *parent, QList<TT::Point *> &points);
    ~TT_DialogPoints();

private slots:
    void identifyStationsAndMeasures();
    void setupCheckBoxes();

    void on_pbSelectAll_clicked();
    void on_pbUnselectAll_clicked();

    void on_pbCalculatePoints_clicked();

    void on_buttonBox_accepted();

private:
    Ui::TT_DialogPoints *ui;
    QList<TT::Point *> &points;

    QList<TT::Point*> stations;
    QList<QList<TT::Point*>> measures;

    QList<QList<TT::Point*>> newPoints;
};

#endif // TT_DIALOGPOINTS_H
