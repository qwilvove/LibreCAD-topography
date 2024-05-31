#include "tt_dialogdelete.h"
#include "ui_tt_dialogdelete.h"

TT_DialogDelete::TT_DialogDelete(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TT_DialogDelete)
{
    ui->setupUi(this);
}

TT_DialogDelete::~TT_DialogDelete()
{
    delete ui;
}
