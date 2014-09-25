#ifndef USERSETTINGS_H
#define USERSETTINGS_H

#include <QString>

class UserSettings
{
public:
    UserSettings();
    UserSettings(QString name, int score);
    int getScore(){return m_score;}
    QString getName(){return m_userName;}
    void addScore(int addToScore);


private:
    int m_score;
    QString m_userName;
};

#endif // USERSETTINGS_H
