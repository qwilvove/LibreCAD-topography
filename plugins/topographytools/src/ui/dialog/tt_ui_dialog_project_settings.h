#ifndef TT_UI_DIALOG_PROJECT_SETTINGS_H
#define TT_UI_DIALOG_PROJECT_SETTINGS_H

#include <QDialog>

#include "src/tt.h"

namespace Ui {
class TT_DialogProjectSettings;
}

class TT_DialogProjectSettings : public QDialog
{
    Q_OBJECT

public:
    explicit TT_DialogProjectSettings(QWidget *parent, TT::ProjectSettings *settings);
    ~TT_DialogProjectSettings();

private:
    TT::ProjectSettings *settings;
    Ui::TT_DialogProjectSettings *ui;

    void displayCurrentSettings();
    void saveSettings();
    void setVisibleCustomScaleSettings(bool visible);

private slots:
    void slot_cbScaleIndexChanged(int index);
    void slot_leCustomScaleTextChanged(QString text);

    void slot_validateInputs();
};

#endif // TT_UI_DIALOG_PROJECT_SETTINGS_H
