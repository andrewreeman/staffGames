#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QMap>
#include "staffscene.h"

//TODO midi audio: not easily possibly. Instead think about audio synthesis.
//TODO multi-touch resize!
//TODO bass clef
//TODO launch from title

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

signals:        

private slots:

    void lineSelected(int line);    
    void on_pushButton_clicked();

private:

    void makeMap();
    void correct();
    void incorrect();
    void nextRound();
    QList<QGraphicsItem*> getLines();

    Ui::MainWindow *ui;    
    StaffScene* m_scene; 
    QMap<int, QString> m_lineToNoteMap;    
    int m_answer;



};

#endif // MAINWINDOW_H
