#include "import.h"
#include "ui_import.h"

#include <QMessageBox>

TT_DialogImport::TT_DialogImport(QWidget *parent, QList<TT::Point *> *points, int *nbPointsImported):
    QDialog(parent),
    points(points),
    nbPointsImported(nbPointsImported),
    ui(new Ui::TT_DialogImport)
{
    ui->setupUi(this);

    gbCsv = new TT_GroupBoxImportCsv();
    gbGeobase = new TT_GroupBoxImportGeobase();
    gbGsi = new TT_GroupBoxImportGsi();

    initUi();
}

TT_DialogImport::~TT_DialogImport()
{
    delete ui;

    delete gbCsv;
    delete gbGeobase;
    delete gbGsi;
}

void TT_DialogImport::initUi()
{
    ui->cb_type->addItem(tr("CSV"));
    ui->cb_type->addItem(tr("Geobase"));
    ui->cb_type->addItem(tr("GSI"));

    ui->vl->addWidget(gbCsv);
    ui->vl->addWidget(gbGeobase);
    ui->vl->addWidget(gbGsi);

    ui->cb_type->setCurrentIndex(0);
}

void TT_DialogImport::slot_cbTypeCurrentIndexChanged()
{
    gbCsv->hide();
    gbGeobase->hide();
    gbGsi->hide();

    switch (ui->cb_type->currentIndex()) {
    case 0:
        gbCsv->show();
        break;
    case 1:
        gbGeobase->show();
        break;
    case 2:
        gbGsi->show();
        break;
    default:
        break;
    }
}

void TT_DialogImport::slot_validateInputs()
{
    bool ok = false;

    switch (ui->cb_type->currentIndex()) {
    case 0:
        if (gbCsv->getNewPoints()->size() > 0)
        {
            for (TT::Point *p : *gbCsv->getNewPoints())
            {
                points->append(p);
            }
            *nbPointsImported = gbCsv->getNewPoints()->size();
            ok = true;
        }
        else
        {
            QMessageBox::warning(this, tr("Error!"), tr("No point to import!"));
        }
        break;
    case 1:
        if (gbGeobase->getNewPoints()->size() > 0)
        {
            for (TT::Point *p : *gbGeobase->getNewPoints())
            {
                points->append(p);
            }
            *nbPointsImported = gbGeobase->getNewPoints()->size();
            ok = true;
        }
        else
        {
            QMessageBox::warning(this, tr("Error!"), tr("No point to import!"));
        }
        break;
    case 2:
        if (gbGsi->getNewPoints()->size() > 0)
        {
            for (TT::Point *p : *gbGsi->getNewPoints())
            {
                points->append(p);
            }
            *nbPointsImported = gbGsi->getNewPoints()->size();
            ok = true;
        }
        else
        {
            QMessageBox::warning(this, tr("Error!"), tr("No point to import!"));
        }
        break;
    default:
        break;
    }

    if (ok)
    {
        this->accept();
    }
}
