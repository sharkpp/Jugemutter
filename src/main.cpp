#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // QSettings 用の設定
    QCoreApplication::setOrganizationName("sharkpp");
    QCoreApplication::setOrganizationDomain("sharkpp.net");
    QCoreApplication::setApplicationName("Jugemutter");

    // リソースの初期化
    Q_INIT_RESOURCE(resources);

    MainWindow w;
    w.show();

    return a.exec();
}
