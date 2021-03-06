#ifndef NOTE_H
#define NOTE_H

#include <QGraphicsEllipseItem>
#include <QBrush>

class Note : public QGraphicsItem
{
public:
    Note();

    virtual bool collidesWithItem(const QGraphicsItem *other, Qt::ItemSelectionMode mode) const;    
    virtual QVariant itemChange(GraphicsItemChange change, const QVariant &value);
    virtual QRectF boundingRect() const;
#ifndef QT_DEBUG
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
        Q_UNUSED(painter)Q_UNUSED(option)Q_UNUSED(widget)
    }
#else
       virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
#endif

private:
    void makeNoteHead();
    void makeStem();    

#ifdef QT_DEBUG
    mutable QRectF m_collide;
#endif
    QGraphicsEllipseItem* m_noteHead;
    QGraphicsLineItem* m_stem;
};

#endif // NOTE_H
