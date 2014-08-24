#ifndef STAFFSCENE_H
#define STAFFSCENE_H

#include <QObject>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QPixmap>
#include <QGraphicsPixmapItem>

#include "note.h"
#include "linemanager.h"


class StaffScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit StaffScene(QObject *parent = 0);    
    void selectLine(int lineNumber);
    void setCorrectState(int lineNumber, bool correctState);
    StaffLine* getLine(int lineNumber);

signals:
    void lineSelected(int lineNumber);
public slots:

private slots:
    void unselectLine();

private:
    void makeLine();
    void makeNote();
    void makeTrebleClef();
    void setBoundries();
    void setNoteY(int selectedLineNumber);    

    Note* m_note;
    lineManager* m_lineManager;
    int m_selectedLine;
    QGraphicsPixmapItem* m_treble;
};

#endif // STAFFSCENE_H
