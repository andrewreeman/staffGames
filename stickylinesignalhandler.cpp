#include "stickylinesignalhandler.h"
#include <QDebug>

StickyLineSignalHandler::StickyLineSignalHandler(QObject *parent) :
    QObject(parent)
{
}

void StickyLineSignalHandler::setLine(StickyLine* line)
{
    m_line = line;
}

void StickyLineSignalHandler::userCorrect(bool isCorrect)
{
    qDebug() << isCorrect;
}
