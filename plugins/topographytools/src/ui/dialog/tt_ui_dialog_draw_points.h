#ifndef TT_UI_DIALOG_DRAW_POINTS_H
#define TT_UI_DIALOG_DRAW_POINTS_H

#include "qc_plugininterface.h"
#include <QDialog>

#include "src/tt.h"

namespace Ui {
class TT_DialogDrawPoints;
}

class TT_DialogDrawPoints : public QDialog
{
    Q_OBJECT

public:
    explicit TT_DialogDrawPoints(QWidget *parent, Document_Interface *doc, QList<TT::Point*> *points, int *nbPointsDrawn, double scale);
    ~TT_DialogDrawPoints();

private:
    Ui::TT_DialogDrawPoints *ui;
    Document_Interface *doc;
    QList<TT::Point*> *points;
    int *nbPointsDrawn;
    double scale;

    int drawPoints();
    void drawPoint(TT::Point *point);

private slots:
    void slot_validateInputs();
};

#endif // TT_UI_DIALOG_DRAW_POINTS_H
