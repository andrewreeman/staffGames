#include "usersettings.h"
#include "staffGamesConstants.h"
#include "exceptions.h"

#include <QSettings>

UserSettings::UserSettings() :
    m_userName("default"), m_score(-1)
{
    m_ownedGames.push_back(gameIDs::noteFinderSpaces);
}

UserSettings::UserSettings(QString name, int score) :
    m_userName(name), m_score(score)
{
    m_ownedGames.push_back(gameIDs::noteFinderSpaces);
}

UserSettings::UserSettings(const UserSettings &otherUser)
{
    //TODO const functions
    this->m_userName = otherUser.name();
    this->m_score = otherUser.score();
    this->m_ownedGames = otherUser.ownedGames();
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

void UserSettings::addOwnedGame(int gameId){

    if(gameId>=0 && gameId<gameIDs::numGames){
        if(m_ownedGames.contains(gameId)) return;
        m_ownedGames.push_back(gameId);
    }
    else
        throw Except_OutOfBounds(Q_FUNC_INFO);
}

void UserSettings::write()
{
    QSettings settings;
    settings.beginGroup(userSettingsKeys::users);
        settings.beginGroup( m_userName );
            settings.setValue(userSettingsKeys::totalBeats, m_score);
            settings.beginGroup(userSettingsKeys::ownedGames);
                for(int game : m_ownedGames){
                    settings.setValue(QString::number(game), true);
                }
            settings.endGroup();
        settings.endGroup();
    settings.endGroup();
    settings.sync();
}
