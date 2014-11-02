#include "usersettings.h"
#include "staffGamesConstants.h"
#include "exceptions.h"

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

void UserSettings::setScore(int score)
{
    if(score >=0 )
        m_score = score;
    else
        m_score = 0;
}

void UserSettings::addOwnedGame(int gameId)
{
    switch(gameId){
        case gameIDs::noteFinderAll:
        case gameIDs::noteFinderLines:
        case gameIDs::noteFinderSpaces:
            m_ownedGames.push_back(gameId);
            break;
        default:
            throw Except_OutOfBounds(Q_FUNC_INFO);
    }
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
