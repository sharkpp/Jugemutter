#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // configuration for QSettings
    QCoreApplication::setOrganizationName("sharkpp");
    QCoreApplication::setOrganizationDomain("sharkpp.net");
    QCoreApplication::setApplicationName("Jugemutter");

    MainWindow w;
    w.show();

    return a.exec();
}
