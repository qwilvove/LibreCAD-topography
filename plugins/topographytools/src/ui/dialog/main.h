#ifndef TT_UI_DIALOG_MAIN_H
#define TT_UI_DIALOG_MAIN_H

#include "qc_plugininterface.h"
#include <QDialog>

#include "src/tt.h"

namespace Ui {
class TT_DialogMain;
}

enum class DIALOG
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

    void initMenuBarAndToolbar();

    void loadTtFile();
    void loadPreviousState();

    void displayPoints();
    void displayPoint(TT::Point *point);

    void enableAllTools();

    void addPoint();
    void removePoints(QList<int> &indexesToRemove);
    void editPoint(TT::Point *point);
    void movePointUp(int index);
    void movePointDown(int index);

    int drawPoints();
    void drawPoint(TT::Point *point);

private slots:
    void actionNew();
    void actionOpen();
    void actionSave();
    void actionImport();
    void actionAdd();
    void actionRemove();
    void actionEdit();
    void actionUp();
    void actionDown();
    void actionCalculateV0();
    void actionCalculatePolygonation();
    void actionCalculatePoints();
    void actionDrawPoints();
    void actionDrawBlocks();
    void actionDrawGrid();
    void actionGlobalSettings();
    void actionProjectSettings();
    void tableWidgetCellDoubleClicked(int row, int column);
};

#endif // TT_UI_DIALOG_MAIN_H
