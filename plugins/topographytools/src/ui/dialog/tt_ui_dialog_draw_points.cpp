#include "tt_ui_dialog_draw_points.h"
#include "ui_tt_ui_dialog_draw_points.h"

TT_DialogDrawPoints::TT_DialogDrawPoints(QWidget *parent, Document_Interface *doc, QList<TT::Point*> *points, int *nbPointsDrawn, double scale):
    QDialog(parent),
    ui(new Ui::TT_DialogDrawPoints),
    doc(doc),
    points(points),
    nbPointsDrawn(nbPointsDrawn),
    scale(scale)
{
    ui->setupUi(this);
}

TT_DialogDrawPoints::~TT_DialogDrawPoints()
{
    delete ui;
}

// Draw points on the current drawing
int TT_DialogDrawPoints::drawPoints()
{
    int nbPoints = 0;

    // Check if layers already exist
    QStringList layers = this->doc->getAllLayer();

    bool hasTtPointsLayer = layers.contains(TT::LAYERS[TT::LAYER::POINTS].name);
    bool hasTtNameLayer = layers.contains(TT::LAYERS[TT::LAYER::NAME].name);
    bool hasTtAltiLayer = layers.contains(TT::LAYERS[TT::LAYER::ALTI].name);

    // If at least one of the following layers exists, do not draw points
    if (hasTtPointsLayer || hasTtNameLayer || hasTtAltiLayer)
    {
        return -1;
    }

    // Prepare layers
    QString currentLayer = this->doc->getCurrentLayer();

    this->doc->setLayer(TT::LAYERS[TT::LAYER::POINTS].name);
    this->doc->setCurrentLayerProperties(TT::LAYERS[TT::LAYER::POINTS].colour, TT::LAYERS[TT::LAYER::POINTS].lineWidth, TT::LAYERS[TT::LAYER::POINTS].lineType);

    this->doc->setLayer(TT::LAYERS[TT::LAYER::NAME].name);
    this->doc->setCurrentLayerProperties(TT::LAYERS[TT::LAYER::NAME].colour, TT::LAYERS[TT::LAYER::NAME].lineWidth, TT::LAYERS[TT::LAYER::NAME].lineType);

    this->doc->setLayer(TT::LAYERS[TT::LAYER::ALTI].name);
    this->doc->setCurrentLayerProperties(TT::LAYERS[TT::LAYER::ALTI].colour, TT::LAYERS[TT::LAYER::ALTI].lineWidth, TT::LAYERS[TT::LAYER::ALTI].lineType);

    // Draw each point
    for (auto i = 0; i < points->size(); i++)
    {
        TT::Point *currentPoint = points->at(i);
        if (currentPoint->type == TT::Point::TYPE::POINT)
        {
            drawPoint(currentPoint);
            nbPoints++;
        }
    }

    this->doc->setLayer(currentLayer);

    return nbPoints;
}

// Draw a single point on the current drawing
void TT_DialogDrawPoints::drawPoint(TT::Point *point)
{
    this->doc->setLayer(TT::LAYERS[TT::LAYER::POINTS].name);
    QPointF insertionPoint(point->x, point->y);
    this->doc->addPoint(&insertionPoint);

    if (!point->name.isEmpty())
    {
        this->doc->setLayer(TT::LAYERS[TT::LAYER::NAME].name);
        QPointF textInsertionPoint(point->x + 1.0, point->y + 4.0);
        this->doc->addText(point->name, "standard", &textInsertionPoint, 12.0, 0.0, DPI::HAlignLeft, DPI::VAlignTop);
    }

    if (point->hasZ)
    {
        this->doc->setLayer(TT::LAYERS[TT::LAYER::ALTI].name);
        QString text = QString("%1").arg(point->z);
        QPointF textInsertionPoint(point->x + 1.0, point->y - 12.0 - 4.0);
        this->doc->addText(text, "standard", &textInsertionPoint, 12.0, 0.0, DPI::HAlignLeft, DPI::VAlignTop);
    }
}

void TT_DialogDrawPoints::slot_validateInputs()
{
    bool ok = false;

    if (1)
    {
        ok = true;
    }

    if (ok)
    {
        *nbPointsDrawn = drawPoints();
        this->accept();
    }
}
