#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QMap>

#include "stickyline.h"
#include "stickynote.h"
#include "stickylinesignalhandler.h"

#ifdef QT_DEBUG
    //#define MOUSE_TRACKING
#endif

#ifdef MOUSE_TRACKING
    #include <QTimer>
#endif




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
    void userResult(bool);
    void scrollFinished();

private slots:

#ifdef MOUSE_TRACKING
    void getMousePos();
#endif

    void userNoteMoved(QString line);
    void scrollDown();
    void scrollUp();
    void on_pushButton_clicked();

private:
    void createStaff();
    void createNote();
    void setBounds();
    void makeMap();
    void nextRound();
    QList<QGraphicsItem*> getLines();

    Ui::MainWindow *ui;
    QGraphicsScene* m_scene;

    QMap<QString, QString> m_noteLineMap;
    QString m_answer;
    QMap<QString, StickyLineSignalHandler*> m_lineToSignalHandler;





#ifdef MOUSE_TRACKING
    QTimer* m_mousePosTrigger;
#endif

};

#endif // MAINWINDOW_H
