#ifndef TT_MAINWINDOW_H
#define TT_MAINWINDOW_H

#include "document_interface.h"

#include "tt.h"

#include <QMainWindow>

namespace Ui {
class TT_MainWindow;
}

class TT_MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit TT_MainWindow(QWidget *parent = nullptr, Document_Interface *doc = nullptr);
    ~TT_MainWindow();

private:
    Ui::TT_MainWindow *ui;
    Document_Interface *doc;

    QString fileName;
    QList<TT::Point *> points;

    void readSettings();
    void writeSettings();

    int loadPoints();
    void loadPoint(QDataStream &stream, TT::Point *point);
    int savePoints();
    void savePoint(QDataStream &stream, TT::Point *point);
    int importPoints();
    bool importPoint(QString &line, TT::Point *point);

    void displayPoints();
    void displayPoint(TT::Point *point);

    void addPoint();
    void removePoints(QList<int> &indexesToRemove);
    void editPoint(TT::Point *point);
    void movePointUp(int index);
    void movePointDown(int index);

    int drawPoints();
    void drawPoint(TT::Point *point);

private slots:
    void on_actionnew_triggered();
    void on_actionopen_triggered();
    void on_actionsave_triggered();

    void on_actionimport_triggered();

    void on_actionadd_triggered();
    void on_actionremove_triggered();
    void on_actionedit_triggered();
    void on_actionup_triggered();
    void on_actiondown_triggered();

    void on_actionv0_triggered();
    void on_actionpolygo_triggered();
    void on_actionpoints_triggered();

    void on_actiondraw_triggered();

    void on_tableWidget_cellDoubleClicked(int row, int column);
};

#endif // TT_MAINWINDOW_H
