#ifndef TT_UI_DIALOG_GLOBAL_SETTINGS_H
#define TT_UI_DIALOG_GLOBAL_SETTINGS_H

#include <QDialog>

namespace Ui {
class TT_DialogGlobalSettings;
}

class TT_DialogGlobalSettings : public QDialog
{
    Q_OBJECT

public:
    explicit TT_DialogGlobalSettings(QWidget *parent = nullptr);
    ~TT_DialogGlobalSettings();

private:
    Ui::TT_DialogGlobalSettings *ui;
};

#endif // TT_UI_DIALOG_GLOBAL_SETTINGS_H
