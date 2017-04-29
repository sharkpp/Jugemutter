#include "viewsettinggeneralpage.h"
#include "ui_viewsettinggeneralpage.h"

ViewSettingGeneralPage::ViewSettingGeneralPage(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::ViewSettingGeneralPage)
{
    ui->setupUi(this);
}

ViewSettingGeneralPage::~ViewSettingGeneralPage()
{
    delete ui;
}
