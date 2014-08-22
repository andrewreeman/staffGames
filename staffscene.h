#ifndef STAFFSCENE_H
#define STAFFSCENE_H

#include <QObject>
#include <QGraphicsScene>
#include <QGraphicsItem>

#include "note.h"
#include "linemanager.h"


class StaffScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit StaffScene(QObject *parent = 0);
    void selectLine(int lineNumber);

signals:
    void lineSelected(int lineNumber);
public slots:

private slots:
    void unselectLine();

private:
    void makeLine();
    void makeCircle();
    void setTargetLine();
    void setNoteY(int selectedLineNumber);


    Note* m_note;
    lineManager* m_line;

    int m_targetLine;
    int m_selectedLine;

};

#endif // STAFFSCENE_H
