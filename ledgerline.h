#ifndef LEDGERLINE_H
#define LEDGERLINE_H

#include "staffline.h"
#include <QGraphicsItem>
#include <QGraphicsRectItem>


class LedgerLine : public StaffLine
{
public:
    LedgerLine(QRectF line, QGraphicsItem* parent=0);
    void setCentreX(int centreX);
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
private:
    qreal m_circleCentreX;

};

#endif // LEDGERLINE_H
