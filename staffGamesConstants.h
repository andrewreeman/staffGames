#ifndef STAFFGAMESCONSTANTS_H
#define STAFFGAMESCONSTANTS_H

#include <QString>
#include <QColor>

namespace staffLayout{
    const int lineHeight = 10;
    const int lineLength = 800;
    const int whitespaceHeight = 100;
    const int numStaffLines = 5;
    const int onePixel = 1;
}

namespace colours{
    const QColor highlighted("lightseagreen");
    const QColor incorrect(Qt::red);
    const QColor correct(Qt::green);
}

namespace noteProperties{
    const int noteDiameter = 70;
}

namespace objectPropertyKeys{
    const int type = 0;
    const int name = 1;
}

namespace objectPropertyTypes{
    const QString whiteLineType = "whiteline";
    const QString lineType = "line";
    const QString noteType = "note";
    const QString trebleType = "treble";
}

#endif // STAFFGAMESCONSTANTS_H
