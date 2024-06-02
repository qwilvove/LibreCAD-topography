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
    explicit TT_DialogV0(QWidget *parent = nullptr, QList<TT::Point> *points = nullptr);
    ~TT_DialogV0();

private slots:
    void identifyStationsAndReferences();
    void setupComboBox();
    void setupCheckBoxes(int index);
    void clearCheckBoxes();
    //void testCheckBoxes(bool state);

    void on_cbStation_currentIndexChanged(int index);

private:
    Ui::TT_DialogV0 *ui;
    QList<TT::Point> *points;
    QList<TT::Point> stationList;
    QList<QList<TT::Point>> referencesList;
    QList<QCheckBox*> checkBoxes; // Checkboxes currently displayed to the user
};

#endif // TT_DIALOGV0_H
