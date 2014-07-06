#ifndef STICKYLINESIGNALHANDLER_H
#define STICKYLINESIGNALHANDLER_H

class StickyLineSignalHandler;

#include <QObject>
#include "stickyline.h"

class StickyLineSignalHandler : public QObject
{
    Q_OBJECT
public:
    explicit StickyLineSignalHandler(QObject *parent = 0);
    void setLine(StickyLine*);
signals:

public slots:
    void userCorrect(bool);
private:
    StickyLine* m_line;

};

#endif // STICKYLINESIGNALHANDLER_H
