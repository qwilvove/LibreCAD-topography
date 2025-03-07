#ifndef TT_DIALOGEDIT_H
#define TT_DIALOGEDIT_H

#include <QDialog>

#include "topographytools.h"

namespace Ui {
class TT_DialogEdit;
}

class TT_DialogEdit : public QDialog
{
    Q_OBJECT

public:
    explicit TT_DialogEdit(QWidget *parent = nullptr, TT::Point *point = nullptr);
    ~TT_DialogEdit();

private slots:
    void loadData();
    void saveData();
    void on_cbType_currentIndexChanged(int index);
    void on_leName_textChanged(const QString &arg1);
    void on_cbHasZ_stateChanged(int arg1);
    void on_buttonBox_accepted();

private:
    Ui::TT_DialogEdit *ui;
    TT::Point *point;
    TT::Point tempPoint;
};

#endif // TT_DIALOGEDIT_H
