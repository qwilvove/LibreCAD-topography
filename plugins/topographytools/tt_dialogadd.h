#ifndef TT_DIALOGADD_H
#define TT_DIALOGADD_H

#include <QDialog>

#include "tt.h"

namespace Ui {
class TT_DialogAdd;
}

class TT_DialogAdd : public QDialog
{
    Q_OBJECT

public:
    explicit TT_DialogAdd(QWidget *parent = nullptr, TT::Point *point = nullptr);
    ~TT_DialogAdd();

private slots:
    void saveData();
    void on_cbType_currentIndexChanged(int index);
    void on_leName_textChanged(const QString &arg1);
    void on_cbHasZ_stateChanged(int arg1);
    void on_buttonBox_accepted();

private:
    Ui::TT_DialogAdd *ui;
    TT::Point *point;
    TT::Point tempPoint;
};

#endif // TT_DIALOGADD_H
