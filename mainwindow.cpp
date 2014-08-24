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


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_scene = new StaffScene(this);    
    ui->graphicsView->setScene(m_scene);        
    ui->score->setValue(0);    
    makeMap();
    nextRound();
    ui->graphicsView->setDragMode(QGraphicsView::ScrollHandDrag);
    QWidget::showMaximized();
    connect(m_scene, SIGNAL( lineSelected(int) ), this, SLOT( lineSelected(int) ));
}

MainWindow::~MainWindow()
{
    delete ui;
}

//TODO make use of lambda functions more.
void MainWindow::makeMap()
{
    QList<QChar> noteLetters{'F', 'E', 'D', 'C', 'B', 'A', 'G'}; // top staff note first descending
    QList<QString> totalLetters;
    int numLedgerNotes = staffLayout::numLedgerLines*2;
    int numStaffNotes = staffLayout::numStaffLines*2;
    int totalNotes = (numLedgerNotes*2)+numStaffNotes;
    int noteIndex = noteLetters.size() - numLedgerNotes;
    int highA_Index = numLedgerNotes+5;

    auto f_initTotalLetters = [&](){
        for(int i=0; i<totalNotes; ++i, ++noteIndex){
            QChar letter = noteLetters.at( noteIndex % noteLetters.size() );
            totalLetters.push_back(letter);
        }
    };

    auto f_processUpperOctaves = [&](){
        for(int i=highA_Index; i>=0; --i){
            int note = highA_Index-i;
            int octave = (note/7)+1;
            QString letter = totalLetters.at(i);
            for(int j=0; j<octave; ++j){
                letter += "'";
            }
            totalLetters.replace(i, letter);
        }
    };

    auto f_processLowerOctaves = [&](){
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
    };

    auto f_copyTo_IntLineNumber_Map = [&](){
        for(int i=0; i<totalNotes; ++i){
            m_lineToNoteMap.insert(i-numLedgerNotes, totalLetters.at(i));
        }
    };

    f_initTotalLetters();
    f_processUpperOctaves();
    f_processLowerOctaves();
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
    QString sound = ":/audio/"+ m_lineToNoteMap.value(m_answer) +".wav";
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
