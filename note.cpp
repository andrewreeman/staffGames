#include "note.h"
#include "projectConstants.hh"
#include "staffGamesConstants.h"
#include <QPainter>
#include <QWidget>

Note::Note(QRectF rec, QGraphicsItem* parent) :
    QGraphicsEllipseItem(rec, parent)
{
    this->setData(objectPropertyKeys::type, objectPropertyTypes::noteType);
#ifdef QT_DEBUG
    m_collide = QRectF(0, 0, 0, 0);
#endif
}

bool Note::collidesWithItem(const QGraphicsItem *other, Qt::ItemSelectionMode mode) const
{
    QVariant type = other->data(objectPropertyKeys::type);
    if(type.toString() == objectPropertyTypes::lineType){
        //int lineNumber = other->data(projectConstants::keyLineNumber).toInt();
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
    return QGraphicsEllipseItem::collidesWithItem(other, mode);
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

    QGraphicsEllipseItem::paint(painter, option, widget);
    widget->update();
}
#endif
