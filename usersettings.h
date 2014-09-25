#ifndef USERSETTINGS_H
#define USERSETTINGS_H

#include <QString>

class UserSettings
{
public:
    UserSettings();
    UserSettings(QString name, int score);
    //UserSettings& operator=(const UserSettings&);
    int getScore(){return m_score;}
    QString getName(){return m_userName;}
    void addScore(int addToScore);
    void setScore(int score){m_score = score;}
    void setName(QString name){m_userName = name;}

private:
    int m_score;
    QString m_userName;
};

#endif // USERSETTINGS_H
