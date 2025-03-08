#include "tt_mainwindow.h"
#include "ui_tt_mainwindow.h"

TT_MainWindow::TT_MainWindow(QWidget *parent, Document_Interface *doc):
    QMainWindow(parent),
    ui(new Ui::TT_MainWindow),
    doc(doc)
{
    ui->setupUi(this);
}

TT_MainWindow::~TT_MainWindow()
{
    delete ui;
}
