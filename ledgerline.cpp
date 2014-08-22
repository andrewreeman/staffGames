#include "ledgerline.h"
#include "projectConstants.hh"
#include "staffGamesConstants.h"

#include <QDebug>
#include <QWidget>
#include <QStyleOptionGraphicsItem>
#include <QPainter>
#include <QPen>


LedgerLine::LedgerLine(QRectF line, QGraphicsItem *parent) :
    StaffLine(line, parent)
{
    m_circleCentreX = this->boundingRect().center().x();
}

void LedgerLine::setCentreX(int centreX)
{
    //int thisCentreX = boundingRect().center().x();
    //this->moveBy(centreX-thisCentreX, 0);
    m_circleCentreX = centreX;
}

void LedgerLine::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);

    QRectF rec = boundingRect();
    QPen pen;
    pen.setStyle(Qt::NoPen);

    rec.setWidth(staffLayout::ledgerLinelength);
    rec.moveCenter( QPointF(m_circleCentreX, rec.center().y() ) );
    painter->setPen(pen);
    painter->setBrush( brush() );
    painter->drawRect(rec);

    widget->update();
}
