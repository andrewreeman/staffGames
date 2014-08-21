#ifndef PROJECTCONSTANTS_HH
#define PROJECTCONSTANTS_HH

#include <QString>
#include <QColor>


namespace projectConstants{
    const int blackWidth = 10;
    const int whiteWidth = blackWidth*2;
    const int staffLineLength = 150;
    const int ledgerLineLength = 10;
    const int keyType = 0;
    const int keyLineNumber = 1;    
    const QString typeStaffLine = "staffLine";
    const QString typeNote = "note";
    const QColor noteColour = Qt::white;
    const QColor correctColour = Qt::green;
    const QColor incorrectColour = Qt::red;
    const QColor selectedColour = Qt::gray;
}



#endif // PROJECTCONSTANTS_HH
