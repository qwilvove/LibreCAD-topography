#ifndef TT_DIALOGDELETE_H
#define TT_DIALOGDELETE_H

#include <QDialog>

namespace Ui {
class TT_DialogDelete;
}

class TT_DialogDelete : public QDialog
{
    Q_OBJECT

public:
    explicit TT_DialogDelete(QWidget *parent = nullptr);
    ~TT_DialogDelete();

private:
    Ui::TT_DialogDelete *ui;
};

#endif // TT_DIALOGDELETE_H
