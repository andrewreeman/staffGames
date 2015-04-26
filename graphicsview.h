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
    //TODO when zoom in first touch centers on note meaning note doesnt move when first selected
};

#endif // GRAPHICSVIEW_H
