#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "staffGamesConstants.h"

#include "exceptions.h"
#include <QMessageBox>

#include "game_notefinding.h"
#include "game_linefinding.h"
#include "game_spacefinding.h"

#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), m_game(nullptr), m_title(nullptr), m_user(nullptr),
    ui(new Ui::MainWindow)
{    

    ui->setupUi(this);
    centralWidget()->layout()->setContentsMargins(0, 0, 0, 0);
    initTitle();
    this->showMaximized();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::startGame(int gameId)
{    
    try{
        qDebug() << m_user->name();
        removeWidget(m_title);
        disconnect(m_title, SIGNAL(setUser(UserSettings)), this, SLOT(setUser(UserSettings)));
        initGame(gameId);
    }
    catch(Except_StaffGames &e){
        m_game = nullptr;
        initTitle();
        QMessageBox::critical(this, "Error", QString() + "Error starting game.\n" + e.what());
    }
}

void MainWindow::stopGame()
{
    removeWidget(m_game);
    m_game = nullptr;
    initTitle();
}

void MainWindow::setUser(UserSettings user)
{

    if(m_user){
        delete m_user;
        m_user = nullptr;
    }
    m_user = new UserSettings("bob", 0);
}

void MainWindow::removeWidget(QWidget *widget)
{
    ui->gameContainer->removeWidget(widget);
    widget->hide();
    widget->close();
}

void MainWindow::initGame(int gameId)
{
    try{
        m_game = m_gameFactory.createGame(gameId, this);
        m_game->setAttribute(Qt::WA_DeleteOnClose);
        ui->gameContainer->addWidget(m_game);
        connect(m_game, SIGNAL(stopGame()), this, SLOT(stopGame()));
        m_game->startGame();
    }
    catch(Except_MemoryAlloc &e){
        qCritical() << "Error: Could not allocate memory for game of gameID " << gameId << "\n";
        throw;
    }
    catch(Except_OutOfBounds &e){
        qCritical() << "Error: gameID " << gameId << " is not a valid gameID" << "\n";
        throw;
    }
    catch(Except_StaffGames &e){
        throw;
    }
    catch(...){
        throw;
    }
}

void MainWindow::initTitle()
{
    m_title = new Title(this);
    m_title->setAttribute(Qt::WA_DeleteOnClose);
    ui->gameContainer->addWidget(m_title);
    connect(m_title, SIGNAL(startGame(int)), this, SLOT(startGame(int)));
    connect(m_title, SIGNAL(setUser(UserSettings)), this, SLOT(setUser(UserSettings)));
}



/*
 * TODO
 *  All userDetails are ONLY stored in MainWindow
 * MainWindow{
 *  MainWindow::(){
 *      new Title()
 * }
 *
 *  userInfo
 *
 * setUser(userName){
 *
 *  userInfo = getUserSettingsFromUserName(userName)
 *   if(userInfo.isValid)
 *      delete userDetails
 *      new userDetails(userInfo)
 * }
 *
 * startGame{
 *  game->start(this);
 * delete title;
 * }
 *
 * stopGame{
 *  delete game;
 * new Title(userDetails);
 *
 * }
 *
 *
 * }
 *
 * Title{
 *      MainWindow* main;
 *
 *      main->userScore;
 *       main->userAddScore
 *
 *
 *      setUser(
 *          main->setUser(userName)
 *      }
 *
 *      Title::(){....as normal}
 *      Title::(UserSettings){ start with selected user home page}
 *
 * }
 *
 *Game{
 *  start(MainWindow* parent){
 *      userName = parent->userName
 * }
 *
 * }
 *
 *
 *
 * */
