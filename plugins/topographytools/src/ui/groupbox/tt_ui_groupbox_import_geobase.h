#ifndef TT_UI_GROUPBOX_IMPORT_GEOBASE_H
#define TT_UI_GROUPBOX_IMPORT_GEOBASE_H

#include <QGroupBox>

#include "src/tt.h"

namespace Ui {
class TT_GroupBoxImportGeobase;
}

class TT_GroupBoxImportGeobase : public QGroupBox
{
    Q_OBJECT

public:
    explicit TT_GroupBoxImportGeobase(QWidget *parent = nullptr);
    ~TT_GroupBoxImportGeobase();

    QList<TT::Point*> *getNewPoints();

private:
    Ui::TT_GroupBoxImportGeobase *ui;
    QList<TT::Point*> *newPoints;
    QList<QTableWidgetItem*> *twis;

    void loadFilePreview(QTextStream &stream);
    void loadPointsPreview(QTextStream &stream);
    bool loadPointPreview(QString &line);

private slots:
    void slot_tbOpenPressed();
};

#endif // TT_UI_GROUPBOX_IMPORT_GEOBASE_H
