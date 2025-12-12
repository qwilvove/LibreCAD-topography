#ifndef TT_UI_GROUPBOX_IMPORT_GSI_H
#define TT_UI_GROUPBOX_IMPORT_GSI_H

#include <QGroupBox>

#include "src/tt.h"

namespace Ui {
class TT_GroupBoxImportGsi;
}

class TT_GroupBoxImportGsi : public QGroupBox
{
    Q_OBJECT

public:
    explicit TT_GroupBoxImportGsi(QWidget *parent = nullptr);
    ~TT_GroupBoxImportGsi();

    QList<TT::Point*> *getNewPoints();

private:
    Ui::TT_GroupBoxImportGsi *ui;
    QList<TT::Point*> *newPoints;
    QList<QTableWidgetItem*> *twis;

    void detectGsiFormat(QTextStream &stream);
    void loadFilePreview(QTextStream &stream);
    void loadPointsPreview(QTextStream &stream);
    bool loadPointPreview(QString line);
    double convertToDouble(QString str);

private slots:
    void slot_tbOpenPressed();
};

#endif // TT_UI_GROUPBOX_IMPORT_GSI_H
