#include "staffscene.h"
#include "staffGamesConstants.h"

#include <QDebug>
#include <QTimer>
#include <QGraphicsView>

StaffScene::StaffScene(QObject *parent) :
    QGraphicsScene(parent)
{
    makeLine();
    makeTrebleClef();
    makeNote();
    m_note->installSceneEventFilter(m_lineManager);
    setBoundries();
}

void StaffScene::makeLine(){
    m_lineManager = new lineManager();
    this->addItem(m_lineManager);
}

void StaffScene::makeNote()
{
    m_note = new Note();
    m_note->setFlag(QGraphicsItem::ItemIsMovable, true);
    m_note->moveBy(0, -25);        
    this->addItem(m_note);
}

void StaffScene::makeTrebleClef()
{
    QPixmap image(":/notation/treble");
    m_treble = addPixmap(image);
    m_treble->setPos(trebleClef::offset, getLine(6)->pos().y() - 130);
    m_treble->setScale(1);
    m_treble->setData(objectPropertyKeys::type, objectPropertyTypes::trebleType);
}

void StaffScene::setBoundries()
{
    QRectF rec = sceneRect();
    rec.setTop(rec.top() - noteProperties::noteDiameter);
    rec.setBottom(rec.bottom() + noteProperties::noteDiameter);
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
