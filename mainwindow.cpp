#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "staffGamesConstants.h"
#include "stickylinesignalhandler.h"
#include "stickynotesignalhandler.h"

#include <QLine>
#include <QRect>
#include <QGraphicsItem>

#include <QMouseEvent>

#include <QDebug>


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
    //setMouseTracking(true);



    createStaff();
    QPixmap image(":/notation/treble");
    QGraphicsPixmapItem* pixmap = m_scene->addPixmap(image);
    pixmap->setPos(-100, -30);
    pixmap->setScale(1);
    pixmap->setData(objectPropertyKeys::type, objectPropertyTypes::trebleType);
    makeMap();
    createNote();
    nextRound();

    //m_scene->itemAt(p1, QTransform())->setFlag(QGraphicsItem::ItemIsMovable);;

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
        StickyLineSignalHandler* signalHandler(this);
        signalHandler->setLine(staffLine);

        QVariant lineStr = line;
        staffLine->setColour( QColor(Qt::black) );
        staffLine->setData(objectPropertyKeys::type, objectPropertyTypes::lineType);
        staffLine->setData(objectPropertyKeys::name, objectPropertyTypes::lineType + lineStr.toString());
        m_scene->addItem(staffLine);

        p1Staff += staffOffset;
        p2Staff += staffOffset;

        StickyLine* whitespace = new StickyLine(p1White, p2White);
        StickyLineSignalHandler* signalHandler(this);
        signalHandler->setLine(whitespace);
        SETTING UP COLOUR CHANGE IN LINE USING SIGNAL HANDLERS
        whitespace->setColour(QColor(Qt::white));
        whitespace->setCollisionMode(Qt::ContainsItemShape);
        whitespace->setData(objectPropertyKeys::type, objectPropertyTypes::whiteLineType);
        whitespace->setData(objectPropertyKeys::name, objectPropertyTypes::whiteLineType + lineStr.toString());
        m_scene->addItem(whitespace);

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
    connect(this, SIGNAL(userResult(bool)), signalHandler, SLOT(userResult(bool)));
    note->setSignalHandler(signalHandler);
    //m_scene->addEllipse(noteSize, notePen, noteBrush);
    m_scene->addItem(note);
    m_scene->itemAt(noteSize.center(), QTransform())->setFlag(QGraphicsItem::ItemIsMovable);

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
    ui->guiChallenge->setText("Find a : " + m_answer);
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
        nextRound();
        qDebug() << "WIN!";
        emit userResult(true);
    }
    else{
        qDebug() << "FAIL!";
        emit userResult(false);
    }
}
