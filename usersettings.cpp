#include "usersettings.h"
#include <QSettings>

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

void UserSettings::write()
{
    QSettings settings;
    settings.beginGroup("users");
        settings.beginGroup( m_userName );
            settings.setValue("totalBeats", m_score);
        settings.endGroup();
    settings.endGroup();
    settings.sync();
}
