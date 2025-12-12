#include "tt_ui_dialog_draw_points.h"
#include "ui_tt_ui_dialog_draw_points.h"

#include <QMessageBox>

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

    QStringList layers = this->doc->getAllLayer();

    if ( pluginSettings->getInsertionLayerPoint() == "" )
    {
        QMessageBox::warning(this, tr("Error!"), tr("Insertion layer for points is not set!\nIt can be set in plugin settings"), QMessageBox::StandardButton::Ok);
        return -1;
    }
    else if (layers.contains(pluginSettings->getInsertionLayerPoint()))
    {
        QMessageBox::warning(this, tr("Error!"), tr("Insertion layer for points (%1) already exists in current document!\nTo redraw points, remove layer \"%1\" from document and draw again").arg(pluginSettings->getInsertionLayerPoint()), QMessageBox::StandardButton::Ok);
        return -1;
    }

    if (ui->ck_addName->isChecked())
    {
        if ( pluginSettings->getInsertionLayerName() == "" )
        {
            QMessageBox::warning(this, tr("Error!"), tr("Insertion layer for points names is not set!\nIt can be set in plugin settings"), QMessageBox::StandardButton::Ok);
            return -1;
        }
        else if (layers.contains(pluginSettings->getInsertionLayerName()))
        {
            QMessageBox::warning(this, tr("Error!"), tr("Insertion layer for points names (%1) already exists in current document!\nTo redraw points, remove layer \"%1\" from document and draw again").arg(pluginSettings->getInsertionLayerName()), QMessageBox::StandardButton::Ok);
            return -1;
        }
    }

    if (ui->ck_addHeight->isChecked())
    {
        if ( pluginSettings->getInsertionLayerAlti() == "" )
        {
            QMessageBox::warning(this, tr("Error!"), tr("Insertion layer for points altitudes is not set!\nIt can be set in plugin settings"), QMessageBox::StandardButton::Ok);
            return -1;
        }
        else if (layers.contains(pluginSettings->getInsertionLayerAlti()))
        {
            QMessageBox::warning(this, tr("Error!"), tr("Insertion layer for points altitudes (%1) already exists in current document!\nTo redraw points, remove layer \"%1\" from document and draw again").arg(pluginSettings->getInsertionLayerAlti()), QMessageBox::StandardButton::Ok);
            return -1;
        }
    }

    // Prepare layers
    QString initialLayer = this->doc->getCurrentLayer();

    TT::Layer *pLayer = pluginSettings->getLayerByName(pluginSettings->getInsertionLayerPoint());
    this->doc->setLayer(pLayer->getName());
    this->doc->setCurrentLayerProperties(pLayer->getColour().rgb(), pLayer->getLineWidth(), pLayer->getLineType());

    if (ui->ck_addName->isChecked())
    {
        TT::Layer *nLayer = pluginSettings->getLayerByName(pluginSettings->getInsertionLayerName());
        this->doc->setLayer(nLayer->getName());
        this->doc->setCurrentLayerProperties(nLayer->getColour().rgb(), nLayer->getLineWidth(), nLayer->getLineType());
    }

    if (ui->ck_addHeight->isChecked())
    {
        TT::Layer *aLayer = pluginSettings->getLayerByName(pluginSettings->getInsertionLayerAlti());
        this->doc->setLayer(aLayer->getName());
        this->doc->setCurrentLayerProperties(aLayer->getColour().rgb(), aLayer->getLineWidth(), aLayer->getLineType());
    }

    // Draw each point
    int nbPoints = 0;
    for (auto i = 0; i < points->size(); i++)
    {
        TT::Point *currentPoint = points->at(i);
        if (currentPoint->getType() == TT::Point::TYPE::POINT)
        {
            drawPoint(currentPoint, pluginSettings->getInsertionLayerPoint(), pluginSettings->getInsertionLayerName(), pluginSettings->getInsertionLayerAlti());
            nbPoints++;
        }
    }

    this->doc->setLayer(initialLayer);

    return nbPoints;
}

// Draw a single point on the current drawing
void TT_DialogDrawPoints::drawPoint(TT::Point *point, QString pPlayerName, QString nLayerName, QString aLayerName)
{
    this->doc->setLayer(pPlayerName);
    QPointF insertionPoint(point->getX(), point->getY());
    this->doc->addPoint(&insertionPoint);

    double fontSize = 12.0 * scale * 100.0; // 12pt for 1:100 scale

    if (ui->ck_addName->isChecked() && !point->getName().isEmpty())
    {
        this->doc->setLayer(nLayerName);
        QPointF textInsertionPoint(point->getX() + fontSize/4, point->getY() + fontSize/3);
        this->doc->addText(point->getName(), "standard", &textInsertionPoint, fontSize, 0.0, DPI::HAlignLeft, DPI::VAlignTop);
    }

    if (ui->ck_addHeight->isChecked() && point->getHasZ())
    {
        this->doc->setLayer(aLayerName);
        QPointF textInsertionPoint(point->getX() + fontSize/4, point->getY() - fontSize - fontSize/3);
        this->doc->addText(QString::number(point->getZ(), 'f', 3), "standard", &textInsertionPoint, fontSize, 0.0, DPI::HAlignLeft, DPI::VAlignTop);
    }
}

void TT_DialogDrawPoints::drawCodes()
{
    if (points->size() <= 0)
    {
        return;
    }

    QList<TT::Layer*> layersToUse;
    for (TT::Code *c : *pluginSettings->getCodes())
    {
        if (!layersToUse.contains(c->getLayer()))
        {
            layersToUse.append(c->getLayer());
        }
    }

    // Check if layers already exist
    QStringList layers = this->doc->getAllLayer();

    for (TT::Layer *l : layersToUse)
    {
        if (layers.contains(l->getName()))
        {
            QMessageBox::warning(this, tr("Error!"), tr("At least one layer used by codes already exist in current document!"), QMessageBox::StandardButton::Ok);
            return;
        }
    }

    // Prepare layer
    QString initialLayer = this->doc->getCurrentLayer();

    for (TT::Layer *l : layersToUse)
    {
        this->doc->setLayer(l->getName());
        this->doc->setCurrentLayerProperties(l->getColour().rgb(), l->getLineWidth(), l->getLineType());
    }

    QPointF firstPointOfTheShape(0.0, 0.0);
    std::vector<QPointF> currentLine;
    std::vector<QPointF> currentArc;
    TT::Code::TYPE currentLineCodeType = TT::Code::TYPE::NONE;
    TT::Code::TYPE lastLineCodeType = TT::Code::TYPE::NONE;

    // Draw each code
    for (auto i = 0; i < points->size(); i++)
    {
        TT::Point *currentPoint = points->at(i);
        TT::Code *currentCode = pluginSettings->getCodeByCode(currentPoint->getCode());

        if (currentPoint->getType() != TT::Point::TYPE::POINT || currentCode == nullptr)
        {
            continue;
        }

        this->doc->setLayer(currentCode->getLayer()->getName());

        QPointF insertionPoint(currentPoint->getX(), currentPoint->getY());

        if (TT::Code::isLineType(currentCode->getType()))
        {
            currentLineCodeType = currentCode->getType();
            drawLineCode(currentLineCodeType, lastLineCodeType, &firstPointOfTheShape, &insertionPoint, &currentLine, &currentArc);
            lastLineCodeType = currentCode->getType();
        }
    }

    this->doc->setLayer(initialLayer);

    return;
}

void TT_DialogDrawPoints::drawLineCode(TT::Code::TYPE currentCodeType, TT::Code::TYPE lastCodeType, QPointF *firstPointOfTheShape, QPointF *insertionPoint, std::vector<QPointF> *currentLine, std::vector<QPointF> *currentArc)
{
    if (currentCodeType == TT::Code::TYPE::LINE_SEGMENT_INIT)
    {
        drawArc(currentArc);
        drawLine(currentLine);
        currentLine->push_back(*insertionPoint);
        *firstPointOfTheShape = *insertionPoint;
    }
    else if (currentCodeType == TT::Code::TYPE::LINE_SEGMENT_CONTINUE)
    {
        if (lastCodeType == TT::Code::TYPE::LINE_ARC_INIT    ||
            lastCodeType == TT::Code::TYPE::LINE_ARC_MIDDLE  ||
            lastCodeType == TT::Code::TYPE::LINE_ARC_CONTINUE)
        {
            currentArc->push_back(*insertionPoint);
            drawArc(currentArc);
        }
        currentLine->push_back(*insertionPoint);
    }
    else if (currentCodeType == TT::Code::TYPE::LINE_ARC_INIT)
    {
        drawArc(currentArc);
        drawLine(currentLine);
        currentArc->push_back(*insertionPoint);
        *firstPointOfTheShape = *insertionPoint;
    }
    else if (currentCodeType == TT::Code::TYPE::LINE_ARC_MIDDLE)
    {
        currentArc->push_back(*insertionPoint);
    }
    else if (currentCodeType == TT::Code::TYPE::LINE_ARC_CONTINUE)
    {
        if (lastCodeType == TT::Code::TYPE::LINE_SEGMENT_INIT    ||
            lastCodeType == TT::Code::TYPE::LINE_SEGMENT_CONTINUE)
        {
            currentLine->push_back(*insertionPoint);
            drawLine(currentLine);
        }
        if (lastCodeType == TT::Code::TYPE::LINE_ARC_INIT    ||
            lastCodeType == TT::Code::TYPE::LINE_ARC_MIDDLE  ||
            lastCodeType == TT::Code::TYPE::LINE_ARC_CONTINUE)
        {
            currentArc->push_back(*insertionPoint);
            drawArc(currentArc);
        }
        currentArc->push_back(*insertionPoint);
    }
    else if (currentCodeType == TT::Code::TYPE::LINE_CLOSE)
    {
        if (lastCodeType == TT::Code::TYPE::LINE_SEGMENT_INIT    ||
            lastCodeType == TT::Code::TYPE::LINE_SEGMENT_CONTINUE)
        {
            currentLine->push_back(*insertionPoint);
        }
        if (lastCodeType == TT::Code::TYPE::LINE_ARC_INIT    ||
            lastCodeType == TT::Code::TYPE::LINE_ARC_MIDDLE  ||
            lastCodeType == TT::Code::TYPE::LINE_ARC_CONTINUE)
        {
            currentArc->push_back(*insertionPoint);
        }
        drawArc(currentArc);
        drawLine(currentLine);
        currentLine->push_back(*firstPointOfTheShape);
        currentLine->push_back(*insertionPoint);
        drawLine(currentLine);
    }
    else if (currentCodeType == TT::Code::TYPE::LINE_END)
    {
        if (lastCodeType == TT::Code::TYPE::LINE_SEGMENT_INIT    ||
            lastCodeType == TT::Code::TYPE::LINE_SEGMENT_CONTINUE)
        {
            currentLine->push_back(*insertionPoint);
        }
        if (lastCodeType == TT::Code::TYPE::LINE_ARC_INIT    ||
            lastCodeType == TT::Code::TYPE::LINE_ARC_MIDDLE  ||
            lastCodeType == TT::Code::TYPE::LINE_ARC_CONTINUE)
        {
            currentArc->push_back(*insertionPoint);
        }
        drawArc(currentArc);
        drawLine(currentLine);
    }
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
