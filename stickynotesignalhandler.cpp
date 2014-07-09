#include "stickynotesignalhandler.h"
#include <QDebug>

StickyNoteSignalHandler::StickyNoteSignalHandler(QObject *parent) :
    QObject(parent)
{
}

void StickyNoteSignalHandler::signal_submitLine(QString line)
{
    emit submitLine(line);
}

void StickyNoteSignalHandler::setNote(StickyNote* note)
{
    m_note = note;
}
