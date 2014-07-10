#include "stickynote.h"

#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include <QWidget>
#include <QGraphicsScene>
#include <QImage>
#include <QDebug>
#include <QGraphicsView>





/*StickyNote::StickyNote()
{
    QPoint topLeft(0, 0);
    QPoint bottomRight(noteProperties::noteDiameter, noteProperties::noteDiameter);
    m_rec = QRectF(topLeft, bottomRight);

    this->setData(objectPropertyKeys::type, "note");
}

StickyNote::StickyNote(QPoint topLeft, QPoint bottomRight)
{
    m_rec = QRectF(topLeft, bottomRight);
    this->setData(objectPropertyKeys::type, "note");
}
*/
StickyNote::StickyNote(QRectF rec) : m_isMousePressed(false)
{
    m_rec = rec;
    this->setData(objectPropertyKeys::type, objectPropertyTypes::noteType);
}
/*
StickyNote::StickyNote(int topLeftX, int topLeftY, int bottomRightX, int bottomRightY)
{
    m_rec = QRectF(topLeftX, topLeftY, bottomRightX, bottomRightY);
    this->setData(objectPropertyKeys::type, "note");
}
*/
QRectF StickyNote::boundingRect() const
{
    QPoint topLeft(0, 0);
    QPoint bottomRight(noteProperties::noteDiameter, noteProperties::noteDiameter);
    return QRectF(topLeft, bottomRight);

}

void StickyNote::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QPen pen(Qt::black);    
    QRectF thisRec = boundingRect();
    if(m_isMousePressed) pen.setColor(Qt::blue);

    QBrush brush(Qt::black);

    pen.setWidth(3);
    painter->setPen(pen);
    painter->setBrush(brush);
    painter->save();
    painter->shear(-0.1, -0.1);
    painter->drawEllipse(thisRec);
    painter->restore();

    QPen linePen(Qt::black);
    QLineF staffLine = makeStem();

    linePen.setWidth(6);
    linePen.setCapStyle(Qt::RoundCap);
    painter->setPen(linePen);

    painter->drawLine(staffLine);    

    widget->update();
}

void StickyNote::setSignalHandler(StickyNoteSignalHandler *handler)
{
    m_signalHandler = handler;
    m_signalHandler->setNote(this);
}

void StickyNote::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    m_isMousePressed = true;
    QGraphicsItem::mousePressEvent(event);

}

void StickyNote::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    m_isMousePressed = false;    
    QList<QGraphicsItem*> otherItems = this->scene()->collidingItems(this);
    QList<QGraphicsItem*> lines;
    for(int i=0; i<otherItems.size(); ++i){
        QVariant varType = otherItems.at(i)->data(objectPropertyKeys::type);
        QString type = varType.toString();
        if(type == objectPropertyTypes::lineType || type == objectPropertyTypes::whiteLineType){
            lines.push_back(otherItems.at(i));
        }
    }
    if(lines.size() > 1){
        qDebug() << "MouseReleaseEvent error: Note touching multiple lines";
        return;
    }

    if( !(lines.isEmpty()) ){
        QGraphicsItem* other = lines.at(0);
        QVariant varName = other->data(objectPropertyKeys::name);
        QString name = varName.toString();
        QVariant varType = other->data(objectPropertyKeys::type);
        QString type = varType.toString();

        if(!(type.isEmpty())){
            QRectF otherRec = other->boundingRect();            
            QRectF thisRec = this->boundingRect();
            qreal diff = otherRec.center().y() - thisRec.center().y();            

            this->setPos(this->x(), diff);            
            m_signalHandler->signal_submitLine(name);
        }
    }

    QGraphicsItem::mouseReleaseEvent(event);   
}


void StickyNote::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{    
    QGraphicsItem::mouseMoveEvent(event);
    checkBounds();
    QGraphicsView* view = scene()->views().at(0);
    int thisGlobalPosY = view->mapFromScene(this->pos()).y();
    int viewY = view->height();

    if(thisGlobalPosY > viewY-noteProperties::noteDiameter){
//        view->verticalScrollBar()->setValue( (view->verticalScrollBar()->value())+1);
        m_signalHandler->signal_scrollDown();
    }
    else if(thisGlobalPosY < 0){
  //      view->verticalScrollBar()->setValue( (view->verticalScrollBar()->value())-1);
        m_signalHandler->signal_scrollUp();
    }
    else
        m_signalHandler->signal_stopScroll();





    /*QGraphicsView* view = scene()->views().at(0);
    int outsideRange = view->geometry().height() - (this->boundingRect().height());

    qDebug() << mapToParent(this->pos());
    if(this->pos().y() > outsideRange || this->pos().y() < 0){

        view->centerOn(this);
    }*/
}

bool StickyNote::collidesWithItem(const QGraphicsItem *other, Qt::ItemSelectionMode mode) const
{
    return other->collidesWithItem(this, mode);

}

QLineF StickyNote::makeStem()
{
    QRectF thisRec = boundingRect();
    QPointF p1 = thisRec.topRight() - QPointF(noteProperties::noteDiameter*0.01 , -(noteProperties::noteDiameter*0.5));
    QPointF p2 = p1 - QPointF(0, staffLayout::whitespaceHeight * 2);
    QLineF staffLine(p1, p2);
    staffLine.translate(-5, 0);
    return staffLine;
}

void StickyNote::checkBounds()
{
    int y = this->pos().y();
    int x = this->pos().x();
    if(y > (staffLayout::lowerBounds-noteProperties::noteDiameter)){
        y = staffLayout::lowerBounds-noteProperties::noteDiameter;
        this->setPos(x, y);
    }
    else if(y < (staffLayout::upperBounds + noteProperties::noteDiameter)){
        y = staffLayout::upperBounds+noteProperties::noteDiameter;
        this->setPos(x, y);
    }

    if(x < noteProperties::noteDiameter){
        x =  noteProperties::noteDiameter;
        this->setPos(x,y);
    }
    else if(x > (staffLayout::lineLength - noteProperties::noteDiameter)){
        x =  staffLayout::lineLength - noteProperties::noteDiameter;
        this->setPos(x,y);
    }
}
