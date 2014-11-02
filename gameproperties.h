#ifndef GAMEPROPERTIES_H
#define GAMEPROPERTIES_H

#include <QString>
#include "staffGamesConstants.h"

class GameProperties
{
public:
    GameProperties();

    virtual int getGameId() = 0;
    virtual QString getGameTitle() = 0;

};

class GameProperties_NoteFindingAll : public GameProperties{
public:
    GameProperties_NoteFindingAll();
    int getGameId(){ return gameIDs::noteFinderAll;}
    QString getGameTitle(){ return QString("Finding all notes");}
};

class GameProperties_LineFinding : public GameProperties{
public:
  GameProperties_LineFinding();
  int getGameId(){ return gameIDs::noteFinderLines;}
  QString getGameTitle(){ return QString("Finding all staff lines");}
};

class GameProperties_SpaceFinding : public GameProperties{
public:
    GameProperties_SpaceFinding();
    int getGameId(){ return gameIDs::noteFinderSpaces;}
    QString getGameTitle(){ return QString("Finding all staff spaces");}
};
#endif // GAMEPROPERTIES_H
