#ifndef TITLE_H
#define TITLE_H

#include <QWidget>

namespace Ui {
class RenameMeToTitle;
}

class Title : public QWidget
{
    Q_OBJECT

public:
    explicit Title(QWidget *parent = 0);
    ~Title();

signals:
    void startGame();

private slots:


    void on_titleToLogin_clicked();

    void on_loginToGames_clicked();

    void on_startGame_clicked();

private:
    Ui::RenameMeToTitle *ui;
};

#endif // TITLE_H
