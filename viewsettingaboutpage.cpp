#include "viewsettingaboutpage.h"
#include "ui_viewsettingaboutpage.h"

ViewSettingAboutPage::ViewSettingAboutPage(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::ViewSettingAboutPage)
{
    ui->setupUi(this);
    ui->appIcon->setPixmap(QIcon(":/icons.white/add.svg").pixmap(QSize(48, 48)));
}

ViewSettingAboutPage::~ViewSettingAboutPage()
{
    delete ui;
}
