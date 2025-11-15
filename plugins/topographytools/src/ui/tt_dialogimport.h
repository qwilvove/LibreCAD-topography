#ifndef TT_DIALOGIMPORT_H
#define TT_DIALOGIMPORT_H

#include <QDialog>
#include <QTextStream>

#include "src/tt.h"

namespace Ui {
    class TT_DialogImport;
}

class TT_DialogImport : public QDialog
{
    Q_OBJECT

public:
    explicit TT_DialogImport(QWidget *parent, QList<TT::Point *> &points, int &nbPointsImported);
    ~TT_DialogImport();

private:
    void loadCsvFilePreview(QTextStream &stream);
    void loadCsvPointsPreview(QTextStream &stream);
    bool loadCsvPointPreview(QString &line);

    int importPointsFromCsv();
    //bool importPointFromCsv(QString &line, TT::Point *point);

    void loadGeobaseFilePreview(QTextStream &stream);
    void loadGeobasePointsPreview(QTextStream &stream);
    bool loadGeobasePointPreview(QString &line);

    int importPointsFromGeobase();
    //bool importPointFromGeobase(QString &line, TT::Point *point);

private slots:
    void on_comboBox_currentIndexChanged(int index);
    void on_tbOpenCsv_clicked();
    void on_tbOpenGeobase_clicked();
    void on_buttonBox_accepted();

private:
    Ui::TT_DialogImport *ui;
    QList<TT::Point *> &points;
    QList<TT::Point *> newPointsFromCsv;
    QList<TT::Point *> newPointsFromGeobase;
    int &nbPointsImported;
};

#endif // TT_DIALOGIMPORT_H
