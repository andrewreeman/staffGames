#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "staffGamesConstants.h"

#include "game_notefinding.h"
#include "game_linefinding.h"
#include "game_spacefinding.h"

#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), m_game(0), m_title(0),
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
    removeWidget(m_title);
    disconnect(m_title, SIGNAL(setUser(UserSettings)), this, SLOT(setUser(UserSettings)));
    initGame(gameId);
}

void MainWindow::stopGame()
{
    removeWidget(m_game);
    initTitle();
}

void MainWindow::setUser(UserSettings user)
{
    m_user = user;
}

void MainWindow::removeWidget(QWidget *widget)
{
    ui->gameContainer->removeWidget(widget);
    widget->hide();
    widget->close();
}

void MainWindow::initGame(int gameId)
{
    m_game = m_gameFactory.createGame(gameId, this);
    m_game->setAttribute(Qt::WA_DeleteOnClose);
    ui->gameContainer->addWidget(m_game);
    connect(m_game, SIGNAL(stopGame()), this, SLOT(stopGame()));
    m_game->startGame();
}

void MainWindow::initTitle()
{
    m_title = new Title(this);
    m_title->setAttribute(Qt::WA_DeleteOnClose);
    ui->gameContainer->addWidget(m_title);
    connect(m_title, SIGNAL(startGame(int)), this, SLOT(startGame(int)));

    connect(m_title, SIGNAL(setUser(UserSettings)), this, SLOT(setUser(UserSettings)));
}
