#include "viewsettingaboutpage.h"
#include "ui_viewsettingaboutpage.h"
#include <QFileInfo>
#include <QDir>
#include <QDebug>

ViewSettingAboutPage::ViewSettingAboutPage(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::ViewSettingAboutPage)
{
    ui->setupUi(this);
#ifdef Q_OS_MAC
    QFileInfo iconPath = QFileInfo(QDir(qApp->applicationDirPath()), "../Resources/icon-macOS.icns");
    ui->appIcon->setPixmap(QIcon(iconPath.absoluteFilePath()).pixmap(ui->appIcon->geometry().size()));
#endif
}

ViewSettingAboutPage::~ViewSettingAboutPage()
{
    delete ui;
}
