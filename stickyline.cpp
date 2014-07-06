#include "stickyline.h"
#include <QPainter>
#include <QGraphicsScene>
#include <QWidget>
#include <QTimer>



#include <QDebug>

StickyLine::StickyLine()
{
    QPoint topLeft(10, 10);
    QPoint bottomRight(staffLayout::lineLength, staffLayout::lineHeight);
    m_rec = QRectF(topLeft, bottomRight);
    m_defaultColour = QColor(Qt::black);
    m_collisionMode = Qt::IntersectsItemShape;
}

StickyLine::StickyLine(QPoint topLeft, QPoint bottomRight)
{
    m_rec = QRectF(topLeft, bottomRight);
}

StickyLine::StickyLine(QRectF rec)
{
    m_rec = rec;
}

StickyLine::StickyLine(int topLeftX, int topLeftY, int bottomRightX, int bottomRightY)
{
    m_rec = QRectF(topLeftX, topLeftY, bottomRightX, bottomRightY);
}

void StickyLine::userResult(bool userResult)
{

}

void StickyLine::setColour(QColor colour)
{
    m_defaultColour = colour;
}

void StickyLine::setCollisionMode(Qt::ItemSelectionMode mode)
{
    m_collisionMode = mode;
}

void StickyLine::addSignalHandler(StickyLineSignalHandler* handler)
{
    m_signalHandler = handler;
    m_signalHandler->setLine(this);
}

QRectF StickyLine::boundingRect() const
{
    return m_rec;
}

void StickyLine::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QPen pen(Qt::black);
    QBrush brush(Qt::black);
    //QLineF staff(m_rec.topLeft(), m_rec.topRight());
    QRectF staff = boundingRect();


    //pen.setWidth(staffLayout::lineHeight);


    if( scene()->collidingItems(this).isEmpty() ){
        pen.setColor(m_defaultColour);
        brush.setColor(m_defaultColour);
    }
    else{        
        pen.setColor(colours::highlighted);
        brush.setColor(colours::highlighted);
    }

    //painter->setPen(pen);


    painter->setBrush(brush);
    painter->setPen(pen);

    painter->fillRect(staff, brush);
    painter->drawRect(staff);
    widget->update();

    //painter->drawLine(staff);

}

void StickyLine::mousePressEvent(QGraphicsSceneMouseEvent *event)
{    
    QGraphicsItem::mousePressEvent(event);
}

void StickyLine::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mouseReleaseEvent(event);
}

bool StickyLine::collidesWithItem(const QGraphicsItem *other, Qt::ItemSelectionMode mode) const
{
    QVariant otherType = other->data(objectPropertyKeys::type);
    QString otherStr = otherType.toString();
    if(otherStr == "note"){
        QVariant type = this->data(objectPropertyKeys::type);
        QString typeStr = type.toString();

        // map other rect to this coord system
        QRectF recOther = other->boundingRect();
        recOther = this->mapRectFromItem(other, recOther);
        recOther.setLeft(0);

        QRectF recThis = this->boundingRect();

        if( typeStr.contains("white") ){
            if(recThis.contains(recOther)){
                return true;
            }
            else{
                return false;
            }
        }
        if( typeStr.contains("line") ){
            if(recThis.intersects(recOther)){
                return true;
            }
            else{
                return false;
            }
        }
     //   return QGraphicsItem::collidesWithItem(other, Qt::IntersectsItemBoundingRect);
    }
    else{
        return false;
    }
}


