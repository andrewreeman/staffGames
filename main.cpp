//#include "mainwindow.h"
#include "title.h"
#include <QApplication>

int main(int argc, char *argv[])
{

    qSetMessagePattern("(%{file}:%{line}): %{message}");
    QApplication a(argc, argv);
    Title w;
    w.show();

    return a.exec();
}
