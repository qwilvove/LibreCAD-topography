#ifndef TT_UI_DIALOG_CODE_ADD_H
#define TT_UI_DIALOG_CODE_ADD_H

#include <QDialog>

#include "src/tt.h"

namespace Ui {
class TT_DialogCodeAdd;
}

class TT_DialogCodeAdd : public QDialog
{
    Q_OBJECT

public:
    explicit TT_DialogCodeAdd(QWidget *parent, TT::PluginSettings *settings);
    ~TT_DialogCodeAdd();

private:
    TT::PluginSettings *settings;
    Ui::TT_DialogCodeAdd *ui;

    void initUi();
    bool codeAlreadyExists(QString code);

private slots:
    void slot_cbTypeCurrentIndexChanged();

    void slot_validateInputs();
};

#endif // TT_UI_DIALOG_CODE_ADD_H
