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

    virtual int gameId()const = 0;
    virtual QString gameTitle()const = 0;
    virtual int price()const = 0;

};

class GameProperties_NoteFindingAll : public GameProperties{
public:
    GameProperties_NoteFindingAll(){}
    ~GameProperties_NoteFindingAll(){}

    int gameId()const{ return gameIDs::noteFinderAll;}
    QString gameTitle()const{ return QString("Finding all notes");}
    int price()const{ return gamePrices::noteFinderAll;}
};

class GameProperties_LineFinding : public GameProperties{
public:
  GameProperties_LineFinding(){}
  ~GameProperties_LineFinding(){}

  int gameId()const{ return gameIDs::noteFinderLines;}
  QString gameTitle()const{ return QString("Finding all staff lines");}
  int price()const{ return gamePrices::noteFinderLines;}
};

class GameProperties_SpaceFinding : public GameProperties{
public:
    GameProperties_SpaceFinding(){}
    ~GameProperties_SpaceFinding() {}

    int gameId()const{ return gameIDs::noteFinderSpaces;}
    QString gameTitle()const{ return QString("Finding all staff spaces");}
    int price()const{ return gamePrices::noteFinderSpaces;}
};
#endif // GAMEPROPERTIES_H
