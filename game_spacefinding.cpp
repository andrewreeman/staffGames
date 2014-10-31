#include "game_spacefinding.h"

Game_SpaceFinding::Game_SpaceFinding(QWidget *parent) :
    Game_NoteFinding(parent)
{
}

void Game_SpaceFinding::setSelectableLines()
{
    for(int i=1; i<=7; i+=2){
        addSelectableLine(i);
    }

}
