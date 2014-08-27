#ifndef TITLE_H
#define TITLE_H

#include <QMainWindow>
#include "mainwindow.h"

namespace Ui {
class Title;
}

class Title : public QMainWindow
{
    Q_OBJECT

public:
    explicit Title(QWidget *parent = 0);
    ~Title();

private:
    Ui::Title *ui;
    MainWindow* m_main;
};

#endif // TITLE_H
