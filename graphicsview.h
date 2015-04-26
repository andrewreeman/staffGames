#ifndef GRAPHICSVIEW_H
#define GRAPHICSVIEW_H

#include <QGraphicsView>

class GraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit GraphicsView(QWidget *parent = 0);
    virtual bool viewportEvent(QEvent *event);

signals:

public slots:


private:
    float totalScaleFactor;
};

#endif // GRAPHICSVIEW_H
