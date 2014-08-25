#ifndef NOTE_H
#define NOTE_H

#include <QGraphicsEllipseItem>
#include <QBrush>

class Note : public QGraphicsItem
{
public:
    Note();

    virtual bool collidesWithItem(const QGraphicsItem *other, Qt::ItemSelectionMode mode) const;
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    virtual QRectF boundingRect() const;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){}

private:
    void makeNoteHead();
    void makeStem();
    void checkBounds(QGraphicsSceneMouseEvent* event);

#ifdef QT_DEBUG
    mutable QRectF m_collide;
#endif
    QGraphicsEllipseItem* m_noteHead;
    QGraphicsLineItem* m_stem;
};

#endif // NOTE_H
