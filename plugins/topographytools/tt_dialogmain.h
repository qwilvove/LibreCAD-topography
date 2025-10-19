#ifndef TT_DIALOGMAIN_H
#define TT_DIALOGMAIN_H

#include "qc_plugininterface.h"
#include <QDialog>

#include "tt.h"

namespace Ui {
class TT_DialogMain;
}

enum DIALOG
{
    NONE,
    DRAW_BLOCKS,
    DRAW_GRID
};

struct State
{
    DIALOG dialog;
    // If DRAW_BLOCKS
    int tabIndex;
    int insertTypeIndex;
    TT::BLOCK_INSERTION_TYPE insertType;
};

class TT_DialogMain : public QDialog
{
    Q_OBJECT

public:
    explicit TT_DialogMain(QWidget *parent = nullptr, Document_Interface *doc = nullptr);
    ~TT_DialogMain();

    bool isRunning;

    void savePreviousState(DIALOG dialog = DIALOG::NONE, int tabIndex = 0, int insertTypeIndex = 0, TT::BLOCK_INSERTION_TYPE insertType = TT::BLOCK_INSERTION_TYPE::P1);

protected:
    void showEvent(QShowEvent *event) override;

private:
    Ui::TT_DialogMain *ui;
    Document_Interface *doc;
    QString fileName;
    QList<TT::Point*> points;

    State previousState;

    void readSettings();
    void writeSettings();

    int loadPoints();
    void loadPoint(QDataStream &stream, TT::Point *point);
    int savePoints();
    void savePoint(QDataStream &stream, TT::Point *point);

    void displayPoints();
    void displayPoint(TT::Point *point);

    void enableAllTools();

    void importPoints();
    void addPoint();
    void removePoints(QList<int> &indexesToRemove);
    void editPoint(TT::Point *point);
    void movePointUp(int index);
    void movePointDown(int index);

    int drawPoints();
    void drawPoint(TT::Point *point);

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
    void on_pbPoints_clicked();

    void on_pbDraw_clicked();
    void on_pbDrawBlocks_clicked();
    void on_pbGrid_clicked();

    void on_tableWidget_cellDoubleClicked(int row, int column);

    void loadPreviousState();
};

#endif // TT_DIALOGMAIN_H
