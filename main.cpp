#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{

    qSetMessagePattern("(%{file}:%{line}): %{message}");
    QCoreApplication::setOrganizationName("StepWise");
    QCoreApplication::setOrganizationDomain("stepwise.com");
    QCoreApplication::setApplicationName("Staff Games");
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
