#include "graphicsview.h"
#include <QEvent>
#include <QTouchEvent>
#include <QDebug>

GraphicsView::GraphicsView(QWidget *parent) :
    QGraphicsView(parent), totalScaleFactor(1)
{
    viewport()->setAttribute(Qt::WA_AcceptTouchEvents);
        setDragMode(ScrollHandDrag);
}

bool GraphicsView::viewportEvent(QEvent *event)
{
    switch (event->type()) {
         case QEvent::TouchBegin:
         case QEvent::TouchUpdate:
         case QEvent::TouchEnd:
         {
             QTouchEvent *touchEvent = static_cast<QTouchEvent *>(event);
             QList<QTouchEvent::TouchPoint> touchPoints = touchEvent->touchPoints();
             if (touchPoints.count() == 2) {
                 // determine scale factor
                 const QTouchEvent::TouchPoint &touchPoint0 = touchPoints.first();
                 const QTouchEvent::TouchPoint &touchPoint1 = touchPoints.last();
                 qreal currentScaleFactor =
                         QLineF(touchPoint0.pos(), touchPoint1.pos()).length()
                         / QLineF(touchPoint0.startPos(), touchPoint1.startPos()).length();

                 setTransform(QTransform().scale(totalScaleFactor * currentScaleFactor,
                                                 totalScaleFactor * currentScaleFactor));

                 if (touchEvent->touchPointStates() & Qt::TouchPointReleased) {
                     // if one of the fingers is released, remember the current scale
                     // factor so that adding another finger later will continue zooming
                     // by adding new scale factor to the existing remembered value.
                     totalScaleFactor *= currentScaleFactor;
                     //currentScaleFactor = 1;
                 }
                 qDebug() << totalScaleFactor;

             }
             return false; // don't consume as touch is needed for mouse

         }
         default:
             break;
         }
         return QGraphicsView::viewportEvent(event);

}
