#ifndef TITLE_H
#define TITLE_H

#include <QMainWindow>
#include "mainwindow.h"
#include "renamemetotitle.h"

namespace Ui {
class Title;
}

class Title : public QMainWindow
{
    Q_OBJECT

public:
    explicit Title(QWidget *parent = 0);
    ~Title();
public slots:
    void startGame();
    void killGame();
private:
    void removeWidget(QWidget* widget);
    void initGame();
    void initTitle();

    Ui::Title *ui;
    MainWindow* m_main;
    RenameMeToTitle* m_renameMeToTitle;
};

#endif // TITLE_H
