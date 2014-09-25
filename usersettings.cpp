#include "usersettings.h"

UserSettings::UserSettings() :
    m_userName("default"), m_score(-1)
{
}

UserSettings::UserSettings(QString name, int score) :
    m_userName(name), m_score(score)
{
}

void UserSettings::addScore(int addToScore)
{
    int newScore = m_score + addToScore;
    m_score = newScore < 0 ? 0 : newScore;
}