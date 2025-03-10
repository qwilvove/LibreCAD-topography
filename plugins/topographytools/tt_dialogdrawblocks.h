#ifndef TT_DIALOGDRAWBLOCKS_H
#define TT_DIALOGDRAWBLOCKS_H

#include <QDialog>

#include "document_interface.h"

namespace Ui {
class TT_DialogDrawBlocks;
}

class TT_DialogDrawBlocks : public QDialog
{
    Q_OBJECT

public:
    explicit TT_DialogDrawBlocks(QWidget *parent = nullptr, Document_Interface *doc = nullptr);
    ~TT_DialogDrawBlocks();

private:
    Ui::TT_DialogDrawBlocks *ui;
    QWidget *parent;
    Document_Interface *doc;

    void drawBlockOnePoint(QString name, QString layer);
    void drawBlockTwoPoints(QString name, QString layer);
    void drawBlockThreePoints(QString name, QString layer);

private slots:
    void on_tbTree_clicked();

    void on_tbElectricalBox_clicked();
    void on_tbElectricalBox_2points_clicked();
    void on_tbElectricalBox_3points_clicked();
};

#endif // TT_DIALOGDRAWBLOCKS_H
