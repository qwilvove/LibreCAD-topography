#ifndef TT_DIALOGV0_H
#define TT_DIALOGV0_H

#include <QDialog>
#include <QCheckBox>

#include "topographytools.h"

namespace Ui {
class TT_DialogV0;
}

class TT_DialogV0 : public QDialog
{
    Q_OBJECT

public:
    explicit TT_DialogV0(QWidget *parent, QList<TT::Point> &points);
    ~TT_DialogV0();

private slots:
    void identifyStationsAndReferences();
    void setupComboBox();
    void setupCheckBoxes(int index);
    void clearCheckBoxes();

    void on_cbStation_currentIndexChanged(int index);
    void on_pbCalculate_clicked();
    void on_buttonBox_accepted();

private:
    Ui::TT_DialogV0 *ui;
    QList<TT::Point> &points;

    QList<TT::Point*> stations;
    QList<QList<TT::Point*>> references;

    TT::Point *currentStation;
    QList<TT::Point*> currentReferences;

    QList<TT::Point*> checkedReferences;

    QList<QCheckBox*> checkBoxes; // Checkboxes currently displayed to the user
};

#endif // TT_DIALOGV0_H
