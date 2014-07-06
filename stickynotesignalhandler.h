#ifndef STICKYNOTESIGNALHANDLER_H
#define STICKYNOTESIGNALHANDLER_H

#include <QObject>

class StickyNoteSignalHandler;
#include "stickynote.h"

class StickyNoteSignalHandler : public QObject
{
    Q_OBJECT
public:
    explicit StickyNoteSignalHandler(QObject *parent = 0);
    void signal_submitLine(QString line);
    void setNote(StickyNote*);

signals:
    void submitLine(QString line);    
public slots:
    void userResult(bool);
private:
    StickyNote* m_note;
};

#endif // STICKYNOTESIGNALHANDLER_H
