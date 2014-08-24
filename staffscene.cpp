#include "staffscene.h"
#include "staffGamesConstants.h"

#include <QDebug>
#include <QTimer>
#include <QGraphicsView>

StaffScene::StaffScene(QObject *parent) :
    QGraphicsScene(parent)
{
    makeLine();
    makeCircle();
    m_note->installSceneEventFilter(m_lineManager);
    setBoundries();
}

void StaffScene::makeLine(){
    m_lineManager = new lineManager();
    this->addItem(m_lineManager);
}

void StaffScene::makeCircle()
{    
    QBrush brush(colours::note);
    QPoint p1(0, 0);    
    QPoint p2(noteProperties::noteDiameter, 0);
    QRectF rec(p1, p2);
    rec.setHeight(noteProperties::noteDiameter);
    m_note = new Note(rec);
    m_note->setFlag(QGraphicsItem::ItemIsMovable, true);
    m_note->moveBy(0, -25);
    m_note->setBrush(brush);
    this->addItem(m_note);
}


void StaffScene::setBoundries()
{


    setSceneRect(rec);
}

void StaffScene::setNoteY(int selectedLineNumber)
{
    QPointF noteCentre = m_note->mapToScene(m_note->boundingRect().center());
    StaffLine* line = m_lineManager->getLine(selectedLineNumber);
    QPointF lineCentre = line->mapToScene(line->boundingRect().center());
    int diffY = lineCentre.y() - noteCentre.y();
    m_note->moveBy(0, diffY);
}

void StaffScene::selectLine(int lineNumber)
{
    //TODO check if I need m_selectedLine
    setNoteY(lineNumber);    
    m_selectedLine = lineNumber;
    emit lineSelected(m_selectedLine);
}

void StaffScene::setCorrectState(int lineNumber, bool correctState)
{
    if(correctState)
        m_lineManager->setCorrectState(lineNumber, true);
    else
        m_lineManager->setCorrectState(lineNumber, false);
    QTimer::singleShot(500, this, SLOT(unselectLine()));
}

StaffLine *StaffScene::getLine(int lineNumber)
{
    return m_lineManager->getLine(lineNumber);
}

void StaffScene::unselectLine()
{
    m_lineManager->unsetCorrectState(m_selectedLine);
}
