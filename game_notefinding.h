#ifndef Game_NoteFinding_H
#define Game_NoteFinding_H

#include <QWidget>
#include <QGraphicsScene>
#include <QMap>

#include "usersettings.h"
#include "staffscene.h"
#include "game.h"

namespace Ui {
class Game_NoteFinding;
}

class Game_NoteFinding : public Game
{

    Q_OBJECT
public:
    explicit Game_NoteFinding(QWidget *parent = 0);
    ~Game_NoteFinding();
    void addSelectableLine(int lineIndex);
    virtual void startGame();

private slots:
    void lineSelected(int line);    
    void on_pushButton_clicked();

private:
    void makeMap();
    void correct();
    void incorrect();
    void nextRound();
    void writeUserSettings();
    virtual void setSelectableLines();

    Ui::Game_NoteFinding *ui;
    StaffScene* m_scene; 
    QMap<int, QString> m_lineToNoteMap;    
    int m_answer;    
    QList<int> m_selectableLines;
};

#endif // Game_NoteFinding_H
