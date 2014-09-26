#ifndef TITLE_H
#define TITLE_H

#include <QWidget>
#include "usersettings.h"
#include "buttonrelay.h"

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
    void startGame();    
    void setUser(UserSettings user);
private slots:

    void userButtonClicked(QString userName);

    void on_titleToLogin_clicked();

    void on_startGame_clicked();

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
    void addMenu();

    Ui::Title *ui;

    QList<ButtonRelay*> m_userButtonRelays;
    QList<QPushButton*> m_userPushButtons;
    QList<UserSettings> m_allUsers;
};

#endif // TITLE_H
