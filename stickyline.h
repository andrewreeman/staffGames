#ifndef STICKYLINE_H
#define STICKYLINE_H

class StickyLine;

#include <QGraphicsItem>
#include <staffGamesConstants.h>
#include "stickylinesignalhandler.h"

class StickyLine : public QGraphicsItem
{
public:
    StickyLine();
    StickyLine(QPoint x, QPoint y);
    StickyLine(QRectF);
    StickyLine(int, int, int, int);
    void setColour(QColor);
    void setHighlight(QColor);
    void userResult(bool);
    void setCollisionMode(Qt::ItemSelectionMode mode);
    void addSignalHandler(StickyLineSignalHandler*);

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    bool collidesWithItem(const QGraphicsItem *other, Qt::ItemSelectionMode mode) const;
private:
    QRectF m_rec;
    QColor m_defaultColour;
    QColor m_highlightColour;
    Qt::ItemSelectionMode m_collisionMode;
    StickyLineSignalHandler* m_signalHandler;

};

#endif // STICKYLINE_H
