#include "game_linefinding.h"
#include <QDebug>

Game_LineFinding::Game_LineFinding(QWidget *parent)
    : Game_NoteFinding(parent)
{
}

void Game_LineFinding::setSelectableLines()
{
    for(int i=0; i<=8; i+=2){
        addSelectableLine(i);
    }
}
