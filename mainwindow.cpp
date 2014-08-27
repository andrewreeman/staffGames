#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "staffGamesConstants.h"

#include <QLine>
#include <QRect>
#include <QGraphicsItem>
#include <QScrollBar>
#include <QMouseEvent>
#include <QTime>
#include <QPointF>

#include <QDebug>
#include <QSound>
#include <QMediaPlayer>
#include <QFileInfo>


MainWindow::MainWindow(QWidget *parent) :
    //QMainWindow(parent),
    QWidget(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_scene = new StaffScene(this);
    ui->graphicsView->setScene(m_scene);
    ui->score->setValue(0);
    makeMap();
    nextRound();
    ui->graphicsView->setDragMode(QGraphicsView::ScrollHandDrag);
    connect(m_scene, SIGNAL( lineSelected(int) ), this, SLOT( lineSelected(int) ));
}

MainWindow::~MainWindow()
{
    qDebug() << "closed";
    delete ui;
}

void MainWindow::makeMap()
{
    QList<QChar> noteLetters{'F', 'E', 'D', 'C', 'B', 'A', 'G'}; // top staff note first descending
    QList<QString> totalLetters;
    int numLedgerNotes = staffLayout::numLedgerLines*2;
    int numStaffNotes = staffLayout::numStaffLines*2;
    int totalNotes = (numLedgerNotes*2)+numStaffNotes;
    int noteIndex = noteLetters.size() - numLedgerNotes;    
    int currentOctave = 3;

    QStringList octaves;
    octaves << "contra" << "double low" << "low" << "bass" << "middle" << "treble" << "top";

    auto f_initTotalLetters = [&](){
        for(int i=0; i<totalNotes; ++i, ++noteIndex){
            QChar letter = noteLetters.at( noteIndex % noteLetters.size() );
            totalLetters.push_back(letter);
        }
    };


    auto f_octavizeNotes = [&](){
        for(int i=totalLetters.size()-1; i>=0; --i){
            QString letter = totalLetters.at(i);           
            if(letter == "C")
                ++currentOctave;
            letter  = octaves.at(currentOctave) + " " + letter;            
            totalLetters.replace(i, letter);    
        }

    };

    auto f_copyTo_IntLineNumber_Map = [&](){
        for(int i=0; i<totalLetters.size(); ++i){
            m_lineToNoteMap.insert(i-numLedgerNotes, totalLetters.at(i));            
        }
    };

    f_initTotalLetters();    
    f_octavizeNotes();
    f_copyTo_IntLineNumber_Map();
}

void MainWindow::lineSelected(int line)
{
    if(line == m_answer){
        m_scene->setCorrectState(line, true);
        correct();
    }
    else{
        m_scene->setCorrectState(line, false);
        incorrect();
    }
}

void MainWindow::correct()
{

    QString sound = ":/audio/"+ QString("treble-") + QString::number(m_answer) +".wav";
    qDebug() << sound;
    QSound::play(sound);
    ui->score->setValue(ui->score->value() + 1);
    nextRound();
    if(ui->score->value() == ui->score->maximum())
        ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::incorrect()
{
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
    this->hide();
    this->close();
}

void MainWindow::nextRound()
{
    qsrand(QTime::currentTime().msec());
    m_answer = ( qrand() % m_lineToNoteMap.size() ) - staffLayout::numLedgerLines*2 ;
    ui->guiChallenge->setText("Find the note: " + m_lineToNoteMap.value(m_answer));
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

void MainWindow::on_pushButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
    ui->score->setValue(0);
}
