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
    this->setData(propertyKeys::type, propertyTypes::noteType);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges);
#ifdef QT_DEBUG
    m_collide = QRectF(0, 0, 0, 0);
#endif
}

QVariant Note::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if (change == ItemPositionHasChanged && scene()) {
        QPointF newPos = value.toPointF();
        QRectF rect = scene()->sceneRect();
        rect.setHeight( rect.height() - noteProperties::noteDiameter);
        if (!rect.contains(newPos)) {
            // Keep the item inside the scene rect.
            newPos.setX(qMin(rect.right(), qMax(newPos.x(), rect.left())));
            newPos.setY(qMin(rect.bottom(), qMax(newPos.y(), rect.top())));
            setPos(newPos);
        }        

    }
    else if(change == ItemPositionChange && scene())
        scene()->views().at(0)->ensureVisible(this, 10, 100);
//TODO WEIRD BEHAVIOUR WHEN MOVE NOTES
    return QGraphicsItem::itemChange(change, value);
}

QRectF Note::boundingRect() const
{
    return m_noteHead->boundingRect();
}

void Note::makeNoteHead()
{
    QBrush brush(colours::crotchet);
    QPen pen(colours::crotchet);
    QPoint p1(0, 0);
    QPoint p2(noteProperties::noteDiameter*1.1, 0);
    QRectF rec(p1, p2);

    rec.setHeight(noteProperties::noteDiameter*0.7);
    m_noteHead = new QGraphicsEllipseItem(rec, this);    
    m_noteHead->setBrush(brush);
    pen.setWidth(noteProperties::noteThickness);
    m_noteHead->setPen(pen);
    m_noteHead->setTransformOriginPoint(this->boundingRect().center());
    m_noteHead->setRotation(-30);
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
    m_stem->moveBy(-noteProperties::noteThickness*0.9, -noteProperties::noteThickness*0.5);

}

bool Note::collidesWithItem(const QGraphicsItem *other, Qt::ItemSelectionMode mode) const
{
    QVariant type = other->data(propertyKeys::type);
    if(type.toString() == propertyTypes::lineType){
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
