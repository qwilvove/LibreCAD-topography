#include "tt_dialogpoints.h"
#include "ui_tt_dialogpoints.h"

TT_DialogPoints::TT_DialogPoints(QWidget *parent, QList<TT::Point> &points) :
    QDialog(parent),
    ui(new Ui::TT_DialogPoints),
    points(points)
{
    ui->setupUi(this);
}

TT_DialogPoints::~TT_DialogPoints()
{
    delete ui;
}
