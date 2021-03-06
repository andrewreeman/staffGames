#ifndef USERSETTINGS_H
#define USERSETTINGS_H

#include <QString>
#include <QList>

class UserSettings
{
public:
    UserSettings();
    UserSettings(QString name, int score);    

    QString name()const{return m_userName;}
    void setName(QString name){m_userName = name;}

    int score()const{return m_score;}
    void addScore(int addToScore);
    void setScore(int score);

    void addOwnedGame(int gameId);
    QList<int> ownedGames()const{return m_ownedGames;}

    void write();

private:
    QString m_userName;
    int m_score;
    QList<int> m_ownedGames;
};

#endif // USERSETTINGS_H
