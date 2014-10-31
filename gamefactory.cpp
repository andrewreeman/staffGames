#include "gamefactory.h"

#include "exceptions.h"

#include "game_notefinding.h"
#include "game_linefinding.h"
#include "game_spacefinding.h"
#include <exception>

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
    if(gameID < m_creators.size() && gameID >= 0){
        Game* game = nullptr;
        game = m_creators.at(gameID)(parent);        
        if(game)
            return game;
        throw Except_MemoryAlloc(Q_FUNC_INFO);

    }
    throw Except_OutOfBounds(Q_FUNC_INFO);
}
