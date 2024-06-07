#ifndef TT_DIALOGMAIN_H
#define TT_DIALOGMAIN_H

#include "qc_plugininterface.h"
#include <QDialog>

#include "topographytools.h"

namespace Ui {
class TT_DialogMain;
}

class TT_DialogMain : public QDialog
{
    Q_OBJECT

public:
    explicit TT_DialogMain(QWidget *parent = nullptr, Document_Interface *doc = nullptr);
    ~TT_DialogMain();

private:
    void readSettings();
    void writeSettings();

    int loadPoints();
    void loadPoint(QDataStream &stream, TT::Point &point);
    int savePoints();
    void savePoint(QDataStream &stream, TT::Point &point);
    int importPoints();
    bool importPoint(QString &line, TT::Point &point);

    void displayPoints();
    void displayPoint(TT::Point &point);

    void addPoint();
    void removePoints(QList<int> &indexesToRemove);
    void editPoint(TT::Point &point);
    void movePointUp(int index);
    void movePointDown(int index);

    int drawPoints();
    void drawPoint(TT::Point &point);

    Ui::TT_DialogMain *ui;
    Document_Interface *doc;
    QString fileName;
    QList<TT::Point> points;

private slots:
    void on_pbNew_clicked();
    void on_pbOpen_clicked();
    void on_pbSave_clicked();

    void on_pbImport_clicked();

    void on_pbAdd_clicked();
    void on_pbRemove_clicked();
    void on_pbEdit_clicked();
    void on_pbUp_clicked();
    void on_pbDown_clicked();

    void on_pbV0_clicked();
    void on_pbPolygo_clicked();
    void on_pbDraw_clicked();

    void on_tableWidget_cellDoubleClicked(int row, int column);
};

#endif // TT_DIALOGMAIN_H
