#ifndef STICKYLINESIGNALHANDLER_H
#define STICKYLINESIGNALHANDLER_H

class StickyLineSignalHandler;

#include <QObject>
#include <QTimer>

#include "stickyline.h"


class StickyLineSignalHandler : public QObject
{
    Q_OBJECT
public:
    explicit StickyLineSignalHandler(QObject *parent = 0);
    void setLine(StickyLine*);
    void test();
    //del me
    QString m_lineName;
signals:

public slots:
    void userResult(bool);
    void resetLineColour();
private:
    StickyLine* m_line;
    QTimer* m_lineColourTimer;



};

#endif // STICKYLINESIGNALHANDLER_H
