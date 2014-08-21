#include "staffline.h"
#include "projectConstants.hh"

StaffLine::StaffLine(QRectF rect, QGraphicsItem *parent) :
    QGraphicsRectItem(rect, parent), m_defaultBrush(Qt::black), m_selectedBrush(projectConstants::selectedColour),
    m_correctBrush(projectConstants::correctColour), m_incorrectBrush(projectConstants::incorrectColour)
{
    setBrush(m_defaultBrush);
}

void StaffLine::setDefaultBrush(QBrush brush)
{
    m_defaultBrush = brush;
    setBrush(m_defaultBrush);
}

void StaffLine::setSelected(bool selected)
{
    if(selected)
        setBrush(m_selectedBrush);            
    else
        setBrush(m_defaultBrush);
    QGraphicsRectItem::setSelected(selected);
}

void StaffLine::setCorrectState(bool isCorrect)
{
    if(isCorrect)
        setBrush(m_correctBrush);
    else
        setBrush(m_incorrectBrush);
}

void StaffLine::unsetCorrectState()
{
    setBrush(m_defaultBrush);
}

