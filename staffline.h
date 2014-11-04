#ifndef STAFFLINE_H
#define STAFFLINE_H

#include <QGraphicsRectItem>
#include <QBrush>

class StaffLine : public QGraphicsRectItem
{
public:
    StaffLine(QRectF rect, QGraphicsItem* parent=0);
    void setDefaultBrush(QBrush);
    virtual void setSelected(bool selected);
    //QBrush selectedBrush(){ return m_selectedBrush; }
    void setCorrectState(bool isCorrect);
    void unsetCorrectState();


    //virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
private:
    QBrush m_defaultBrush;
    QBrush m_selectedBrush;
//    QBrush m_paintingBrush;
    QBrush m_correctBrush;
    QBrush m_incorrectBrush;

};

#endif // STAFFLINE_H
