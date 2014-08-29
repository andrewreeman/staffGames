#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "game_notefinding.h"
#include "title.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
public slots:
    void startGame();
    void stopGame();
private:
    void removeWidget(QWidget* widget);
    void initGame();
    void initTitle();

    Ui::MainWindow *ui;
    Game_NoteFinding* m_main;
    Title* m_renameMeToTitle;
};

#endif // MAINWINDOW_H
