#include "tt_dialogdrawblocks.h"
#include "ui_tt_dialogdrawblocks.h"

#include <QDir>

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

void TT_DialogDrawBlocks::drawBlockOnePoint(QString name, QString layer)
{
    this->close();
    this->parent->close();

    QString initialLayer = this->doc->getCurrentLayer();

    this->doc->setLayer(layer);

    QDir dir(QCoreApplication::applicationDirPath());
    QString absolute_file_path = dir.absoluteFilePath("../plugins/topographytools/blocks/" + name + ".dxf");
    this->doc->addBlockfromFromdisk(absolute_file_path);

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

    QDir dir(QCoreApplication::applicationDirPath());
    QString absolute_file_path = dir.absoluteFilePath("../plugins/topographytools/blocks/" + name + ".dxf");
    this->doc->addBlockfromFromdisk(absolute_file_path);

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

    QDir dir(QCoreApplication::applicationDirPath());
    QString absolute_file_path = dir.absoluteFilePath("../plugins/topographytools/blocks/" + name + ".dxf");
    this->doc->addBlockfromFromdisk(absolute_file_path);

    QPointF *point1 = new QPointF();
    QPointF *point2 = new QPointF();
    QPointF *point3 = new QPointF();

    while ( this->doc->getPoint(point1) == true && this->doc->getPoint(point2) == true && this->doc->getPoint(point3) == true )
    {
        double distance = std::sqrt( std::pow(point2->x() - point1->x(), 2.0) + std::pow(point2->y() - point1->y(), 2.0) );
        double distance2 = std::sqrt( std::pow(point3->x() - point2->x(), 2.0) + std::pow(point3->y() - point2->y(), 2.0) );
        if (distance > 0 && distance2 > 0)
        {
            // TODO : use vectors to calculate projected point
            QPointF *scale = new QPointF(distance, distance2);
            qreal rotation = std::atan2( point2->y() - point1->y(), point2->x() - point1->x() );

            this->doc->addInsert(name, *point1, *scale, rotation);
            this->doc->updateView();
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
