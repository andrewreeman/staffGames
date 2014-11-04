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

    virtual int getGameId() = 0;
    virtual QString getGameTitle() = 0;
    virtual int getPrice() = 0;

};

class GameProperties_NoteFindingAll : public GameProperties{
public:
    GameProperties_NoteFindingAll(){}
    ~GameProperties_NoteFindingAll(){}

    int getGameId(){ return gameIDs::noteFinderAll;}
    QString getGameTitle(){ return QString("Finding all notes");}
    int getPrice(){ return gamePrices::noteFinderAll;}
};

class GameProperties_LineFinding : public GameProperties{
public:
  GameProperties_LineFinding(){}
  ~GameProperties_LineFinding(){}

  int getGameId(){ return gameIDs::noteFinderLines;}
  QString getGameTitle(){ return QString("Finding all staff lines");}
  int getPrice(){ return gamePrices::noteFinderLines;}
};

class GameProperties_SpaceFinding : public GameProperties{
public:
    GameProperties_SpaceFinding(){}
    ~GameProperties_SpaceFinding() {}

    int getGameId(){ return gameIDs::noteFinderSpaces;}
    QString getGameTitle(){ return QString("Finding all staff spaces");}
    int getPrice(){ return gamePrices::noteFinderSpaces;}
};
#endif // GAMEPROPERTIES_H
