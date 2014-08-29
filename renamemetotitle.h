#ifndef RENAMEMETOTITLE_H
#define RENAMEMETOTITLE_H

#include <QWidget>

namespace Ui {
class RenameMeToTitle;
}

class RenameMeToTitle : public QWidget
{
    Q_OBJECT

public:
    explicit RenameMeToTitle(QWidget *parent = 0);
    ~RenameMeToTitle();

signals:
    void startGame();

private slots:


    void on_titleToLogin_clicked();

    void on_loginToGames_clicked();

    void on_startGame_clicked();

private:
    Ui::RenameMeToTitle *ui;
};

#endif // RENAMEMETOTITLE_H
