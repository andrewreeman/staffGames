#include "game_notefinding.h"
#include "ui_game_notefinding.h"
#include "staffGamesConstants.h"
#include "exceptions.h"

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
#include <QSettings>

#include "mainwindow.h"

Game_NoteFinding::Game_NoteFinding(QWidget *parent) :
    Game(parent),
    ui(new Ui::Game_NoteFinding), m_scene(nullptr), m_user(nullptr)
{    
}

Game_NoteFinding::~Game_NoteFinding()
{    
    delete ui;
}

void Game_NoteFinding::startGame()
{
    ui->setupUi(this);
    m_user = ( (MainWindow*)parent() )->getUser();
    m_scene = new StaffScene(ui->graphicsView, this);

    if(!m_scene) throw Except_MemoryAlloc(Q_FUNC_INFO);

    ui->score->setValue(0);
    makeMap();
    setSelectableLines();
    nextRound();
    ui->graphicsView->setDragMode(QGraphicsView::ScrollHandDrag);
    connect(m_scene, SIGNAL( lineSelected(int) ), this, SLOT( lineSelected(int) ));
}
//TODO check all project for possible errors. Try and catch works so far
void Game_NoteFinding::addSelectableLine(int lineIndex)
{
    int totalNumLedgers = staffLayout::numLedgerNotes;
    int lowerRange = -totalNumLedgers;
    int higherRange = m_lineToNoteMap.size() - totalNumLedgers;

    if(lineIndex >= lowerRange && lineIndex <= higherRange){
        m_selectableLines.push_back(lineIndex);
    };
}

void Game_NoteFinding::makeMap()
{
    QList<QChar> noteLetters{'F', 'E', 'D', 'C', 'B', 'A', 'G'}; // top staff note first descending
    QList<QString> totalLetters;
    int numLedgerNotes = staffLayout::numLedgerNotes;
    int numStaffNotes = staffLayout::numStaffLines;
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

void Game_NoteFinding::lineSelected(int line)
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

void Game_NoteFinding::correct()
{


    QString sound = ":/audio/"+ QString("treble-") + QString::number(m_answer) +".wav";        
    QSound::play(sound);
    ui->score->setValue(ui->score->value() + 1);
    nextRound();
    if(ui->score->value() == ui->score->maximum())
        ui->stackedWidget->setCurrentIndex(1);
}

void Game_NoteFinding::incorrect()
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

void Game_NoteFinding::nextRound()
{
    auto getRandLine = [&](){
        if(m_selectableLines.size() <= 1) return m_selectableLines.at(0);

        int newAns;
        do{
            int randIndex = qrand() % m_selectableLines.size();
            newAns = m_selectableLines.at(randIndex);
        }
        while(newAns == m_answer);
        return newAns;
    };

    qsrand(QTime::currentTime().msec());   
    m_answer = getRandLine();
    ui->guiChallenge->setText("Find the note: " + m_lineToNoteMap.value(m_answer));
}

QList<QGraphicsItem *> Game_NoteFinding::getLines()
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

void Game_NoteFinding::setSelectableLines()
{
    // all lines
    for(int i=0; i<m_lineToNoteMap.size(); ++i){
        addSelectableLine( i - (staffLayout::numLedgerNotes*2));
    };
}


void Game_NoteFinding::on_pushButton_clicked()
{

    m_user->addScore(ui->score->value());
    m_user->write();
    emit stopGame();
}
