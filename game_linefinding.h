#ifndef GAME_LINEFINDING_H
#define GAME_LINEFINDING_H

#include "game_notefinding.h"

class Game_LineFinding : public Game_NoteFinding
{
    Q_OBJECT

public:
    Game_LineFinding(QWidget *parent=0);
private:
    virtual void setSelectableLines();
};

#endif // GAME_LINEFINDING_H
