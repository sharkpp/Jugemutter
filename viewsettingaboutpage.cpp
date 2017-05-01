#include "viewsettingaboutpage.h"
#include "ui_viewsettingaboutpage.h"
#include <QFileInfo>
#include <QDir>
#include <QDebug>

#if defined(Q_OS_WIN)
#include <QtWin>
#endif

ViewSettingAboutPage::ViewSettingAboutPage(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::ViewSettingAboutPage)
{
    ui->setupUi(this);

    QSize iconSize = ui->appIcon->geometry().size();
#if defined(Q_OS_MAC)
    QFileInfo iconPath = QFileInfo(QDir(qApp->applicationDirPath()), "../Resources/icon-macOS.icns");
    ui->appIcon->setPixmap(QIcon(iconPath.absoluteFilePath()).pixmap(iconSize));
#elif defined(Q_OS_WIN)
    HICON hIcon;
    UINT uIconId;
    //::ExtractIconEx(qApp->applicationFilePath().toStdWString().c_str(), 0, &hIcon, NULL, 1);
    ::PrivateExtractIcons(qApp->applicationFilePath().toStdWString().c_str(),
                          0, 256, 256, &hIcon, &uIconId,
                          1, LR_LOADFROMFILE | LR_VGACOLOR);
    QPixmap icon = QtWin::fromHICON(hIcon);
    ::DestroyIcon(hIcon);
    ui->appIcon->setPixmap(icon.scaled(iconSize.width(), iconSize.height(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
#endif
}

ViewSettingAboutPage::~ViewSettingAboutPage()
{
    delete ui;
}
