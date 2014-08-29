#include "renamemetotitle.h"
#include "ui_renamemetotitle.h"
#include <QDebug>

RenameMeToTitle::RenameMeToTitle(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RenameMeToTitle)
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(0);
}

RenameMeToTitle::~RenameMeToTitle()
{
    qDebug() << "Title destroyed";
    delete ui;
}



void RenameMeToTitle::on_titleToLogin_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void RenameMeToTitle::on_loginToGames_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
}

void RenameMeToTitle::on_startGame_clicked()
{
    emit startGame();
}
