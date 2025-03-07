#ifndef TT_DIALOGPOINTS_H
#define TT_DIALOGPOINTS_H

#include <QDialog>

#include "topographytools.h"

namespace Ui {
class TT_DialogPoints;
}

class TT_DialogPoints : public QDialog
{
    Q_OBJECT

public:
    explicit TT_DialogPoints(QWidget *parent, QList<TT::Point> &points);
    ~TT_DialogPoints();

private:
    Ui::TT_DialogPoints *ui;
    QList<TT::Point> &points;
};

#endif // TT_DIALOGPOINTS_H
