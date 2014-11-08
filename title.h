#ifndef TITLE_H
#define TITLE_H

#include <QWidget>
#include <QMap>

#include "usersettings.h"
#include "buttonrelay.h"
#include "gameproperties.h"
#include "mainwindow.h"

namespace Ui {
class Title;
}

class MainWindow;

class Title : public QWidget
{
    Q_OBJECT

public:
    explicit Title(QWidget *parent = 0, bool isUserSelected = false);
    ~Title();

signals:
    void startGame(int);
    void startLineFind();
private slots:

    void userButtonClicked(QVariant userName);
    void gameButtonClicked(QVariant gameID);
    void shopButtonClicked(QVariant gameID);
    void on_titleToLogin_clicked();

    void on_AddUser_clicked();    
    void removeUser_clicked();
    void removeMenu();

    void on_shopButton_clicked();

    void on_backToUserGames_clicked();

    void on_backToUserList_clicked();

private:

    bool addUser(QString newUser);
    bool removeUser(QString userName);

    void makeAllUserButtons();
    void makeUserButton(QString name);
    void removeAllUserButtons();

    void makeAllGameButtons();
    void makeGameButton(GameProperties* gameProps);
    void removeAllGameButtons();

    void makeAllShopButtons();
    void makeShopButton(GameProperties* gameProps);
    void removeShopButton(GameProperties* gameProps);
    void removeAllShopButtons();

    void addMenu();

    Ui::Title *ui;

    QList<ButtonRelay*> m_userButtonRelays;
    QList<QPushButton*> m_userPushButtons;

    QList<ButtonRelay*> m_gameButtonRelays;
    QList<QPushButton*> m_gamePushButtons;

    QList<ButtonRelay*> m_shopButtonRelays;
    QList<QPushButton*> m_shopPushButtons;

    QMap<int, GameProperties*> m_gameProperties;

    MainWindow* m_mainWindow;
};

#endif // TITLE_H
