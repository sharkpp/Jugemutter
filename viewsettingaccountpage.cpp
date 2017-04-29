#include "viewsettingaccountpage.h"
#include "ui_viewsettingaccountpage.h"

ViewSettingAccountPage::ViewSettingAccountPage(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::ViewSettingAccountPage)
{
    ui->setupUi(this);
}

ViewSettingAccountPage::~ViewSettingAccountPage()
{
    delete ui;
}
