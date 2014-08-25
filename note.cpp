#include "note.h"
#include "staffGamesConstants.h"

#include <QPainter>
#include <QWidget>
#include <QDebug>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QLine>
#include <QGraphicsSceneMouseEvent>


Note::Note()
{
    makeNoteHead();
    makeStem();
    this->setData(objectPropertyKeys::type, objectPropertyTypes::noteType);

#ifdef QT_DEBUG
    m_collide = QRectF(0, 0, 0, 0);
#endif
}

void Note::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{   
    QGraphicsItem::mouseMoveEvent(event);
    scene()->views().at(0)->ensureVisible(this, 10, 50);
    checkBounds(event);
}

void Note::checkBounds(QGraphicsSceneMouseEvent *event)
{
    QRectF sceneRect = scene()->sceneRect();
    QRectF thisRect = mapToScene(boundingRect()).boundingRect();

    int topBoundry = sceneRect.top();
    bool topHit = false;
    int bottomBoundry = sceneRect.bottom();
    bool bottomHit = false;
    int leftBoundry = sceneRect.left();
    bool leftHit = false;
    int rightBoundry = sceneRect.right();
    bool rightHit = false;

    //init check
    if(thisRect.top()<topBoundry)
        topHit = true;
    if(thisRect.bottom() > bottomBoundry)
        bottomHit = true;
    if(thisRect.left() < leftBoundry)
        leftHit = true;
    if(thisRect.right() > rightBoundry)
        rightHit = true;
    if( !(topHit || bottomHit || leftHit || rightHit) )
        return;

    QPointF mousePos = event->scenePos();
    QPointF mouseDelta = mousePos - event->lastScenePos();
    QPointF newPos(pos().x(), pos().y());

    if(topHit){
        if(mouseDelta.y() <=0 || mousePos.y()<topBoundry+noteProperties::noteDiameter)
            newPos.setY(topBoundry);
    }
    if(bottomHit){
        if(mouseDelta.y() >=0 || mousePos.y()>bottomBoundry)
            newPos.setY(bottomBoundry-noteProperties::noteDiameter);
    }
    if(leftHit){
        if(mouseDelta.x() <=0 || mousePos.x()<leftBoundry+noteProperties::noteDiameter)
            newPos.setX(leftBoundry);
    }
    if(rightHit){
        if(mouseDelta.x() >= 0 || mousePos.x()>rightBoundry)
            newPos.setX(rightBoundry-noteProperties::noteDiameter);
    }
    setPos(newPos);
}

QRectF Note::boundingRect() const
{
    return m_noteHead->boundingRect();
}

void Note::makeNoteHead()
{
    QBrush brush(colours::note);
    QPen pen(colours::stem);
    QPoint p1(0, 0);
    QPoint p2(noteProperties::noteDiameter, 0);
    QRectF rec(p1, p2);

    rec.setHeight(noteProperties::noteDiameter);
    m_noteHead = new QGraphicsEllipseItem(rec, this);
    m_noteHead->setBrush(brush);
    pen.setWidth(noteProperties::noteThickness);
    m_noteHead->setPen(pen);
}

void Note::makeStem()
{
    QPen pen(colours::stem);
    int noteRight = m_noteHead->boundingRect().right();
    int noteCentreY = m_noteHead->boundingRect().center().y();
    QPoint p1(noteRight, m_noteHead->boundingRect().top() - noteProperties::stemHeight);
    QPoint p2(noteRight, noteCentreY);
    QLine stem(p1, p2);

    m_stem = new QGraphicsLineItem(stem, this);
    pen.setWidth(noteProperties::noteThickness);
    pen.setCapStyle(Qt::RoundCap);
    m_stem->setPen(pen);
    m_stem->moveBy(-noteProperties::noteThickness/2, 0);

}

bool Note::collidesWithItem(const QGraphicsItem *other, Qt::ItemSelectionMode mode) const
{
    QVariant type = other->data(objectPropertyKeys::type);
    if(type.toString() == objectPropertyTypes::lineType){
        QRectF otherRec = other->boundingRect();
        QPointF otherCentre = mapFromScene(otherRec.center());
        QPointF thisCentre = this->boundingRect().center();

        QRectF collisionArea = otherRec;
        collisionArea.setHeight(staffLayout::whiteLineHeight/2);
        collisionArea.moveCenter( otherCentre );
        if(collisionArea.contains(thisCentre)){
#ifdef QT_DEBUG
            m_collide = collisionArea;
#endif
            return true;
        }
        else{
            return false;
        }
    }
    return QGraphicsItem::collidesWithItem(other, mode);
}

#ifdef QT_DEBUG

void Note::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{

    QBrush brush(Qt::blue);
    QPen pen(Qt::black);

    pen.setWidth(2);
    painter->save();
    painter->setBrush(brush);
    painter->setPen(pen);
    painter->drawRect(m_collide);
    painter->restore();

    widget->update();
}


#endif
