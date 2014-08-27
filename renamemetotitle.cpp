#include "renamemetotitle.h"
#include "ui_renamemetotitle.h"

RenameMeToTitle::RenameMeToTitle(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RenameMeToTitle)
{
    ui->setupUi(this);
}

RenameMeToTitle::~RenameMeToTitle()
{
    delete ui;
}

void RenameMeToTitle::on_pushButton_clicked()
{
    emit startGame();
}
