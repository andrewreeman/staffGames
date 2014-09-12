#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), m_main(0), m_renameMeToTitle(0),
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

void MainWindow::startGame()
{    
    removeWidget(m_renameMeToTitle);    
    initGame();
}

void MainWindow::stopGame()
{
    removeWidget(m_main);
    initTitle();
}

void MainWindow::removeWidget(QWidget *widget)
{
    ui->verticalLayout_2->removeWidget(widget);
    widget->hide();
    widget->close();
}

void MainWindow::initGame()
{
    m_main = new Game_NoteFinding(this);
    m_main->setAttribute(Qt::WA_DeleteOnClose);
    ui->verticalLayout_2->addWidget(m_main);
    connect(m_main, SIGNAL(stopGame()), this, SLOT(stopGame()));
}

void MainWindow::initTitle()
{
    m_renameMeToTitle = new Title(this);
    m_renameMeToTitle->setAttribute(Qt::WA_DeleteOnClose);
    ui->verticalLayout_2->addWidget(m_renameMeToTitle);
    connect(m_renameMeToTitle, SIGNAL(startGame()), this, SLOT(startGame()));
}
