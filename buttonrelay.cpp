#include "buttonrelay.h"

ButtonRelay::ButtonRelay(QPushButton* button, QString userName, QObject *parent) :
    m_button(button), m_userName(userName), QObject(parent)
{
    connect(button, SIGNAL(clicked()), this, SLOT(clicked()));
}


void ButtonRelay::clicked()
{
    emit buttonClicked(m_userName);
}
