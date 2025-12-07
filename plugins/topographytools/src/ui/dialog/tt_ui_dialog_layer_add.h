#ifndef TT_UI_DIALOG_LAYER_ADD_H
#define TT_UI_DIALOG_LAYER_ADD_H

#include <QDialog>

#include "src/tt.h"

namespace Ui {
class TT_DialogLayerAdd;
}

class TT_DialogLayerAdd : public QDialog
{
    Q_OBJECT

public:
    explicit TT_DialogLayerAdd(QWidget *parent, TT::PluginSettings *settings);
    ~TT_DialogLayerAdd();

private:
    TT::PluginSettings *settings;
    Ui::TT_DialogLayerAdd *ui;

    void initUi();
    bool nameAlreadyExists(QString name);

private slots:
    void slot_leColourTextChanged();

    void slot_validateInputs();
};

#endif // TT_UI_DIALOG_LAYER_ADD_H
