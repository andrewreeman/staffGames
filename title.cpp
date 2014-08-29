#include "title.h"
#include "ui_title.h"
//#include <QSizePolicy>
#include <QDebug>


Title::Title(QWidget *parent) :
    QMainWindow(parent), m_main(0), m_renameMeToTitle(0),
    ui(new Ui::Title)
{
    ui->setupUi(this);
    centralWidget()->layout()->setContentsMargins(0, 0, 0, 0);
    initTitle();
}

Title::~Title()
{
    delete ui;
}

void Title::startGame()
{    
    removeWidget(m_renameMeToTitle);    
    initGame();
}

void Title::killGame()
{
    removeWidget(m_main);
    initTitle();
}

void Title::removeWidget(QWidget *widget)
{
    ui->verticalLayout_2->removeWidget(widget);
    widget->hide();
    widget->close();
}

void Title::initGame()
{
    m_main = new MainWindow(this);
    m_main->setAttribute(Qt::WA_DeleteOnClose);
    ui->verticalLayout_2->addWidget(m_main);
    connect(m_main, SIGNAL(killMe()), this, SLOT(killGame()));
}

void Title::initTitle()
{
    m_renameMeToTitle = new RenameMeToTitle(this);
    m_renameMeToTitle->setAttribute(Qt::WA_DeleteOnClose);
    ui->verticalLayout_2->addWidget(m_renameMeToTitle);
    connect(m_renameMeToTitle, SIGNAL(startGame()), this, SLOT(startGame()));
}
