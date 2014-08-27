#include "title.h"
#include "ui_title.h"
#include <QSizePolicy>


Title::Title(QWidget *parent) :
    QMainWindow(parent), m_main(0), m_renameMeToTitle(0),
    ui(new Ui::Title)
{
    ui->setupUi(this);
    centralWidget()->layout()->setContentsMargins(0, 0, 0, 0);
    m_renameMeToTitle = new RenameMeToTitle(this);
    ui->verticalLayout_2->addWidget(m_renameMeToTitle);
    connect(m_renameMeToTitle, SIGNAL(startGame()), this, SLOT(startGame()));
}

Title::~Title()
{
    delete ui;
}

void Title::startGame()
{
    removeWidget(m_renameMeToTitle);
    m_main = new MainWindow(this);
    ui->verticalLayout_2->addWidget(m_main);
    connect(m_main, SIGNAL(stopGame()), this, SLOT(killGame()));
}

void Title::killGame()
{
    //removeWidget(m_main);
        ui->verticalLayout_2->removeWidget(m_main);
    m_renameMeToTitle = new RenameMeToTitle(this);
    ui->verticalLayout_2->addWidget(m_renameMeToTitle);
}

void Title::removeWidget(QWidget *widget)
{
    ui->verticalLayout_2->removeWidget(widget);
    widget->hide();
    widget->close();
}
