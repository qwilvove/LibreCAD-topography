#include "tt_dialogdrawblocks.h"
#include "ui_tt_dialogdrawblocks.h"

#include <QTemporaryDir>

TT_DialogDrawBlocks::TT_DialogDrawBlocks(QWidget *parent, Document_Interface *doc):
    ui(new Ui::TT_DialogDrawBlocks),
    parent(parent),
    doc(doc)
{
    ui->setupUi(this);
}

TT_DialogDrawBlocks::~TT_DialogDrawBlocks()
{
    delete ui;
}

void TT_DialogDrawBlocks::createBlockIfNotExists(QString name)
{
    // Extract block from ressources
    QTemporaryDir tempDir;
    if (tempDir.isValid())
    {
        const QString tempFile = tempDir.path() + "/" + name + ".dxf";
        if (QFile::copy(":/blocks/" + name + ".dxf", tempFile))
        {
            // Only create the block if it does not exist
            bool blockExists = false;
            foreach (QString blockName, this->doc->getAllBlocks())
            {
                if ( blockName == name )
                {
                    blockExists = true;
                    break;
                }
            }
            if ( !blockExists )
            {
                this->doc->addBlockfromFromdisk(tempFile);
            }
        }
    }
}

void TT_DialogDrawBlocks::drawBlockOnePoint(QString name, QString layer)
{
    this->close();
    this->parent->close();

    QString initialLayer = this->doc->getCurrentLayer();

    this->doc->setLayer(layer);

    createBlockIfNotExists(name);

    QPointF *point = new QPointF();
    QPointF *scale = new QPointF(1,1);
    while ( this->doc->getPoint(point) == true )
    {
        this->doc->addInsert(name, *point, *scale, 0);
        this->doc->updateView();
    }

    this->doc->setLayer(initialLayer);
}

void TT_DialogDrawBlocks::drawBlockTwoPoints(QString name, QString layer)
{
    this->close();
    this->parent->close();

    QString initialLayer = this->doc->getCurrentLayer();

    this->doc->setLayer(layer);

    createBlockIfNotExists(name);

    QPointF *point1 = new QPointF();
    QPointF *point2 = new QPointF();

    while ( this->doc->getPoint(point1) == true && this->doc->getPoint(point2) == true )
    {
        double distance = std::sqrt( std::pow(point2->x() - point1->x(), 2.0) + std::pow(point2->y() - point1->y(), 2.0) );
        if (distance > 0)
        {
            QPointF *scale = new QPointF(distance,distance);
            qreal rotation = std::atan2( point2->y() - point1->y(), point2->x() - point1->x() );

            this->doc->addInsert(name, *point1, *scale, rotation);
            this->doc->updateView();
        }
    }

    this->doc->setLayer(initialLayer);
}

void TT_DialogDrawBlocks::drawBlockThreePoints(QString name, QString layer)
{
    this->close();
    this->parent->close();

    QString initialLayer = this->doc->getCurrentLayer();

    this->doc->setLayer(layer);

    createBlockIfNotExists(name);

    QPointF *point1 = new QPointF();
    QPointF *point2 = new QPointF();
    QPointF *point3 = new QPointF();

    while ( this->doc->getPoint(point1) == true && this->doc->getPoint(point2) == true && this->doc->getPoint(point3) == true )
    {
        double distance = std::sqrt( std::pow(point2->x() - point1->x(), 2.0) + std::pow(point2->y() - point1->y(), 2.0) );

        if (distance > 0)
        {
            QPointF *vector1 = new QPointF( point2->x() - point1->x(), point2->y() - point1->y() );
            QPointF *vector2 = new QPointF( point3->x() - point1->x(), point3->y() - point1->y() );
            double innerProduct = vector1->x() * vector2->x() + vector1->y() * vector2->y();
            double magnitudePow2 = std::pow( vector1->x(), 2.0 ) + std::pow( vector1->y(), 2.0 );
            QPointF *vector3 = new QPointF( vector1->x() * innerProduct / magnitudePow2, vector1->y() * innerProduct / magnitudePow2 );
            QPointF *projectedPoint = new QPointF( point1->x() + vector3->x(), point1->y() + vector3->y() );
            double distance2 = std::sqrt( std::pow(projectedPoint->x() - point3->x(), 2.0) + std::pow(projectedPoint->y() - point3->y(), 2.0) );

            if (distance2 > 0)
            {
                QPointF *scale = new QPointF(distance, distance2);
                qreal rotation = std::atan2( point2->y() - point1->y(), point2->x() - point1->x() );

                this->doc->addInsert(name, *point1, *scale, rotation);
                this->doc->updateView();
            }
        }
    }

    this->doc->setLayer(initialLayer);
}

void TT_DialogDrawBlocks::on_tbTree_clicked()
{
    drawBlockOnePoint("tree", "TT_VEGETATION");
}

void TT_DialogDrawBlocks::on_tbElectricalBox_clicked()
{
    drawBlockOnePoint("electrical_box", "TT_ELECTRICITY");
}

void TT_DialogDrawBlocks::on_tbElectricalBox_2points_clicked()
{
    drawBlockTwoPoints("electrical_box_2p", "TT_ELECTRICITY");
}

void TT_DialogDrawBlocks::on_tbElectricalBox_3points_clicked()
{
    drawBlockThreePoints("electrical_box_3p", "TT_ELECTRICITY");
}
