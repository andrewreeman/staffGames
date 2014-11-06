#include "staffscene.h"
#include "staffGamesConstants.h"

#include <QDebug>
#include <QTimer>
#include <QGraphicsView>

StaffScene::StaffScene(QGraphicsView* view, QObject *parent) :
    QGraphicsScene(parent), m_view(view)
{
    m_view->setScene(this);
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
    QPointF viewCentre =m_view->rect().center();

    m_note = new Note();
    m_note->setFlag(QGraphicsItem::ItemIsMovable, true);    
    m_note->moveBy(viewCentre.x(), viewCentre.y());
    m_view->centerOn(m_note);

    this->addItem(m_note);
}

void StaffScene::makeTrebleClef()
{
    QPixmap image(":/notation/treble");
    m_treble = addPixmap(image);
    m_treble->setPos(trebleClef::offsetX, line(6)->pos().y() - trebleClef::offsetY);
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
    StaffLine* line = m_lineManager->line(selectedLineNumber);
    QPointF lineCentre = line->mapToScene(line->boundingRect().center());
    int diffY = lineCentre.y() - noteCentre.y();
    m_note->moveBy(0, diffY);
}

void StaffScene::selectLine(int lineNumber)
{    
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

StaffLine *StaffScene::line(int lineNumber)
{
    return m_lineManager->line(lineNumber);
}

void StaffScene::unselectLine()
{
    m_lineManager->unsetCorrectState(m_selectedLine);
}
