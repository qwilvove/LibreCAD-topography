#ifndef TT_UI_DIALOG_PROJECT_SETTINGS_H
#define TT_UI_DIALOG_PROJECT_SETTINGS_H

#include <QDialog>

namespace Ui {
class TT_DialogProjectSettings;
}

class TT_DialogProjectSettings : public QDialog
{
    Q_OBJECT

public:
    explicit TT_DialogProjectSettings(QWidget *parent = nullptr);
    ~TT_DialogProjectSettings();

private:
    Ui::TT_DialogProjectSettings *ui;

    void setVisibleCustomScaleSettings(bool visible);
    void loadCurrentSettings();
    void saveSettings();

private slots:
    void slot_cbScaleIndexChanged(int index);
    void slot_leCustomScaleTextChanged(QString text);

    void slot_validateInputs();
};

#endif // TT_UI_DIALOG_PROJECT_SETTINGS_H
