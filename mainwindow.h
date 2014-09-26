#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>

#include "game_notefinding.h"
#include "title.h"
#include "usersettings.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    UserSettings* getUser(){return &m_user;}
public slots:
    void startGame();
    void stopGame();
    void setUser(UserSettings user);
private:
    void removeWidget(QWidget* widget);
    void initGame();
    void initTitle();

    Game_NoteFinding* m_main;
    Title* m_title;
    UserSettings m_user;
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
