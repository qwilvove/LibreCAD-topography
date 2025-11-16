#ifndef TT_UI_DIALOG_DRAW_BLOCKS_H
#define TT_UI_DIALOG_DRAW_BLOCKS_H

#include <QDialog>
#include <QToolButton>

#include "src/tt.h"

namespace Ui {
class TT_DialogDrawBlocks;
}

class TT_DialogDrawBlocks : public QDialog
{
    Q_OBJECT

public:
    explicit TT_DialogDrawBlocks(QWidget *parent = nullptr, Document_Interface *doc = nullptr);
    ~TT_DialogDrawBlocks();

    void loadPreviousState(int tabIndex, int insertTypeIndex, TT::BLOCK_INSERTION_TYPE insertType);

private:
    Ui::TT_DialogDrawBlocks *ui;
    Document_Interface *doc;

    QList<QList<TT::BLOCK>> blocks;
    QList<QList<QToolButton *>> buttons;
    TT::BLOCK_INSERTION_TYPE currentInsertionType = TT::BLOCK_INSERTION_TYPE::P1;

    void setupUi();
    void updateUi(TT::BLOCK_INSERTION_TYPE insertionType);

    void createBlockIfNotExists(QString file);

    void drawBlockOnePoint(TT::BLOCK block);
    void drawBlockTwoPoints(TT::BLOCK block);
    void drawBlockThreePoints(TT::BLOCK block);
    void drawBlockTwoPointsPlusValue(TT::BLOCK block);

private slots:
    void on_rb1p_clicked();
    void on_rb2p_clicked();
    void on_rb3p_clicked();
    void on_rb2pv_clicked();
};

#endif // TT_UI_DIALOG_DRAW_BLOCKS_H
