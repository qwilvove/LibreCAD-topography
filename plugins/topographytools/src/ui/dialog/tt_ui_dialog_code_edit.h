#ifndef TT_UI_DIALOG_CODE_EDIT_H
#define TT_UI_DIALOG_CODE_EDIT_H

#include <QDialog>

#include "src/tt.h"

namespace Ui {
class TT_DialogCodeEdit;
}

class TT_DialogCodeEdit : public QDialog
{
    Q_OBJECT

public:
    explicit TT_DialogCodeEdit(QWidget *parent, TT::PluginSettings *settings, int index);
    ~TT_DialogCodeEdit();

private:
    TT::PluginSettings *settings;
    int index;
    QString initialCode;
    Ui::TT_DialogCodeEdit *ui;

    void initUi();
    void loadCodeData();
    bool codeAlreadyExists(QString code);

private slots:
    void slot_cbTypeCurrentIndexChanged();

    void slot_validateInputs();
};

#endif // TT_UI_DIALOG_CODE_EDIT_H
