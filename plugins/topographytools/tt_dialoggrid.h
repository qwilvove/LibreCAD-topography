#ifndef TT_DIALOGGRID_H
#define TT_DIALOGGRID_H

#include <QDialog>

#include "tt.h"

namespace Ui {
class TT_DialogGrid;
}

class TT_DialogGrid : public QDialog
{
    Q_OBJECT

public:
    explicit TT_DialogGrid(QWidget *parent = nullptr, Document_Interface *doc = nullptr);
    ~TT_DialogGrid();

private:
    Ui::TT_DialogGrid *ui;
    QWidget *parent;
    Document_Interface *doc;

private slots:
    void on_pbInsertGrid_clicked();
    void on_pbMoveGrid_clicked();
    void on_pbRotateGrid_clicked();
    void on_pbPreparePrint_clicked();
    void on_pbRestoreDrawing_clicked();
};

#endif // TT_DIALOGGRID_H
