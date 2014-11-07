#ifndef Game_NoteFinding_H
#define Game_NoteFinding_H

#include <QWidget>
#include <QGraphicsScene>
#include <QMap>

#include "usersettings.h"
#include "staffscene.h"
#include "game.h"
#include "mainwindow.h"

//TODO classes need pointer to user settings not mainwindow

namespace Ui {
class Game_NoteFinding;
}

class MainWindow;

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
    //TODO change name to winButton
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
    MainWindow* m_mainWindow;
};

#endif // Game_NoteFinding_H
