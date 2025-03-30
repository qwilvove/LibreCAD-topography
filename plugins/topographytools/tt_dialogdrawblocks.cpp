#include "tt_dialogdrawblocks.h"
#include "ui_tt_dialogdrawblocks.h"

#include "tt_dialogmain.h"

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

void TT_DialogDrawBlocks::loadPreviousState(int tabIndex, int insertTypeIndex, TT::BLOCK_INSERTION_TYPE insertType)
{
    this->ui->tabWidget->setCurrentIndex(tabIndex);
    this->ui->buttonGroup->button(insertTypeIndex)->setChecked(true);
    updateUi(insertType);
}

void TT_DialogDrawBlocks::setupUi()
{
    // Collect tab names
    QList<QString> tabNames;
    for (auto [key, value] : TT::BLOCKS.asKeyValueRange())
    {
        if ( !tabNames.contains(value.tabName) )
        {
            tabNames.append(value.tabName);
        }
    }

    // Collect blocks
    for ( int i = 0; i < tabNames.size(); i++ )
    {
        QList<TT::BLOCK> blocksOfThisTab;
        for (auto [key, value] : TT::BLOCKS.asKeyValueRange())
        {
            if ( value.tabName == tabNames.at(i) )
            {
                blocksOfThisTab.append(key);
            }
        }
        blocks.append(blocksOfThisTab);
    }

    // For each tab name, create a QWidget and populate it with buttons
    QList<QWidget*> tabs;
    for ( int i = 0; i < tabNames.size(); i++ )
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
            QString iconPath = QString(":thumbnails/" + TT::BLOCKS[blocks.at(i).at(j)].file + ".svg");
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
                case TT::BLOCK_INSERTION_TYPE::P1:
                    drawBlockOnePoint(blocks.at(i).at(j));
                    break;
                case TT::BLOCK_INSERTION_TYPE::P2:
                    drawBlockTwoPoints(blocks.at(i).at(j));
                    break;
                case TT::BLOCK_INSERTION_TYPE::P3:
                    drawBlockThreePoints(blocks.at(i).at(j));
                    break;
                case TT::BLOCK_INSERTION_TYPE::P2V:
                    drawBlockTwoPointsPlusValue(blocks.at(i).at(j));
                    break;
                default:
                    break;
                }
            });
        }

        scrollArea->setWidget(scrollAreaContent);

        buttons.append(tabButtons);
        tabs.append(newTab);

        ui->tabWidget->addTab(newTab, tabNames.at(i));
    }
    updateUi(TT::BLOCK_INSERTION_TYPE::P1);
}

void TT_DialogDrawBlocks::updateUi(TT::BLOCK_INSERTION_TYPE insertionType)
{
    currentInsertionType = insertionType;
    for (int i = 0; i < blocks.size(); i++)
    {
        for (int j = 0; j < blocks.at(i).size(); j++)
        {
            buttons.at(i).at(j)->setEnabled(false);
            switch (insertionType)
            {
            case TT::BLOCK_INSERTION_TYPE::P1:
                if (TT::BLOCKS[blocks.at(i).at(j)].type == TT::BLOCK_INSERTION_TYPE::ALL ||
                    TT::BLOCKS[blocks.at(i).at(j)].type == TT::BLOCK_INSERTION_TYPE::P1)
                {
                    buttons.at(i).at(j)->setEnabled(true);
                }
                break;
            case TT::BLOCK_INSERTION_TYPE::P2:
                if (TT::BLOCKS[blocks.at(i).at(j)].type == TT::BLOCK_INSERTION_TYPE::ALL ||
                    TT::BLOCKS[blocks.at(i).at(j)].type == TT::BLOCK_INSERTION_TYPE::MULTI ||
                    TT::BLOCKS[blocks.at(i).at(j)].type == TT::BLOCK_INSERTION_TYPE::P2)
                {
                    buttons.at(i).at(j)->setEnabled(true);
                }
                break;
            case TT::BLOCK_INSERTION_TYPE::P3:
                if (TT::BLOCKS[blocks.at(i).at(j)].type == TT::BLOCK_INSERTION_TYPE::ALL ||
                    TT::BLOCKS[blocks.at(i).at(j)].type == TT::BLOCK_INSERTION_TYPE::MULTI ||
                    TT::BLOCKS[blocks.at(i).at(j)].type == TT::BLOCK_INSERTION_TYPE::P3)
                {
                    buttons.at(i).at(j)->setEnabled(true);
                }
                break;
            case TT::BLOCK_INSERTION_TYPE::P2V:
                if (TT::BLOCKS[blocks.at(i).at(j)].type == TT::BLOCK_INSERTION_TYPE::ALL ||
                    TT::BLOCKS[blocks.at(i).at(j)].type == TT::BLOCK_INSERTION_TYPE::MULTI ||
                    TT::BLOCKS[blocks.at(i).at(j)].type == TT::BLOCK_INSERTION_TYPE::P2V)
                {
                    buttons.at(i).at(j)->setEnabled(true);
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

void TT_DialogDrawBlocks::drawBlockOnePoint(TT::BLOCK block)
{
    ((TT_DialogMain *)this->parentWidget())->savePreviousState(DIALOG::DRAW_BLOCKS, this->ui->tabWidget->currentIndex(), this->ui->buttonGroup->checkedId(), this->currentInsertionType);

    this->close();
    ((QDialog *)this->parentWidget())->accept();

    QString initialLayer = this->doc->getCurrentLayer();

    // Prepare layer
    QString layer = TT::LAYERS[TT::BLOCKS[block].layer].name;
    this->doc->setLayer(layer);
    int colour = TT::LAYERS[TT::BLOCKS[block].layer].colour;
    DPI::LineWidth lineWidth = TT::LAYERS[TT::BLOCKS[block].layer].lineWidth;
    DPI::LineType lineType = TT::LAYERS[TT::BLOCKS[block].layer].lineType;
    this->doc->setCurrentLayerProperties(colour, lineWidth, lineType);

    QString file = TT::BLOCKS[block].file;
    createBlockIfNotExists(file);

    QPointF *point = new QPointF();
    QPointF *scale = new QPointF(1,1);
    while ( this->doc->getPoint(point) == true )
    {
        this->doc->addInsert(file, *point, *scale, 0);
        this->doc->updateView();
    }

    this->doc->setLayer(initialLayer);
}

void TT_DialogDrawBlocks::drawBlockTwoPoints(TT::BLOCK block)
{
    ((TT_DialogMain *)this->parentWidget())->savePreviousState(DIALOG::DRAW_BLOCKS, this->ui->tabWidget->currentIndex(), this->ui->buttonGroup->checkedId(), this->currentInsertionType);

    this->close();
    ((QDialog *)this->parentWidget())->accept();

    QString initialLayer = this->doc->getCurrentLayer();

    // Prepare layer
    QString layer = TT::LAYERS[TT::BLOCKS[block].layer].name;
    this->doc->setLayer(layer);
    int colour = TT::LAYERS[TT::BLOCKS[block].layer].colour;
    DPI::LineWidth lineWidth = TT::LAYERS[TT::BLOCKS[block].layer].lineWidth;
    DPI::LineType lineType = TT::LAYERS[TT::BLOCKS[block].layer].lineType;
    this->doc->setCurrentLayerProperties(colour, lineWidth, lineType);

    QString file = TT::BLOCKS[block].file;
    createBlockIfNotExists(file);

    QPointF *point1 = new QPointF();
    QPointF *point2 = new QPointF();

    while ( this->doc->getPoint(point1) == true && this->doc->getPoint(point2) == true )
    {
        double distance = std::sqrt( std::pow(point2->x() - point1->x(), 2.0) + std::pow(point2->y() - point1->y(), 2.0) );
        if (distance > 0)
        {
            QPointF *scale = new QPointF(distance,distance);
            qreal rotation = std::atan2( point2->y() - point1->y(), point2->x() - point1->x() );

            this->doc->addInsert(file, *point1, *scale, rotation);
            this->doc->updateView();
        }
    }

    this->doc->setLayer(initialLayer);
}

void TT_DialogDrawBlocks::drawBlockThreePoints(TT::BLOCK block)
{
    ((TT_DialogMain *)this->parentWidget())->savePreviousState(DIALOG::DRAW_BLOCKS, this->ui->tabWidget->currentIndex(), this->ui->buttonGroup->checkedId(), this->currentInsertionType);

    this->close();
    ((QDialog *)this->parentWidget())->accept();

    QString initialLayer = this->doc->getCurrentLayer();

    // Prepare layer
    QString layer = TT::LAYERS[TT::BLOCKS[block].layer].name;
    this->doc->setLayer(layer);
    int colour = TT::LAYERS[TT::BLOCKS[block].layer].colour;
    DPI::LineWidth lineWidth = TT::LAYERS[TT::BLOCKS[block].layer].lineWidth;
    DPI::LineType lineType = TT::LAYERS[TT::BLOCKS[block].layer].lineType;
    this->doc->setCurrentLayerProperties(colour, lineWidth, lineType);

    QString file = TT::BLOCKS[block].file;
    createBlockIfNotExists(file);

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

                this->doc->addInsert(file, *point1, *scale, rotation);
                this->doc->updateView();
            }
        }
    }

    this->doc->setLayer(initialLayer);
}

void TT_DialogDrawBlocks::drawBlockTwoPointsPlusValue(TT::BLOCK block)
{
    ((TT_DialogMain *)this->parentWidget())->savePreviousState(DIALOG::DRAW_BLOCKS, this->ui->tabWidget->currentIndex(), this->ui->buttonGroup->checkedId(), this->currentInsertionType);

    this->close();
    ((QDialog *)this->parentWidget())->accept();

    QString initialLayer = this->doc->getCurrentLayer();

    // Prepare layer
    QString layer = TT::LAYERS[TT::BLOCKS[block].layer].name;
    this->doc->setLayer(layer);
    int colour = TT::LAYERS[TT::BLOCKS[block].layer].colour;
    DPI::LineWidth lineWidth = TT::LAYERS[TT::BLOCKS[block].layer].lineWidth;
    DPI::LineType lineType = TT::LAYERS[TT::BLOCKS[block].layer].lineType;
    this->doc->setCurrentLayerProperties(colour, lineWidth, lineType);

    QString file = TT::BLOCKS[block].file;
    createBlockIfNotExists(file);

    QPointF *point1 = new QPointF();
    QPointF *point2 = new QPointF();

    while ( this->doc->getPoint(point1) == true && this->doc->getPoint(point2) == true )
    {
        double distance = std::sqrt( std::pow(point2->x() - point1->x(), 2.0) + std::pow(point2->y() - point1->y(), 2.0) );
        if (distance > 0)
        {
            qreal distance2;
            this->doc->getReal(&distance2);

            if (distance2 != 0)
            {
                QPointF *scale = new QPointF(distance, distance2);
                qreal rotation = std::atan2( point2->y() - point1->y(), point2->x() - point1->x() );

                this->doc->addInsert(file, *point1, *scale, rotation);
                this->doc->updateView();
            }
        }
    }

    this->doc->setLayer(initialLayer);
}

void TT_DialogDrawBlocks::on_rb1p_clicked()
{
    updateUi(TT::BLOCK_INSERTION_TYPE::P1);
}

void TT_DialogDrawBlocks::on_rb2p_clicked()
{
    updateUi(TT::BLOCK_INSERTION_TYPE::P2);
}

void TT_DialogDrawBlocks::on_rb3p_clicked()
{
    updateUi(TT::BLOCK_INSERTION_TYPE::P3);
}

void TT_DialogDrawBlocks::on_rb2pv_clicked()
{
    updateUi(TT::BLOCK_INSERTION_TYPE::P2V);
}

