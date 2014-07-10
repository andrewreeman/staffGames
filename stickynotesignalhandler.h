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
    void signal_scrollDown();
    void signal_scrollUp();
    void signal_stopScroll();

public slots:
    void scroll();
    void finishedScroll();

signals:
    void submitLine(QString line);    
    void scrollUp();
    void scrollDown();

private:
    StickyNote* m_note;
    bool m_isScrolling;
    bool m_isScrollingUp;
    QTimer* m_scrollSpeed;

};

#endif // STICKYNOTESIGNALHANDLER_H
