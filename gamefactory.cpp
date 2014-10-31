#include "gamefactory.h"

#include "game_notefinding.h"
#include "game_linefinding.h"
#include "game_spacefinding.h"

GameFactory::GameFactory()
{
    auto noteFinding_All = [](QWidget* parent){
        return (Game*) new Game_NoteFinding(parent);
    };
    auto noteFinding_Lines = [](QWidget* parent){
        return (Game*) new Game_LineFinding(parent);
    };
    auto noteFinding_Spaces = [](QWidget* parent){
        return (Game*) new Game_SpaceFinding(parent);
    };

    m_creators.push_back(noteFinding_All);
    m_creators.push_back(noteFinding_Lines);
    m_creators.push_back(noteFinding_Spaces);

}

Game *GameFactory::createGame(int gameID, QWidget* parent)
{
    return m_creators.at(gameID)(parent);
}
