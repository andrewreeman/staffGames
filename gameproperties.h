#ifndef GAMEPROPERTIES_H
#define GAMEPROPERTIES_H

#include <QString>
#include <QDebug>
#include "staffGamesConstants.h"

class GameProperties
{
public:
    GameProperties(){}
    virtual ~GameProperties(){}

    virtual int gameId() = 0;
    virtual QString gameTitle() = 0;
    virtual int price() = 0;

};

class GameProperties_NoteFindingAll : public GameProperties{
public:
    GameProperties_NoteFindingAll(){}
    ~GameProperties_NoteFindingAll(){}

    int gameId(){ return gameIDs::noteFinderAll;}
    QString gameTitle(){ return QString("Finding all notes");}
    int price(){ return gamePrices::noteFinderAll;}
};

class GameProperties_LineFinding : public GameProperties{
public:
  GameProperties_LineFinding(){}
  ~GameProperties_LineFinding(){}

  int gameId(){ return gameIDs::noteFinderLines;}
  QString gameTitle(){ return QString("Finding all staff lines");}
  int price(){ return gamePrices::noteFinderLines;}
};

class GameProperties_SpaceFinding : public GameProperties{
public:
    GameProperties_SpaceFinding(){}
    ~GameProperties_SpaceFinding() {}

    int gameId(){ return gameIDs::noteFinderSpaces;}
    QString gameTitle(){ return QString("Finding all staff spaces");}
    int price(){ return gamePrices::noteFinderSpaces;}
};
#endif // GAMEPROPERTIES_H
