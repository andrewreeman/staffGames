#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "staffGamesConstants.h"

#include "game_notefinding.h"
#include "game_linefinding.h"
#include "game_spacefinding.h"

#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), m_main(0), m_title(0),
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
    removeWidget(m_main);
    initTitle();
}

void MainWindow::setUser(UserSettings user)
{
    m_user = user;
}

void MainWindow::removeWidget(QWidget *widget)
{
    ui->verticalLayout_2->removeWidget(widget);
    widget->hide();
    widget->close();
}

void MainWindow::initGame(int gameId)
{
    m_main = m_gameFactory.createGame(gameId, this);
    m_main->setAttribute(Qt::WA_DeleteOnClose);
    //TODO verticalLayout_2 is not descriptive
    ui->verticalLayout_2->addWidget(m_main);
    connect(m_main, SIGNAL(stopGame()), this, SLOT(stopGame()));
    m_main->startGame();
}

void MainWindow::initTitle()
{
    m_title = new Title(this);
    m_title->setAttribute(Qt::WA_DeleteOnClose);
    ui->verticalLayout_2->addWidget(m_title);
    connect(m_title, SIGNAL(startGame(int)), this, SLOT(startGame(int)));

    connect(m_title, SIGNAL(setUser(UserSettings)), this, SLOT(setUser(UserSettings)));
}
