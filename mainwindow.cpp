#include "mainwindow.h"
#include "ui_mainwindow.h"
//#include "staffGamesConstants.h"
//#include "stickylinesignalhandler.h"
//#include "stickynotesignalhandler.h"

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
    m_scene = new StaffScene(this);
    connect(m_scene, SIGNAL( lineSelected(int) ), this, SLOT( lineSelected(int) ));

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
//    createStaff();
    QPixmap image(":/notation/treble");
    QGraphicsPixmapItem* pixmap = m_scene->addPixmap(image);
    pixmap->setPos(trebleClef::offset, -30);
    pixmap->setScale(1);
    pixmap->setData(objectPropertyKeys::type, objectPropertyTypes::trebleType);
    makeMap();
//    createNote();
    //setMouseTracking(true);
//    nextRound();

    //m_scene->itemAt(p1, QTransform())->setFlag(QGraphicsItem::ItemIsMovable);;    
    //TODO check if this overrides the other drag behaviour
    ui->graphicsView->setDragMode(QGraphicsView::ScrollHandDrag);

    QWidget::showMaximized();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::lineSelected(int line)
{
    //qDebug() << line;
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
/*
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

    for(int line=numStaffLines+numLedgerLines-1; line>numLedgerLines-1; --line){
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
        staffLine->setData(objectPropertyKeys::ledgerType, ledgerType::stave);
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
        whitespace->setData(objectPropertyKeys::ledgerType, ledgerType::stave);
        whitespace->addSignalHandler(signalHandler);
        signalHandler->m_lineName = name;
        m_lineToSignalHandler.insert(name, signalHandler);
        m_scene->addItem(whitespace);
        //signalHandler->setLine(whitespace);

        p1White += whiteOffset;
        p2White += whiteOffset;
    }
    createLedgerLines(QPoint(p1White.x(), p2White.y() - whiteOffset.y()));

}

void MainWindow::createLedgerLines(QPoint startPoint)
{

    using namespace staffLayout;

    QPen staffPen(Qt::black);
    staffPen.setWidth(lineHeight);
    QPoint p1Staff(0, 0);
    QPoint p2Staff(lineLength, lineHeight);
    p1Staff += startPoint;
    p2Staff += startPoint;

    QPoint p1White(0, lineHeight + onePixel );
    QPoint p2White(lineLength, whitespaceHeight+lineHeight - onePixel);
    p1White += startPoint;
    p2White += startPoint;

    QPoint staffOffset(0, whitespaceHeight + lineHeight);
    QPoint whiteOffset(0, lineHeight + whitespaceHeight);

    for(int line=numLedgerLines-1; line>=0; --line){
        //QLine staffLine(p1, p2);
        //m_scene->addLine(staffLine, staffPen);
        StickyLine* staffLine = new StickyLine(p1Staff, p2Staff);
        StickyLineSignalHandler* signalHandler = new StickyLineSignalHandler(this);
        QVariant lineStr = line;
        QString name = objectPropertyTypes::lineType + lineStr.toString();

        staffLine->setColour( QColor(Qt::white) );
        staffLine->setHighlight(QColor(colours::highlighted));
        staffLine->setData(objectPropertyKeys::type, objectPropertyTypes::lineType);
        staffLine->setData(objectPropertyKeys::name, name);
        staffLine->setData(objectPropertyKeys::ledgerType, ledgerType::ledger);
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
        whitespace->setData(objectPropertyKeys::ledgerType, ledgerType::ledger);
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
*/
void MainWindow::setBounds()
{
 //   QRect sceneOrigRect = m_scene->sceneRect().toRect();
    QPoint p1(trebleClef::offset, staffLayout::upperBounds);
    QPoint p2(staffLayout::lineLength+(-trebleClef::offset), staffLayout::lowerBounds);
    QRect sceneRect(p1, p2);
    m_scene->setSceneRect(sceneRect);
}

void MainWindow::makeMap()
{   
    //int totalNumLines = staffLayout::numLedgerLines*2 + staffLayout::numStaffLines;
    QList<QChar> noteLetters{'F', 'E', 'D', 'C', 'B', 'A', 'G'}; // top staff note first descending
    QList<QString> totalLetters;
    int numLedgerNotes = staffLayout::numLedgerLines*2;
    int numStaffNotes = staffLayout::numStaffLines*2;
    int totalNotes = (numLedgerNotes*2)+numStaffNotes;
    int noteIndex = noteLetters.size() - numLedgerNotes;
    int highA_Index = numLedgerNotes+5;

    for(int i=0; i<totalNotes; ++i, ++noteIndex){
        QChar letter = noteLetters.at( noteIndex % noteLetters.size() );
        totalLetters.push_back(letter);
    }


    for(int i=highA_Index; i>=0; --i){
        int note = highA_Index-i;
        int octave = (note/7)+1;
        QString letter = totalLetters.at(i);
        for(int j=0; j<octave; ++j){
            letter += "'";
        }
        totalLetters.replace(i, letter);
    }
    for(int i=highA_Index+1; i<totalNotes; ++i){
        int note = i-(highA_Index+1);
        int octave = (note/7);
        QString letter = "";
        for(int j=0; j<octave; ++j){
            letter += "'";
        }
        letter += totalLetters.at(i);
        totalLetters.replace(i, letter);
    }

    for(int i=0; i<totalNotes; ++i){
        m_lineToNoteMap.insert(i-numLedgerNotes, totalLetters.at(i));
    }

}

void MainWindow::nextRound()
{/*
    QList<QGraphicsItem*> lines = getLines();
    int randNum = qrand() % lines.size();
    QVariant name = lines.at(randNum)->data(objectPropertyKeys::name);
    QString nameStr = name.toString();    
    m_answer = m_noteLineMap.value(nameStr, "");
    ui->guiChallenge->setText("Find the note: " + m_answer); */
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
/*
void MainWindow::userNoteMoved(QString line)
{    
    qDebug() << line;
    if(m_noteLineMap.value(line) == m_answer){

        m_lineToSignalHandler.value(line)->userResult(true);
        QString sound = ":/audio/"+m_answer+".wav";        
        QSound::play(sound);
        ui->score->setValue(ui->score->value() + 1);
        if(ui->score->value() == ui->score->maximum())
            ui->stackedWidget->setCurrentIndex(1);
        nextRound();

    }
    else{        
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
    }

}*/

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
