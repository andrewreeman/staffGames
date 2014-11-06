#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QList>

#include <QStringList>

#include "game.h"
#include "title.h"
#include "usersettings.h"
#include "gamefactory.h"

namespace Ui {
class MainWindow;
}

class Game;
class Title;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void setUser(QString userName);

    //user operations
    QString getUserName()const{return m_user.name();}
    int getUserScore()const{return m_user.score();}
    QList<int> getUserOwnedGames()const{return m_user.ownedGames();}
    void addUserScore(int accumulator){ m_user.addScore(accumulator);}
    void writeUserSettings(){m_user.write();}
    void addUserOwnedGame(int gameID){ m_user.addOwnedGame(gameID); }
    QStringList getAllUserNames(){ return m_userNames; }
    bool addUser(QString userName);
    bool removeUser(QString userName);

public slots:
    void startGame(int);
    void stopGame();
private:
    void removeWidget(QWidget* widget);
    void initGame(int gameId);
    void initTitle();
    void initUserNames();

    GameFactory m_gameFactory;
    Game* m_game;
    Title* m_title;
    UserSettings m_user;
    Ui::MainWindow *ui;
    QStringList m_userNames;

};

#endif // MAINWINDOW_H
