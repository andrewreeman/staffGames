#include "title.h"
#include "ui_title.h"
#include <QSizePolicy>

Title::Title(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Title)
{
    ui->setupUi(this);
    centralWidget()->layout()->setContentsMargins(0, 0, 0, 0);
    m_main = new MainWindow(this);
    ui->verticalLayout_2->addWidget(m_main);



    //this->setCentralWidget(m_main);
  //  m_main->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    /*QSizePolicy sizePolicy;
    sizePolicy.setHorizontalPolicy(QSizePolicy::Expanding);
    sizePolicy.setVerticalPolicy(QSizePolicy::Expanding);
    m_main->setSizePolicy(sizePolicy);*/
    //m_main->showMaximized();

}

Title::~Title()
{
    delete ui;
}
