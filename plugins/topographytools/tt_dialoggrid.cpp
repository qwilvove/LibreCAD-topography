#include "tt_dialoggrid.h"
#include "ui_tt_dialoggrid.h"

#include <QTemporaryDir>

TT_DialogGrid::TT_DialogGrid(QWidget *parent, Document_Interface *doc):
    ui(new Ui::TT_DialogGrid),
    parent(parent),
    doc(doc)
{
    ui->setupUi(this);
}

TT_DialogGrid::~TT_DialogGrid()
{
    delete ui;
}

void TT_DialogGrid::on_pbInsertGrid_clicked()
{
    this->close();
    this->parent->close();

    QString initialLayer = this->doc->getCurrentLayer();

    // Prepare layer
    QString layer = "TT_GRID";
    this->doc->setLayer(layer);
    int colour = 0x000000;
    DPI::LineWidth lineWidth = DPI::LineWidth::WidthDefault;
    DPI::LineType lineType = DPI::LineType::SolidLine;
    this->doc->setCurrentLayerProperties(colour, lineWidth, lineType);

    QString file = "tt_grid";

    // Extract block from ressources
    QTemporaryDir tempDir;
    if (tempDir.isValid())
    {
        const QString tempFile = tempDir.path() + "/" + file + ".dxf";
        if (QFile::copy(":/blocks/" + file + ".dxf", tempFile))
        {
            // Only create the block if it does not exist
            bool blockExists = false;
            foreach (QString blockName, this->doc->getAllBlocks())
            {
                if ( blockName == file )
                {
                    blockExists = true;
                    break;
                }
            }
            if ( !blockExists )
            {
                this->doc->addBlockfromFromdisk(tempFile, "tt_caroyage");
            }
        }
    }

    QPointF *point = new QPointF();
    QPointF *scale = new QPointF(1,1);
    this->doc->getPoint(point);
    this->doc->addInsert("tt_caroyage", *point, *scale, 0);
    this->doc->updateView();

    QString file2 = "tt_grid_cross";

    // Extract block from ressources
    QTemporaryDir tempDir2;
    if (tempDir2.isValid())
    {
        const QString tempFile2 = tempDir2.path() + "/" + file2 + ".dxf";
        if (QFile::copy(":/blocks/" + file2 + ".dxf", tempFile2))
        {
            // Only create the block if it does not exist
            bool blockExists = false;
            foreach (QString blockName, this->doc->getAllBlocks())
            {
                if ( blockName == file2 )
                {
                    blockExists = true;
                    break;
                }
            }
            if ( !blockExists )
            {
                this->doc->addBlockfromFromdisk(tempFile2);
            }
        }
    }

    QPointF *basePoint = new QPointF( point->x() - std::fmod(point->x(), 10.0), point->y() - std::fmod(point->y(), 10.0) );
    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            QPointF *point2 = new QPointF(basePoint->x() + i * 10, basePoint->y() + j * 10);
            this->doc->addInsert(file2, *point2, *scale, 0);
        }
    }
    this->doc->updateView();

    this->doc->setLayer(initialLayer);

    /*Plug_Entity *ent = this->doc->getEnt("Selectionner une (petite) entité !");
    for (int i = 0; i < 10; i++)
    {
        ent->rotate(QPointF(0,0), M_PI / 10.0, DPI::KEEP_ORIGINAL);
    }*/
}

void TT_DialogGrid::on_pbMoveGrid_clicked()
{
    this->close();
    this->parent->close();

    Plug_Entity *ent = this->doc->getEnt();
    //Plug_Entity *newEnt = this->doc->newEntity(DPI::ETYPE::CIRCLE);

    QHash<int, QVariant> entData = QHash<int, QVariant>();
    ent->getData(&entData);
    entData[DPI::EDATA::STARTANGLE] = M_PI/8.0;
    entData[DPI::EDATA::COLSPACE] = M_PI;
    entData[DPI::EDATA::ROWSPACE] = 2*M_PI;
    entData[DPI::EDATA::COLCOUNT] = 3;
    entData[DPI::EDATA::ROWCOUNT] = 6;
    for (auto [key, value] : entData.asKeyValueRange())
    {
        qInfo() << key << value;
    }
    ent->updateData(&entData);

    /*QPointF *point1 = new QPointF();
    QPointF *point2 = new QPointF();
    this->doc->getPoint(point1);
    this->doc->getPoint(point2);

    QPointF *offset = new QPointF(point2->x() - point1->x(), point2->y() - point1->y());
    ent->move(*offset);*/
}

void TT_DialogGrid::on_pbRotateGrid_clicked()
{
    this->close();
    this->parent->close();

    Plug_Entity *grid = this->doc->getEnt("Select grid");
    double angle = 0;
    this->doc->getReal(&angle, "angle de rotation");
    angle = angle * M_PI / 180.0;

    QHash<int, QVariant> gridData = QHash<int, QVariant>();
    grid->getData(&gridData);
    //gridData[DPI::EDATA::STARTANGLE] = angle;

    QPointF rotationCentre = QPointF( gridData[DPI::EDATA::STARTX].toDouble(), gridData[DPI::EDATA::STARTY].toDouble() );
    grid->rotate(rotationCentre, angle);

    /*QList<Plug_Entity*> *sel = new QList<Plug_Entity*>();
    this->doc->getAllEntities(sel);
    //Plug_Entity *newEnt = this->doc->newEntity(DPI::ETYPE::CIRCLE);
    for (int i = 0; i < sel->size(); i++)
    {
        QHash<int, QVariant> entData = QHash<int, QVariant>();
        sel->at(i)->getData(&entData);
        for (auto [key, value] : entData.asKeyValueRange())
        {
            if (key == DPI::EDATA::ETYPE)
            {
                qInfo() << i << key << value;
            }
        }
    }*/
}

void TT_DialogGrid::on_pbPreparePrint_clicked()
{
    this->close();
    this->parent->close();

    qInfo() << "test1";
    Plug_Entity *grid = this->doc->getEnt("Select grid");
    qInfo() << "test2";
    if (true)
    {
        return;
    }

    QHash<int, QVariant> gridData = QHash<int, QVariant>();
    grid->getData(&gridData);
    QPointF rotationCentre = QPointF( gridData[DPI::EDATA::STARTX].toDouble(), gridData[DPI::EDATA::STARTY].toDouble() );
    double angle = gridData[DPI::EDATA::STARTANGLE].toDouble();

    QList<Plug_Entity*> *sel = new QList<Plug_Entity*>();
    this->doc->getAllEntities(sel);
    for (int i = 0; i < sel->size(); i++)
    {
        sel->at(i)->rotateWithoutUndo(rotationCentre, -angle);
        sel->at(i)->moveWithoutUndo(-rotationCentre);
    }

    this->doc->addVariable("TT_GRID_ROTATION", angle);
    this->doc->addVariable("TT_GRID_X", rotationCentre.x());
    this->doc->addVariable("TT_GRID_Y", rotationCentre.y());
}

void TT_DialogGrid::on_pbRestoreDrawing_clicked()
{
    this->close();
    this->parent->close();

    double angle = 0.0;
    double x = 0.0;
    double y = 0.0;

    this->doc->getVariableDouble("TT_GRID_ROTATION", &angle);
    this->doc->getVariableDouble("TT_GRID_X", &x);
    this->doc->getVariableDouble("TT_GRID_Y", &y);

    QPointF rotationCentre = QPointF( x, y );
    QList<Plug_Entity*> *sel = new QList<Plug_Entity*>();
    this->doc->getAllEntities(sel);
    for (int i = 0; i < sel->size(); i++)
    {
        sel->at(i)->moveWithoutUndo(rotationCentre);
        sel->at(i)->rotateWithoutUndo(rotationCentre, angle);
    }

    this->doc->removeVariable("TT_GRID_ROTATION");
    this->doc->removeVariable("TT_GRID_X");
    this->doc->removeVariable("TT_GRID_Y");
}

