#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
#ifdef QT_DEBUG
    qSetMessagePattern("(%{file}:%{line}) - %{message}");
#endif
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
