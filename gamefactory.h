#ifndef GAMEFACTORY_H
#define GAMEFACTORY_H

#include "game.h"

class GameFactory
{

public:
    GameFactory();
    Game* createGame(int gameID, QWidget *parent);
private:
    typedef Game* (*gameFunctionPointer) (QWidget*);

    QList< gameFunctionPointer > m_creators;

};

#endif // GAMEFACTORY_H
