#include "mainwindow.h"
#include "ui_mainwindow.h"

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

void MainWindow::startGame()
{    
    removeWidget(m_title);
    initGame();
}

void MainWindow::stopGame()
{
    removeWidget(m_main);
    initTitle();
}
//TODO keeping score
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
    m_title = new Title(this);
    m_title->setAttribute(Qt::WA_DeleteOnClose);
    ui->verticalLayout_2->addWidget(m_title);
    connect(m_title, SIGNAL(startGame()), this, SLOT(startGame()));
}
