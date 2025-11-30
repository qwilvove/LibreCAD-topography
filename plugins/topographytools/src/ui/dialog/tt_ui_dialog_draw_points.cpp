#include "tt_ui_dialog_draw_points.h"
#include "ui_tt_ui_dialog_draw_points.h"

TT_DialogDrawPoints::TT_DialogDrawPoints(QWidget *parent, Document_Interface *doc, TT::PluginSettings *pluginSettings, QList<TT::Point*> *points, int *nbPointsDrawn, double scale):
    QDialog(parent),
    ui(new Ui::TT_DialogDrawPoints),
    doc(doc),
    pluginSettings(pluginSettings),
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
    if (points->size() <= 0)
    {
        return 0;
    }

    // Check if layers already exist
    QStringList layers = this->doc->getAllLayer();

    bool hasTtPointsLayer = layers.contains(TT::LAYERS[TT::LAYER::POINTS].name);
    bool hasTtNameLayer = layers.contains(TT::LAYERS[TT::LAYER::NAME].name);
    bool hasTtAltiLayer = layers.contains(TT::LAYERS[TT::LAYER::ALTI].name);

    // If at least one of the required layers exists, do not draw points
    if (hasTtPointsLayer || (hasTtNameLayer && ui->ck_addName->isChecked()) || (hasTtAltiLayer && ui->ck_addHeight->isChecked()))
    {
        return -1;
    }

    // Prepare layers
    QString initialLayer = this->doc->getCurrentLayer();

    this->doc->setLayer(TT::LAYERS[TT::LAYER::POINTS].name);
    this->doc->setCurrentLayerProperties(TT::LAYERS[TT::LAYER::POINTS].colour, TT::LAYERS[TT::LAYER::POINTS].lineWidth, TT::LAYERS[TT::LAYER::POINTS].lineType);

    if (ui->ck_addName->isChecked())
    {
        this->doc->setLayer(TT::LAYERS[TT::LAYER::NAME].name);
        this->doc->setCurrentLayerProperties(TT::LAYERS[TT::LAYER::NAME].colour, TT::LAYERS[TT::LAYER::NAME].lineWidth, TT::LAYERS[TT::LAYER::NAME].lineType);
    }

    if (ui->ck_addHeight->isChecked())
    {
        this->doc->setLayer(TT::LAYERS[TT::LAYER::ALTI].name);
        this->doc->setCurrentLayerProperties(TT::LAYERS[TT::LAYER::ALTI].colour, TT::LAYERS[TT::LAYER::ALTI].lineWidth, TT::LAYERS[TT::LAYER::ALTI].lineType);
    }

    // Draw each point
    int nbPoints = 0;
    for (auto i = 0; i < points->size(); i++)
    {
        TT::Point *currentPoint = points->at(i);
        if (currentPoint->type == TT::Point::TYPE::POINT)
        {
            drawPoint(currentPoint);
            nbPoints++;
        }
    }

    this->doc->setLayer(initialLayer);

    return nbPoints;
}

// Draw a single point on the current drawing
void TT_DialogDrawPoints::drawPoint(TT::Point *point)
{
    this->doc->setLayer(TT::LAYERS[TT::LAYER::POINTS].name);
    QPointF insertionPoint(point->x, point->y);
    this->doc->addPoint(&insertionPoint);

    double fontSize = 12.0 * scale * 100.0; // 12pt for 1:100 scale

    if (ui->ck_addName->isChecked() && !point->name.isEmpty())
    {
        this->doc->setLayer(TT::LAYERS[TT::LAYER::NAME].name);
        QPointF textInsertionPoint(point->x + 1.0, point->y + fontSize/3);
        this->doc->addText(point->name, "standard", &textInsertionPoint, fontSize, 0.0, DPI::HAlignLeft, DPI::VAlignTop);
    }

    if (ui->ck_addHeight->isChecked() && point->hasZ)
    {
        this->doc->setLayer(TT::LAYERS[TT::LAYER::ALTI].name);
        QString text = QString("%1").arg(point->z);
        QPointF textInsertionPoint(point->x + 1.0, point->y - fontSize - fontSize/3);
        this->doc->addText(text, "standard", &textInsertionPoint, fontSize, 0.0, DPI::HAlignLeft, DPI::VAlignTop);
    }
}

void TT_DialogDrawPoints::drawCodes()
{
    if (points->size() <= 0)
    {
        return;
    }

    // Check if layer already exists
    QStringList layers = this->doc->getAllLayer();

    TT::LayerProperties linesLayerProperties = TT::LAYERS[TT::LAYER::LINES];

    bool hasTtLinesLayer = layers.contains(linesLayerProperties.name);

    // If the required layer exists, do not draw codes
    if (hasTtLinesLayer)
    {
        return;
    }

    // Prepare layer
    QString initialLayer = this->doc->getCurrentLayer();

    this->doc->setLayer(linesLayerProperties.name);
    this->doc->setCurrentLayerProperties(linesLayerProperties.colour, linesLayerProperties.lineWidth, linesLayerProperties.lineType);

    QPointF firstPointOfTheShape(0.0, 0.0);
    std::vector<QPointF> currentLine;
    std::vector<QPointF> currentArc;
    TT::PluginSettings::CODE lastCode = TT::PluginSettings::CODE::NONE;

    // Draw each code
    for (auto i = 0; i < points->size(); i++)
    {
        TT::Point *currentPoint = points->at(i);
        if (currentPoint->type == TT::Point::TYPE::POINT)
        {
            QPointF insertionPoint(currentPoint->x, currentPoint->y);
            TT::PluginSettings::CODE currentCode = pluginSettings->getCode(currentPoint->code);

            if (currentCode == TT::PluginSettings::CODE::LINE_INIT)
            {
                drawArc(&currentArc);
                drawLine(&currentLine);
                currentLine.push_back(insertionPoint);
                firstPointOfTheShape = insertionPoint;
            }
            else if (currentCode == TT::PluginSettings::CODE::LINE_CONTINUE)
            {
                if (lastCode == TT::PluginSettings::CODE::ARC_INIT ||
                    lastCode == TT::PluginSettings::CODE::ARC_MIDDLE ||
                    lastCode == TT::PluginSettings::CODE::ARC_CONTINUE)
                {
                    currentArc.push_back(insertionPoint);
                    drawArc(&currentArc);
                }
                currentLine.push_back(insertionPoint);
            }
            else if (currentCode == TT::PluginSettings::CODE::ARC_INIT)
            {
                drawArc(&currentArc);
                drawLine(&currentLine);
                currentArc.push_back(insertionPoint);
                firstPointOfTheShape = insertionPoint;
            }
            else if (currentCode == TT::PluginSettings::CODE::ARC_MIDDLE)
            {
                currentArc.push_back(insertionPoint);
            }
            else if (currentCode == TT::PluginSettings::CODE::ARC_CONTINUE)
            {
                if (lastCode == TT::PluginSettings::CODE::LINE_INIT ||
                    lastCode == TT::PluginSettings::CODE::LINE_CONTINUE)
                {
                    currentLine.push_back(insertionPoint);
                    drawLine(&currentLine);
                }
                if (lastCode == TT::PluginSettings::CODE::ARC_INIT ||
                    lastCode == TT::PluginSettings::CODE::ARC_MIDDLE ||
                    lastCode == TT::PluginSettings::CODE::ARC_CONTINUE)
                {
                    currentArc.push_back(insertionPoint);
                    drawArc(&currentArc);
                }
                currentArc.push_back(insertionPoint);
            }
            else if (currentCode == TT::PluginSettings::CODE::CLOSE)
            {
                if (lastCode == TT::PluginSettings::CODE::LINE_INIT ||
                    lastCode == TT::PluginSettings::CODE::LINE_CONTINUE)
                {
                    currentLine.push_back(insertionPoint);
                }
                if (lastCode == TT::PluginSettings::CODE::ARC_INIT ||
                    lastCode == TT::PluginSettings::CODE::ARC_MIDDLE ||
                    lastCode == TT::PluginSettings::CODE::ARC_CONTINUE)
                {
                    currentArc.push_back(insertionPoint);
                }
                drawArc(&currentArc);
                drawLine(&currentLine);
                currentLine.push_back(firstPointOfTheShape);
                currentLine.push_back(insertionPoint);
                drawLine(&currentLine);
            }
            else if (currentCode == TT::PluginSettings::CODE::END)
            {
                if (lastCode == TT::PluginSettings::CODE::LINE_INIT ||
                    lastCode == TT::PluginSettings::CODE::LINE_CONTINUE)
                {
                    currentLine.push_back(insertionPoint);
                }
                if (lastCode == TT::PluginSettings::CODE::ARC_INIT ||
                    lastCode == TT::PluginSettings::CODE::ARC_MIDDLE ||
                    lastCode == TT::PluginSettings::CODE::ARC_CONTINUE)
                {
                    currentArc.push_back(insertionPoint);
                }
                drawArc(&currentArc);
                drawLine(&currentLine);
            }

            lastCode = currentCode;
        }
    }

    this->doc->setLayer(initialLayer);

    return;
}

void TT_DialogDrawPoints::drawLine(std::vector<QPointF> *points)
{
    if (points->size() < 2)
    {
        return;
    }

    doc->addLines(*points);

    points->clear();
}

void TT_DialogDrawPoints::drawArc(std::vector<QPointF> *points)
{
    if (points->size() < 3)
    {
        return;
    }

    doc->addArcFrom3P(points->at(0), points->at(1), points->at(2));

    points->clear();
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
        if (ui->ck_useCodes->isChecked())
        {
            drawCodes();
        }
        this->accept();
    }
}
