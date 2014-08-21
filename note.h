#ifndef NOTE_H
#define NOTE_H

#include <QGraphicsEllipseItem>


class Note : public QGraphicsEllipseItem
{
public:
    Note(QRectF, QGraphicsItem* parent=0);

    virtual bool collidesWithItem(const QGraphicsItem *other, Qt::ItemSelectionMode mode) const;
#ifdef QT_DEBUG
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    mutable QRectF m_collide;
#endif
};

#endif // NOTE_H
