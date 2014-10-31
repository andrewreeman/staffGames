#ifndef GAME_H
#define GAME_H

#include <QWidget>

class Game : public QWidget
{
    Q_OBJECT
public:
    explicit Game(QWidget *parent = 0);
    virtual void startGame() = 0;

signals:
    void stopGame();

public slots:

};

#endif // GAME_H
