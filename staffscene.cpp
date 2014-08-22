#include "staffscene.h"
#include "projectConstants.hh"
#include "staffGamesConstants.h"
#include <QDebug>
#include <QTimer>

StaffScene::StaffScene(QObject *parent) :
    QGraphicsScene(parent)
{
    makeLine();
    makeCircle();
    m_circle->installSceneEventFilter(m_line);
    setTargetLine();
}

void StaffScene::makeLine(){
    m_line = new lineManager();
    this->addItem(m_line);
}

void StaffScene::makeCircle()
{    
    QBrush brush(colours::note);
    QPoint p1(0, 0);    
    QPoint p2(noteProperties::noteDiameter, 0);
    QRectF rec(p1, p2);
    rec.setHeight(noteProperties::noteDiameter);
    m_circle = new Note(rec);    
    m_circle->setFlag(QGraphicsItem::ItemIsMovable, true);
    m_circle->moveBy(0, -25);
    m_circle->setBrush(brush);
    this->addItem(m_circle);
}

void StaffScene::setTargetLine()
{
    m_targetLine = 2;
}

void StaffScene::selectLine(int lineNumber)
{
    if(lineNumber == m_targetLine)
        m_line->setCorrectState(lineNumber, true);
    else
        m_line->setCorrectState(lineNumber, false);
    m_selectedLine = lineNumber;
    QTimer::singleShot(500, this, SLOT(unselectLine()));
    emit lineSelected(m_selectedLine);
}

void StaffScene::unselectLine()
{
    m_line->unsetCorrectState(m_selectedLine);
}
