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

private slots:

    void userButtonClicked(int userIndex);

    void on_titleToLogin_clicked();

    void on_startGame_clicked();

    void on_AddUser_clicked();

private:

    QList<UserSettings> getAllUserSettings();
    bool addUser(QString newUser);
    void makeAllUserButtons();
    void makeUserButton(int userIndex);
    void writeSettings();

    Ui::Title *ui;

    QList<ButtonRelay*> m_userButtonRelays;
    UserSettings m_user;
    QList<UserSettings> m_allUsers;
};

#endif // TITLE_H
