#ifndef TITLE_H
#define TITLE_H

#include <QWidget>
#include <QMap>

#include "usersettings.h"
#include "buttonrelay.h"
#include "gameproperties.h"

namespace Ui {
class Title;
}

class Title : public QWidget
{
    Q_OBJECT

public:
    explicit Title(QWidget *parent = 0);
    ~Title();

signals:
    void startGame(int);
    void startLineFind();
    void setUser(UserSettings user);
private slots:

    void userButtonClicked(QVariant userName);
    void gameButtonClicked(QVariant gameID);
    void on_titleToLogin_clicked();

    void on_AddUser_clicked();

    void removeUser_clicked();
    void removeMenu();

private:

    QList<UserSettings> getAllUserSettings();

    bool addUser(QString newUser);
    bool removeUser(QString user);
    bool isUserExist(QString user);
    int getUserIndex(QString user);

    void makeAllUserButtons();
    void makeUserButton(int userIndex);

    void makeAllGameButtons();
    void makeGameButton(GameProperties *gameProps);

    void addMenu();


    Ui::Title *ui;

    QList<ButtonRelay*> m_userButtonRelays;
    QList<QPushButton*> m_userPushButtons;

    QList<ButtonRelay*> m_gameButtonRelays;
    QList<QPushButton*> m_gamePushButtons;

    QList<UserSettings> m_allUsers;
    UserSettings m_user;
    QMap<int, GameProperties*> m_gameProperties;
};

#endif // TITLE_H
