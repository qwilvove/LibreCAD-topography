#include "draw_blocks.h"
#include "ui_draw_blocks.h"

#include "src/ui/dialog/main.h"

#include <cmath>
#include <QTemporaryDir>
#include <QScrollArea>

TT_DialogDrawBlocks::TT_DialogDrawBlocks(QWidget *parent, Document_Interface *doc):
    QDialog(parent),
    ui(new Ui::TT_DialogDrawBlocks),
    doc(doc)
{
    ui->setupUi(this);

    setupUi();
}

TT_DialogDrawBlocks::~TT_DialogDrawBlocks()
{
    delete ui;
}

void TT_DialogDrawBlocks::loadPreviousState(int tabIndex, int insertTypeIndex, TT::Block::INSERTION_TYPE insertType)
{
    this->ui->tabWidget->setCurrentIndex(tabIndex);
    this->ui->buttonGroup->button(insertTypeIndex)->setChecked(true);
    updateUi(insertType);
}

void TT_DialogDrawBlocks::setupUi()
{
    // Collect tabs
    QList<TT::Block::TAB> tabs;
    for (TT::Block block : TT::Block::getBlocks())
    {
        if ( !tabs.contains(block.getTab()) )
        {
            tabs.append(block.getTab());
        }
    }

    // Collect blocks
    for ( int i = 0; i < tabs.size(); i++ )
    {
        QList<TT::Block> blocksOfThisTab;
        for (TT::Block block : TT::Block::getBlocks())
        {
            if ( block.getTab() == tabs.at(i) )
            {
                blocksOfThisTab.append(block);
            }
        }
        blocks.append(blocksOfThisTab);
    }

    // For each tab name, create a QWidget and populate it with buttons
    QList<QWidget*> tabWidgets;
    for ( int i = 0; i < tabs.size(); i++ )
    {
        QList<QToolButton *> tabButtons;

        QWidget *newTab = new QWidget(ui->tabWidget);
        QVBoxLayout *newTabLayout = new QVBoxLayout(newTab);
        newTab->setLayout(newTabLayout);

        QScrollArea *scrollArea = new QScrollArea(newTab);
        scrollArea->setWidgetResizable(true);
        newTabLayout->addWidget(scrollArea);

        QWidget *scrollAreaContent = new QWidget(scrollArea);
        QGridLayout *scrollAreaContentLayout = new QGridLayout(scrollAreaContent);
        scrollAreaContent->setLayout(scrollAreaContentLayout);

        // For each block : create a button
        for ( int j = 0; j < blocks.at(i).size(); j++ )
        {
            QToolButton *tb = new QToolButton(scrollAreaContent);
            TT::Block blk = blocks.at(i).at(j);
            QString iconPath = QString(":thumbnails/" + blk.getFile() + ".svg");
            QIcon icon = QIcon(iconPath);
            tb->setIcon(icon);
            QSize iconSize = QSize(64, 64);
            tb->setIconSize(iconSize);
            scrollAreaContentLayout->addWidget(tb, j / 4, j % 4);

            tabButtons.append(tb);

            // Then, create a function for the button
            connect(tb, &QToolButton::clicked, this, [this,i,j]{
                switch(currentInsertionType)
                {
                case TT::Block::INSERTION_TYPE::P1:
                    drawBlockOnePoint(blocks.at(i).at(j));
                    break;
                case TT::Block::INSERTION_TYPE::P2:
                    drawBlockTwoPoints(blocks.at(i).at(j));
                    break;
                case TT::Block::INSERTION_TYPE::P3:
                    drawBlockThreePoints(blocks.at(i).at(j));
                    break;
                case TT::Block::INSERTION_TYPE::P2V:
                    drawBlockTwoPointsPlusValue(blocks.at(i).at(j));
                    break;
                default:
                    break;
                }
            });
        }

        scrollArea->setWidget(scrollAreaContent);

        buttons.append(tabButtons);
        tabWidgets.append(newTab);

        ui->tabWidget->addTab(newTab, TT::Block::getTabName(tabs.at(i)));
    }
    updateUi(TT::Block::INSERTION_TYPE::P1);
}

void TT_DialogDrawBlocks::updateUi(TT::Block::INSERTION_TYPE insertionType)
{
    currentInsertionType = insertionType;
    for (int i = 0; i < blocks.size(); i++)
    {
        for (int j = 0; j < blocks.at(i).size(); j++)
        {
            TT::Block blk = blocks.at(i).at(j);
            TT::Block::INSERTION_TYPE blockType = blk.getType();
            QToolButton *button = buttons.at(i).at(j);

            button->setEnabled(false);
            switch (insertionType)
            {
            case TT::Block::INSERTION_TYPE::P1:
                if (blockType == TT::Block::INSERTION_TYPE::ALL ||
                    blockType == TT::Block::INSERTION_TYPE::P1)
                {
                    button->setEnabled(true);
                }
                break;
            case TT::Block::INSERTION_TYPE::P2:
                if (blockType == TT::Block::INSERTION_TYPE::ALL ||
                    blockType == TT::Block::INSERTION_TYPE::MULTI ||
                    blockType == TT::Block::INSERTION_TYPE::P2)
                {
                    button->setEnabled(true);
                }
                break;
            case TT::Block::INSERTION_TYPE::P3:
                if (blockType == TT::Block::INSERTION_TYPE::ALL ||
                    blockType == TT::Block::INSERTION_TYPE::MULTI ||
                    blockType == TT::Block::INSERTION_TYPE::P3)
                {
                    button->setEnabled(true);
                }
                break;
            case TT::Block::INSERTION_TYPE::P2V:
                if (blockType == TT::Block::INSERTION_TYPE::ALL ||
                    blockType == TT::Block::INSERTION_TYPE::MULTI ||
                    blockType == TT::Block::INSERTION_TYPE::P2V)
                {
                    button->setEnabled(true);
                }
                break;
            default:
                break;
            }
        }
    }
}

void TT_DialogDrawBlocks::createBlockIfNotExists(QString file)
{
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
                this->doc->addBlockfromFromdisk(tempFile);
            }
        }
    }
}

void TT_DialogDrawBlocks::drawBlockOnePoint(TT::Block block)
{
    ((TT_DialogMain *)this->parentWidget())->savePreviousState(DIALOG::DRAW_BLOCKS, this->ui->tabWidget->currentIndex(), this->ui->buttonGroup->checkedId(), this->currentInsertionType);

    this->close();
    ((QDialog *)this->parentWidget())->accept();

    //QString initialLayer = this->doc->getCurrentLayer();

    // Prepare layer
    //QString layer = TT::LAYERS[TT::BLOCKS[block].layer].name;
    //this->doc->setLayer(layer);
    //int colour = TT::LAYERS[TT::BLOCKS[block].layer].colour;
    //DPI::LineWidth lineWidth = TT::LAYERS[TT::BLOCKS[block].layer].lineWidth;
    //DPI::LineType lineType = TT::LAYERS[TT::BLOCKS[block].layer].lineType;
    //this->doc->setCurrentLayerProperties(colour, lineWidth, lineType);

    QString file = block.getFile();
    createBlockIfNotExists(file);

    QPointF point = QPointF();
    QPointF scale = QPointF(1,1);
    while ( this->doc->getPoint(&point) == true )
    {
        this->doc->addInsert(file, point, scale, 0);
        this->doc->updateView();
    }

    //this->doc->setLayer(initialLayer);
}

void TT_DialogDrawBlocks::drawBlockTwoPoints(TT::Block block)
{
    ((TT_DialogMain *)this->parentWidget())->savePreviousState(DIALOG::DRAW_BLOCKS, this->ui->tabWidget->currentIndex(), this->ui->buttonGroup->checkedId(), this->currentInsertionType);

    this->close();
    ((QDialog *)this->parentWidget())->accept();

    //QString initialLayer = this->doc->getCurrentLayer();

    // Prepare layer
    //QString layer = TT::LAYERS[TT::BLOCKS[block].layer].name;
    //this->doc->setLayer(layer);
    //int colour = TT::LAYERS[TT::BLOCKS[block].layer].colour;
    //DPI::LineWidth lineWidth = TT::LAYERS[TT::BLOCKS[block].layer].lineWidth;
    //DPI::LineType lineType = TT::LAYERS[TT::BLOCKS[block].layer].lineType;
    //this->doc->setCurrentLayerProperties(colour, lineWidth, lineType);

    QString file = block.getFile();
    createBlockIfNotExists(file);

    QPointF point1;
    QPointF point2;

    while ( this->doc->getPoint(&point1) == true && this->doc->getPoint(&point2) == true )
    {
        double distance = std::sqrt( std::pow(point2.x() - point1.x(), 2.0) + std::pow(point2.y() - point1.y(), 2.0) );
        if (distance > 0)
        {
            QPointF scale = QPointF(distance,distance);
            qreal rotation = std::atan2( point2.y() - point1.y(), point2.x() - point1.x() );

            this->doc->addInsert(file, point1, scale, rotation);
            this->doc->updateView();
        }
    }

    //this->doc->setLayer(initialLayer);
}

void TT_DialogDrawBlocks::drawBlockThreePoints(TT::Block block)
{
    ((TT_DialogMain *)this->parentWidget())->savePreviousState(DIALOG::DRAW_BLOCKS, this->ui->tabWidget->currentIndex(), this->ui->buttonGroup->checkedId(), this->currentInsertionType);

    this->close();
    ((QDialog *)this->parentWidget())->accept();

    //QString initialLayer = this->doc->getCurrentLayer();

    // Prepare layer
    //QString layer = TT::LAYERS[TT::BLOCKS[block].layer].name;
    //this->doc->setLayer(layer);
    //int colour = TT::LAYERS[TT::BLOCKS[block].layer].colour;
    //DPI::LineWidth lineWidth = TT::LAYERS[TT::BLOCKS[block].layer].lineWidth;
    //DPI::LineType lineType = TT::LAYERS[TT::BLOCKS[block].layer].lineType;
    //this->doc->setCurrentLayerProperties(colour, lineWidth, lineType);

    QString file = block.getFile();
    createBlockIfNotExists(file);

    QPointF point1 = QPointF();
    QPointF point2 = QPointF();
    QPointF point3 = QPointF();

    while ( this->doc->getPoint(&point1) == true && this->doc->getPoint(&point2) == true && this->doc->getPoint(&point3) == true )
    {
        double distance = std::sqrt( std::pow(point2.x() - point1.x(), 2.0) + std::pow(point2.y() - point1.y(), 2.0) );

        if (distance > 0)
        {
            QPointF vector1 = QPointF( point2.x() - point1.x(), point2.y() - point1.y() );
            QPointF vector2 = QPointF( point3.x() - point1.x(), point3.y() - point1.y() );
            double innerProduct = vector1.x() * vector2.x() + vector1.y() * vector2.y();
            double magnitudePow2 = std::pow( vector1.x(), 2.0 ) + std::pow( vector1.y(), 2.0 );
            QPointF vector3 = QPointF( vector1.x() * innerProduct / magnitudePow2, vector1.y() * innerProduct / magnitudePow2 );
            QPointF projectedPoint = QPointF( point1.x() + vector3.x(), point1.y() + vector3.y() );
            double distance2 = std::sqrt( std::pow(projectedPoint.x() - point3.x(), 2.0) + std::pow(projectedPoint.y() - point3.y(), 2.0) );

            if (distance2 > 0)
            {
                QPointF scale = QPointF(distance, distance2);
                qreal rotation = std::atan2( point2.y() - point1.y(), point2.x() - point1.x() );

                this->doc->addInsert(file, point1, scale, rotation);
                this->doc->updateView();
            }
        }
    }

    //this->doc->setLayer(initialLayer);
}

void TT_DialogDrawBlocks::drawBlockTwoPointsPlusValue(TT::Block block)
{
    ((TT_DialogMain *)this->parentWidget())->savePreviousState(DIALOG::DRAW_BLOCKS, this->ui->tabWidget->currentIndex(), this->ui->buttonGroup->checkedId(), this->currentInsertionType);

    this->close();
    ((QDialog *)this->parentWidget())->accept();

    //QString initialLayer = this->doc->getCurrentLayer();

    // Prepare layer
    //QString layer = TT::LAYERS[TT::BLOCKS[block].layer].name;
    //this->doc->setLayer(layer);
    //int colour = TT::LAYERS[TT::BLOCKS[block].layer].colour;
    //DPI::LineWidth lineWidth = TT::LAYERS[TT::BLOCKS[block].layer].lineWidth;
    //DPI::LineType lineType = TT::LAYERS[TT::BLOCKS[block].layer].lineType;
    //this->doc->setCurrentLayerProperties(colour, lineWidth, lineType);

    QString file = block.getFile();
    createBlockIfNotExists(file);

    QPointF point1 = QPointF();
    QPointF point2 = QPointF();

    while ( this->doc->getPoint(&point1) == true && this->doc->getPoint(&point2) == true )
    {
        double distance = std::sqrt( std::pow(point2.x() - point1.x(), 2.0) + std::pow(point2.y() - point1.y(), 2.0) );
        if (distance > 0)
        {
            qreal distance2;
            this->doc->getReal(&distance2);

            if (distance2 != 0)
            {
                QPointF scale = QPointF(distance, distance2);
                qreal rotation = std::atan2( point2.y() - point1.y(), point2.x() - point1.x() );

                this->doc->addInsert(file, point1, scale, rotation);
                this->doc->updateView();
            }
        }
    }

    //this->doc->setLayer(initialLayer);
}

void TT_DialogDrawBlocks::on_rb1p_clicked()
{
    updateUi(TT::Block::INSERTION_TYPE::P1);
}

void TT_DialogDrawBlocks::on_rb2p_clicked()
{
    updateUi(TT::Block::INSERTION_TYPE::P2);
}

void TT_DialogDrawBlocks::on_rb3p_clicked()
{
    updateUi(TT::Block::INSERTION_TYPE::P3);
}

void TT_DialogDrawBlocks::on_rb2pv_clicked()
{
    updateUi(TT::Block::INSERTION_TYPE::P2V);
}

