#ifndef STICKYNOTE_H
#define STICKYNOTE_H

#include <QGraphicsItem>
#include <staffGamesConstants.h>
#include "stickyline.h"

class StickyNote;
#include "stickynotesignalhandler.h"

class StickyNote : public QGraphicsItem
{

public:
    //StickyNote();
    //StickyNote(QPoint x, QPoint y);
    StickyNote(QRectF);
    //StickyNote(int, int, int, int);

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void setSignalHandler(StickyNoteSignalHandler* handler);
    void userResult(bool);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    bool collidesWithItem(const QGraphicsItem *other, Qt::ItemSelectionMode mode) const;
private:
    QLineF makeStem();
    void checkBounds();
    QRectF m_rec;
    bool m_isMousePressed;


    StickyNoteSignalHandler* m_signalHandler;

};

#endif // STICKYNOTE_H
