#include "buttonrelay.h"

ButtonRelay::ButtonRelay(QPushButton* button, QVariant message, QObject *parent) :
    QObject(parent), m_button(button), m_message(message)
{
    connect(button, SIGNAL(clicked()), this, SLOT(clicked()));
}

void ButtonRelay::clicked()
{
    emit buttonClicked(m_message);
}
