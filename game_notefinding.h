#ifndef Game_NoteFinding_H
#define Game_NoteFinding_H

//#include <QMainWindow>
#include <QWidget>
#include <QGraphicsScene>
#include <QMap>
#include "staffscene.h"

//TODO multi-touch resize!
//TODO bass clef game
//TODO half sample size on wavs?
//TODO title graphics

namespace Ui {
class Game_NoteFinding;
}

//class MainWindow : public QMainWindow
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

    Ui::Game_NoteFinding *ui;
    StaffScene* m_scene; 
    QMap<int, QString> m_lineToNoteMap;    
    int m_answer;

};

#endif // Game_NoteFinding_H