#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "staffGamesConstants.h"
#include "stickylinesignalhandler.h"
#include "stickynotesignalhandler.h"

#include <QLine>
#include <QRect>
#include <QGraphicsItem>
#include <QScrollBar>
#include <QMouseEvent>

#include <QDebug>
#include <QSound>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_scene = new QGraphicsScene(this);
#ifdef MOUSE_TRACKING
    m_mousePosTrigger = new QTimer(this);
    m_mousePosTrigger->setInterval(10);
    connect(m_mousePosTrigger, SIGNAL(timeout()), this, SLOT(getMousePos()));
    m_mousePosTrigger->start();
#endif    
    ui->graphicsView->setScene(m_scene);    
    ui->score->setValue(0);    
    //QRect sceneRect(0, staffLayout::lowerBounds, m_scene->sceneRect().bottomRight().x(), staffLayout::upperBounds);
    setBounds();
    createStaff();
    QPixmap image(":/notation/treble");
    QGraphicsPixmapItem* pixmap = m_scene->addPixmap(image);
    pixmap->setPos(trebleClef::offset, -30);
    pixmap->setScale(1);
    pixmap->setData(objectPropertyKeys::type, objectPropertyTypes::trebleType);
    makeMap();
    createNote();
    //setMouseTracking(true);
    nextRound();

    //m_scene->itemAt(p1, QTransform())->setFlag(QGraphicsItem::ItemIsMovable);;    
    ui->graphicsView->setDragMode(QGraphicsView::ScrollHandDrag);

//    QWidget::showMaximized();
}

MainWindow::~MainWindow()
{
    delete ui;
}

#ifdef MOUSE_TRACKING
void MainWindow::getMousePos()
{
    static QPoint oldPos = QCursor::pos();
    if(oldPos == QCursor::pos()) return;
    oldPos = QCursor::pos();
    qDebug() << QCursor::pos();
}
#endif

void MainWindow::createStaff()
{


    using namespace staffLayout;

    QPen staffPen(Qt::black);
    staffPen.setWidth(lineHeight);
    QPoint p1Staff(0, 0);
    QPoint p2Staff(lineLength, lineHeight);

    QPoint p1White(0, lineHeight + onePixel );
    QPoint p2White(lineLength, whitespaceHeight+lineHeight - onePixel);

    QPoint staffOffset(0, whitespaceHeight + lineHeight);
    QPoint whiteOffset(0, lineHeight + whitespaceHeight);

    for(int line=0; line<numStaffLines; ++line){
        //QLine staffLine(p1, p2);
        //m_scene->addLine(staffLine, staffPen);
        StickyLine* staffLine = new StickyLine(p1Staff, p2Staff);
        StickyLineSignalHandler* signalHandler = new StickyLineSignalHandler(this);
        QVariant lineStr = line;
        QString name = objectPropertyTypes::lineType + lineStr.toString();

        staffLine->setColour( QColor(Qt::black) );
        staffLine->setHighlight(QColor(colours::highlighted));
        staffLine->setData(objectPropertyKeys::type, objectPropertyTypes::lineType);
        staffLine->setData(objectPropertyKeys::name, name);
        staffLine->addSignalHandler(signalHandler);
        signalHandler->m_lineName = name;
        m_lineToSignalHandler.insert(name, signalHandler);
        m_scene->addItem(staffLine);
//        signalHandler->setLine(staffLine);


        p1Staff += staffOffset;
        p2Staff += staffOffset;

        StickyLine* whitespace = new StickyLine(p1White, p2White);

        name = objectPropertyTypes::whiteLineType + lineStr.toString();
        signalHandler = new StickyLineSignalHandler(this);
        whitespace->setColour(QColor(Qt::white));
        whitespace->setHighlight(QColor(colours::highlighted));
        whitespace->setCollisionMode(Qt::ContainsItemShape);
        whitespace->setData(objectPropertyKeys::type, objectPropertyTypes::whiteLineType);
        whitespace->setData(objectPropertyKeys::name, name);
        whitespace->addSignalHandler(signalHandler);
        signalHandler->m_lineName = name;
        m_lineToSignalHandler.insert(name, signalHandler);
        m_scene->addItem(whitespace);
        //signalHandler->setLine(whitespace);

        p1White += whiteOffset;
        p2White += whiteOffset;
    }

}

void MainWindow::createNote()
{
    QPoint topLeft(0, 0);
    QPoint bottomRight(noteProperties::noteDiameter+10, noteProperties::noteDiameter+10);
    QRect noteSize(topLeft, bottomRight );


    QPoint centre((noteProperties::noteDiameter-10)/2, (noteProperties::noteDiameter-10)/2);
    centre += topLeft;

    QPen notePen(Qt::black);
    QBrush noteBrush(Qt::black);
    StickyNote* note = new StickyNote(noteSize);
    StickyNoteSignalHandler* signalHandler = new StickyNoteSignalHandler(this);

    connect(signalHandler, SIGNAL(submitLine(QString)), this, SLOT(userNoteMoved(QString)));
    connect(signalHandler, SIGNAL(scrollUp()), this, SLOT(scrollUp()));
    connect(signalHandler, SIGNAL(scrollDown()), this, SLOT(scrollDown()));
    connect(this, SIGNAL(scrollFinished()), signalHandler, SLOT(finishedScroll()));
    note->setSignalHandler(signalHandler);
    //m_scene->addEllipse(noteSize, notePen, noteBrush);
    m_scene->addItem(note);
    m_scene->itemAt(noteSize.center(), QTransform())->setFlag(QGraphicsItem::ItemIsMovable);
    note->moveBy(staffLayout::lineLength/2, staffLayout::whitespaceHeight);
    ui->graphicsView->centerOn(note);
}

void MainWindow::setBounds()
{
    QRect sceneOrigRect = m_scene->sceneRect().toRect();
    QPoint p1(trebleClef::offset, staffLayout::upperBounds);
    QPoint p2(staffLayout::lineLength+(-trebleClef::offset), staffLayout::lowerBounds);
    QRect sceneRect(p1, p2);
    m_scene->setSceneRect(sceneRect);
}

void MainWindow::makeMap()
{

    QList<QGraphicsItem*> lines = getLines();

    m_noteLineMap.insert(lines.at(0)->data(objectPropertyKeys::name).toString(), "D");
    m_noteLineMap.insert(lines.at(1)->data(objectPropertyKeys::name).toString(), "E");
    m_noteLineMap.insert(lines.at(2)->data(objectPropertyKeys::name).toString(), "F");
    m_noteLineMap.insert(lines.at(3)->data(objectPropertyKeys::name).toString(), "G");
    m_noteLineMap.insert(lines.at(4)->data(objectPropertyKeys::name).toString(), "A'");
    m_noteLineMap.insert(lines.at(5)->data(objectPropertyKeys::name).toString(), "B'");
    m_noteLineMap.insert(lines.at(6)->data(objectPropertyKeys::name).toString(), "C'");
    m_noteLineMap.insert(lines.at(7)->data(objectPropertyKeys::name).toString(), "D'");
    m_noteLineMap.insert(lines.at(8)->data(objectPropertyKeys::name).toString(), "E'");
    m_noteLineMap.insert(lines.at(9)->data(objectPropertyKeys::name).toString(), "F'");

}

void MainWindow::nextRound()
{

    QList<QGraphicsItem*> lines = getLines();
    int randNum = qrand() % lines.size();
    QVariant name = lines.at(randNum)->data(objectPropertyKeys::name);
    QString nameStr = name.toString();
    qDebug() << m_noteLineMap.value(nameStr, "");
    m_answer = m_noteLineMap.value(nameStr, "");
    ui->guiChallenge->setText("Find the note: " + m_answer);
}

QList<QGraphicsItem *> MainWindow::getLines()
{
    QList<QGraphicsItem*> items = m_scene->items();
    QVariant type;
    QVariant typeStr;
    QList<QGraphicsItem*> lines;
    for(int i=0; i<items.size(); ++i){
        type = items.at(i)->data(objectPropertyKeys::type);
        typeStr = type.toString();
        if(typeStr == objectPropertyTypes::lineType || typeStr == objectPropertyTypes::whiteLineType){
            lines.push_back(items.at(i));
        }
    }
    return lines;
}

void MainWindow::userNoteMoved(QString line)
{    
    if(m_noteLineMap.value(line) == m_answer){

        m_lineToSignalHandler.value(line)->userResult(true);
        QString sound = ":/audio/"+m_answer+".wav";
        qDebug() << sound;
        QSound::play(sound);
        qDebug() << "WIN!";
        ui->score->setValue(ui->score->value() + 1);
        if(ui->score->value() == ui->score->maximum())
            ui->stackedWidget->setCurrentIndex(1);
        nextRound();

 //       emit userResult(true);
    }
    else{
        qDebug() << "FAIL!";
        m_lineToSignalHandler.value(line)->userResult(false);
        QStringList failFiles;
        QString randFile;
        failFiles << ":/audio/fail1";
        failFiles << ":/audio/fail2";
        failFiles << ":/audio/fail3";
        failFiles << ":/audio/fail4";
        failFiles << ":/audio/fail5";
        randFile = failFiles.at(qrand() % failFiles.size());

        QSound::play(randFile);
        ui->score->setValue(ui->score->value() - 1);
   //     emit userResult(false);
    }

}

void MainWindow::scrollDown()
{
    QScrollBar* vScroll = ui->graphicsView->verticalScrollBar();
    int value = vScroll->value();
    vScroll->setValue(value+40);
    //qDebug() << "Scrolled down";
    emit scrollFinished();
}

void MainWindow::scrollUp()
{
    QScrollBar* vScroll = ui->graphicsView->verticalScrollBar();
    int value = vScroll->value();
    vScroll->setValue(value-40);
    //qDebug() << "Scrolled up";
    emit scrollFinished();
}

void MainWindow::on_pushButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
    ui->score->setValue(0);
}
