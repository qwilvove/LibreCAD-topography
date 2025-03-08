#ifndef TT_MAINWINDOW_H
#define TT_MAINWINDOW_H

#include "qc_plugininterface.h"

#include <QMainWindow>

namespace Ui {
class TT_MainWindow;
}

class TT_MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit TT_MainWindow(QWidget *parent, Document_Interface *doc);
    ~TT_MainWindow();

private:
    Ui::TT_MainWindow *ui;
    Document_Interface *doc;

};

#endif // TT_MAINWINDOW_H
