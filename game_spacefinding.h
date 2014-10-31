#ifndef GAME_SPACEFINDING_H
#define GAME_SPACEFINDING_H

#include "game_notefinding.h"

class Game_SpaceFinding : public Game_NoteFinding
{
    Q_OBJECT
public:
    explicit Game_SpaceFinding(QWidget *parent = 0);
    virtual void setSelectableLines();
signals:

public slots:

};

#endif // GAME_SPACEFINDING_H
