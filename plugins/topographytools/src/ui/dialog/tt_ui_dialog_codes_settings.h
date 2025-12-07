#ifndef TT_UI_DIALOG_CODES_SETTINGS_H
#define TT_UI_DIALOG_CODES_SETTINGS_H

#include <QDialog>

#include "src/tt.h"

namespace Ui {
class TT_DialogCodesSettings;
}

class TT_DialogCodesSettings : public QDialog
{
    Q_OBJECT

public:
    explicit TT_DialogCodesSettings(QWidget *parent, TT::PluginSettings *settings);
    ~TT_DialogCodesSettings();

private:
    TT::PluginSettings *settings;
    Ui::TT_DialogCodesSettings *ui;

    void displayCodes();

private slots:
    void slot_addCode();
    void slot_removeCode();
    void slot_editCode();
};

#endif // TT_UI_DIALOG_CODES_SETTINGS_H
