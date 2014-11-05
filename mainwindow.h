#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>

#include "game.h"
#include "title.h"
#include "usersettings.h"
#include "gamefactory.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    UserSettings* getUser(){
        //TODO del me m_user = new UserSettings("bob", 0);
       qDebug() <<  m_user->name();

        return m_user;
    }
public slots:
    void startGame(int);
    void stopGame();
    void setUser(UserSettings user);
private:
    void removeWidget(QWidget* widget);
    void initGame(int gameId);
    void initTitle();

    GameFactory m_gameFactory;
    Game* m_game;
    Title* m_title;
    UserSettings* m_user;
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
