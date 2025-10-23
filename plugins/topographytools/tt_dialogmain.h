#ifndef TT_DIALOGMAIN_H
#define TT_DIALOGMAIN_H

#include "qc_plugininterface.h"
#include <QDialog>

#include "tt.h"

namespace Ui {
class TT_DialogMain;
}

enum class ACTION_GROUP
{
    FILE,
    IMPORT,
    EDIT,
    CALC,
    DRAW
};

enum class ACTION
{
    NEW,
    OPEN,
    SAVE,

    IMPORT,

    ADD,
    REMOVE,
    EDIT,
    UP,
    DOWN,

    CALC_V0,
    CALC_POLYGO,
    CALC_POINTS,

    DRAW_POINTS,
    DRAW_BLOCKS,
    DRAW_GRID
};

struct Action
{
    QString name;
    QString tooltip;
    QString iconName;
    QString shortcut;
};

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
    const QMap<ACTION, Action> ACTIONS = {
        { ACTION::NEW,         { tr("New"),         tr("New .tt file (Ctrl+N)"),            "new",                            "Ctrl+N" } },
        { ACTION::OPEN,        { tr("Open"),        tr("Open a .tt file (Ctrl+O)"),         "open",                           "Ctrl+O" } },
        { ACTION::SAVE,        { tr("Save"),        tr("Save the .tt file (Ctrl+S)"),       "save",                           "Ctrl+S" } },

        { ACTION::IMPORT,      { tr("Import"),      tr("Import data from a file (Ctrl+I)"), "import",                         "Ctrl+I" } },

        { ACTION::ADD,         { tr("Add"),         tr("Add a line (Ctrl++)"),              "add",                            "Ctrl++" } },
        { ACTION::REMOVE,      { tr("Remove"),      tr("Remove a line (Ctrl+-)"),           "remove",                         "Del"    } },
        { ACTION::EDIT,        { tr("Edit"),        tr("Edit attributes (Ctrl+E)"),         "attributes",                     "Ctrl+E" } },
        { ACTION::UP,          { tr("Up"),          tr("Move a line up (PgUp)"),            "up",                             "PgUp"   } },
        { ACTION::DOWN,        { tr("Down"),        tr("Move a line down (PgDown)"),        "down",                           "PgDown" } },

        { ACTION::CALC_V0,     { tr("V0"),          tr("Calculate V0"),                     "angle_line_to_line",             ""       } },
        { ACTION::CALC_POLYGO, { tr("Polygo"),      tr("Calculate polygonation"),           "total_length_selected_entities", ""       } },
        { ACTION::CALC_POINTS, { tr("Points"),      tr("Calculate points"),                 "points",                         ""       } },

        { ACTION::DRAW_POINTS, { tr("Draw points"), tr("Draw points"),                      "draw_points",                    ""       } },
        { ACTION::DRAW_BLOCKS, { tr("Draw blocks"), tr("Draw blocks"),                      "draw_blocks",                    ""       } },
        { ACTION::DRAW_GRID,   { tr("Draw grid"),   tr("Draw grid"),                        "grid",                           ""       } }
    };
    const QMap<ACTION_GROUP, QList<ACTION>> ACTION_GROUPS = {
        { ACTION_GROUP::FILE,   { ACTION::NEW, ACTION::OPEN, ACTION::SAVE                             } },
        { ACTION_GROUP::IMPORT, { ACTION::IMPORT                                                      } },
        { ACTION_GROUP::EDIT,   { ACTION::ADD, ACTION::REMOVE, ACTION::EDIT, ACTION::UP, ACTION::DOWN } },
        { ACTION_GROUP::CALC,   { ACTION::CALC_V0, ACTION::CALC_POLYGO, ACTION::CALC_POINTS           } },
        { ACTION_GROUP::DRAW,   { ACTION::DRAW_POINTS, ACTION::DRAW_BLOCKS, ACTION::DRAW_GRID         } }
    };

    void initMenuBarAndToolbar();

    void readSettings();
    void writeSettings();

    void loadPreviousState();

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

    void executeAction(ACTION action);
    void actionNew();
    void actionOpen();
    void actionSave();
    void actionImport();
    void actionAdd();
    void actionRemove();
    void actionEdit();
    void actionUp();
    void actionDown();
    void actionV0();
    void actionPolygo();
    void actionPoints();
    void actionDraw();
    void actionDrawBlocks();
    void actionGrid();

private slots:
    void on_tableWidget_cellDoubleClicked(int row, int column);
};

#endif // TT_DIALOGMAIN_H
