#ifndef Game_NoteFinding_H
#define Game_NoteFinding_H

#include <QWidget>
#include <QGraphicsScene>
#include <QMap>

#include "usersettings.h"
#include "staffscene.h"

//TODO multi-touch resize!
//TODO bass clef game
//TODO android test
//TODO half sample size on wavs?
//TODO title graphics

namespace Ui {
class Game_NoteFinding;
}

class Game_NoteFinding : public QWidget
{
    Q_OBJECT

public:
    explicit Game_NoteFinding(QWidget *parent = 0);
    ~Game_NoteFinding();

signals:           
    void stopGame();

private slots:

    void lineSelected(int line);    
    void on_pushButton_clicked();

private:

    void makeMap();
    void correct();
    void incorrect();
    void nextRound();
    QList<QGraphicsItem*> getLines();
    void writeUserSettings();
    virtual void setSelectableLines();


    Ui::Game_NoteFinding *ui;
    StaffScene* m_scene; 
    QMap<int, QString> m_lineToNoteMap;    
    int m_answer;
    UserSettings* m_user;
    QList<int> m_selectableLines;

};

#endif // Game_NoteFinding_H
