#include "mainwindow.h"
#include <QApplication>
#include <QTranslator>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // リソースの初期化
    Q_INIT_RESOURCE(resources);

    // 翻訳
    QString locale = QLocale::system().name();
    QTranslator translator;
    translator.load(locale, ":/i18n");
    a.installTranslator(&translator);

    // QSettings 用の設定
    QCoreApplication::setOrganizationName("sharkpp");
    QCoreApplication::setOrganizationDomain("sharkpp.net");
    QCoreApplication::setApplicationName("Jugemutter");

    MainWindow w;
    w.show();

    return a.exec();
}
