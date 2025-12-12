#ifndef TT_UI_GROUPBOX_IMPORT_CSV_H
#define TT_UI_GROUPBOX_IMPORT_CSV_H

#include <QGroupBox>

#include "src/tt.h"

namespace Ui {
class TT_GroupBoxImportCsv;
}

class TT_GroupBoxImportCsv : public QGroupBox
{
    Q_OBJECT

public:
    explicit TT_GroupBoxImportCsv(QWidget *parent = nullptr);
    ~TT_GroupBoxImportCsv();

    QList<TT::Point*> *getNewPoints();

private:
    Ui::TT_GroupBoxImportCsv *ui;
    QList<TT::Point*> *newPoints;
    QList<QTableWidgetItem*> *twis;

    void loadFilePreview(QTextStream &stream);
    void loadPointsPreview(QTextStream &stream);
    bool loadPointPreview(QString &line);

private slots:
    void slot_tbOpenPressed();
};

#endif // TT_UI_GROUPBOX_IMPORT_CSV_H
