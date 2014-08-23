#ifndef STAFFGAMESCONSTANTS_H
#define STAFFGAMESCONSTANTS_H

#include <QString>
#include <QColor>

namespace staffLayout{
    const int blackLineHeight = 10;
    const int whiteLineHeight = blackLineHeight*7;
}

namespace noteProperties{
    const int noteDiameter = staffLayout::whiteLineHeight;
}

namespace  staffLayout{
    const int lineLength = 800;
    const int ledgerLinelength = noteProperties::noteDiameter*4;
    const int numStaffLines = 5;
    const int onePixel = 1;
    const int numLedgerLines = 3;
    const int upperBounds = -( (whiteLineHeight*numLedgerLines) + (blackLineHeight*numLedgerLines) );
    const int lowerBounds = -upperBounds + ((whiteLineHeight*numStaffLines) + (blackLineHeight*numStaffLines));

}

namespace colours{
    const QColor highlighted("lightseagreen");
    const QColor incorrect(Qt::red);
    const QColor correct(Qt::green);
    const QColor note(Qt::white);
}

namespace trebleClef{
    const int offset = -100;
}

namespace objectPropertyKeys{
    const int type = 0;
    const int name = 1;
    const int ledgerType = 2;
}

namespace ledgerType{
    const QString ledger = "ledger";
    const QString stave = "stave";
}

namespace objectPropertyTypes{
    const QString whiteLineType = "whiteline";
    const QString lineType = "line";
    const QString noteType = "note";
    const QString trebleType = "treble"; 
}

#endif // STAFFGAMESCONSTANTS_H
