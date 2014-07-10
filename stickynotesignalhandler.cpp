#include "stickynotesignalhandler.h"
#include <QDebug>

StickyNoteSignalHandler::StickyNoteSignalHandler(QObject *parent) :
    QObject(parent), m_isScrolling(false)
{
    m_scrollSpeed = new QTimer(this);
    m_scrollSpeed->setInterval(40);

//    m_scrollSpeed->setSingleShot(true);
    connect(m_scrollSpeed, SIGNAL(timeout()), this, SLOT(scroll()));

}

void StickyNoteSignalHandler::signal_submitLine(QString line)
{
    emit submitLine(line);
}

void StickyNoteSignalHandler::setNote(StickyNote* note)
{
    m_note = note;
}

void StickyNoteSignalHandler::signal_scrollDown()
{

    if(m_isScrolling) return;
    m_isScrollingUp = 0;
    m_scrollSpeed->start();
}

void StickyNoteSignalHandler::signal_scrollUp()
{
    if(m_isScrolling) return;
    m_isScrollingUp = 1;
    m_scrollSpeed->start();
}

void StickyNoteSignalHandler::signal_stopScroll()
{
    m_scrollSpeed->stop();
}

void StickyNoteSignalHandler::scroll()
{

    m_isScrolling = true;
    if(m_isScrollingUp)
        emit scrollUp();
    else
        emit scrollDown();
}

void StickyNoteSignalHandler::finishedScroll()
{
    m_isScrolling = false;
}
