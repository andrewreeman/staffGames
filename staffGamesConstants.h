#ifndef STAFFGAMESCONSTANTS_H
#define STAFFGAMESCONSTANTS_H

#include <QString>
#include <QColor>

namespace titleStackedWidgetIndices{
    const int title = 0;
    const int users = 1;
    const int userHome = 2;
    const int shop = 3;
}

namespace gameStackedWidgetIndices{
    const int game = 0;
    const int won = 1;
}

namespace gamePrices{
    const int noteFinderAll = 100;
    const int noteFinderLines = 20;
    const int noteFinderSpaces = 0;
}

namespace staffLayout{
    const int blackLineHeight = 10;
    const int whiteLineHeight = blackLineHeight*7;
}

namespace noteProperties{
    const int noteDiameter = staffLayout::whiteLineHeight*1.1;
    const int stemHeight = staffLayout::whiteLineHeight * 3;
    const int noteThickness = staffLayout::blackLineHeight*0.8;
}

namespace  staffLayout{
    const int lineLength = 800;
    const int ledgerLinelength = noteProperties::noteDiameter*4;
    const int numStaffLines = 5;
    const int onePixel = 1;
    const int numBlackLedger = 3;
    const int numLedgerNotes = numBlackLedger * 2; // number of black + white lines
    const int upperBounds = -( (whiteLineHeight*numBlackLedger) + (blackLineHeight*numBlackLedger) );
    const int lowerBounds = -upperBounds + ((whiteLineHeight*numStaffLines) + (blackLineHeight*numStaffLines));
}

namespace colours{
    const QColor highlighted("lightseagreen");
    const QColor incorrect(Qt::red);
    const QColor correct(Qt::green);
    const QColor crotchet(Qt::black);
    const QColor note(Qt::white);
    const QColor stem(Qt::black);
}

namespace trebleClef{
    const int offsetX = -100;
    const int offsetY = 50;
}

namespace propertyKeys{
    const int type = 0;
    const int name = type+1;
    const int ledgerType = name+1;
    const QString gameID = "gameID";
    const QString userName = "userName";
    const QString dynamicButton = "dynamicButton";
    const QString titleLabel = "titleLabel";
}

namespace ledgerType{
    const QString ledger = "ledger";
    const QString stave = "stave";
}

namespace propertyTypes{
    const QString whiteLineType = "whiteline";
    const QString lineType = "line";
    const QString noteType = "note";
    const QString trebleType = "treble";     
}

namespace gameIDs{
    const int noteFinderAll = 0;
    const int noteFinderLines = noteFinderAll + 1;
    const int noteFinderSpaces = noteFinderLines + 1;
    const int numGames = noteFinderSpaces + 1;
}

namespace userSettingsKeys{
    const QString users = "users";
    const QString totalBeats = "totalBeats";
    const QString ownedGames = "ownedGames";
}
#endif // STAFFGAMESCONSTANTS_H
