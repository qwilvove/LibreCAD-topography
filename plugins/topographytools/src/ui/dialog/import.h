#ifndef TT_UI_DIALOG_IMPORT_H
#define TT_UI_DIALOG_IMPORT_H

#include <QDialog>

#include "src/tt.h"

#include "src/ui/groupbox/tt_ui_groupbox_import_csv.h"
#include "src/ui/groupbox/tt_ui_groupbox_import_geobase.h"
#include "src/ui/groupbox/tt_ui_groupbox_import_gsi.h"

namespace Ui {
    class TT_DialogImport;
}

class TT_DialogImport : public QDialog
{
    Q_OBJECT

public:
    explicit TT_DialogImport(QWidget *parent, QList<TT::Point *> *points, int *nbPointsImported);
    ~TT_DialogImport();

private:
    void initUi();

private:
    QList<TT::Point *> *points;
    int *nbPointsImported;
    Ui::TT_DialogImport *ui;
    TT_GroupBoxImportCsv *gbCsv;
    TT_GroupBoxImportGeobase *gbGeobase;
    TT_GroupBoxImportGsi *gbGsi;

private slots:
    void slot_cbTypeCurrentIndexChanged();

    void slot_validateInputs();
};

#endif // TT_UI_DIALOG_IMPORT_H
