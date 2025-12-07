#ifndef TT_UI_DIALOG_LAYER_EDIT_H
#define TT_UI_DIALOG_LAYER_EDIT_H

#include <QDialog>

#include "src/tt.h"

namespace Ui {
class TT_DialogLayerEdit;
}

class TT_DialogLayerEdit : public QDialog
{
    Q_OBJECT

public:
    explicit TT_DialogLayerEdit(QWidget *parent, TT::PluginSettings *settings, int index);
    ~TT_DialogLayerEdit();

private:
    TT::PluginSettings *settings;
    int index;
    QString initialName;
    Ui::TT_DialogLayerEdit *ui;

    void initUi();
    void loadLayerData();
    bool nameAlreadyExists(QString name);

private slots:
    void slot_leColourTextChanged();

    void slot_validateInputs();
};

#endif // TT_UI_DIALOG_LAYER_EDIT_H
