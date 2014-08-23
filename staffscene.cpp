#include "staffscene.h"
#include "staffGamesConstants.h"
#include <QDebug>
#include <QTimer>

StaffScene::StaffScene(QObject *parent) :
    QGraphicsScene(parent)
{
    makeLine();
    makeCircle();
    m_note->installSceneEventFilter(m_lineManager);
    setTargetLine();
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

void StaffScene::setTargetLine()
{
    m_targetLine = 2;
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
    setNoteY(lineNumber);
    if(lineNumber == m_targetLine)
        m_lineManager->setCorrectState(lineNumber, true);
    else
        m_lineManager->setCorrectState(lineNumber, false);
    m_selectedLine = lineNumber;
    QTimer::singleShot(500, this, SLOT(unselectLine()));    
    emit lineSelected(m_selectedLine);
}

void StaffScene::unselectLine()
{
    m_lineManager->unsetCorrectState(m_selectedLine);
}
