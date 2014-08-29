#include "title.h"
#include "ui_renamemetotitle.h"
#include <QDebug>

Title::Title(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RenameMeToTitle)
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(0);
}

Title::~Title()
{
    qDebug() << "Title destroyed";
    delete ui;
}



void Title::on_titleToLogin_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void Title::on_loginToGames_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
}

void Title::on_startGame_clicked()
{
    emit startGame();
}
