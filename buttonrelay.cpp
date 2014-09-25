#include "buttonrelay.h"

ButtonRelay::ButtonRelay(QPushButton* button, int userIndex, QObject *parent) :
    m_button(button), m_userIndex(userIndex), QObject(parent)
{
    connect(button, SIGNAL(clicked()), this, SLOT(clicked()));
}


void ButtonRelay::clicked()
{
    emit buttonClicked(m_userIndex);
}
