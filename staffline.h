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
    void setCorrectState(bool isCorrect);
    void unsetCorrectState();

private:
    QBrush m_defaultBrush;
    QBrush m_selectedBrush;
    QBrush m_correctBrush;
    QBrush m_incorrectBrush;
};

#endif // STAFFLINE_H
