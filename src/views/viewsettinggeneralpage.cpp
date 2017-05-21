#include "viewsettinggeneralpage.h"
#include "ui_viewsettinggeneralpage.h"

ViewSettingGeneralPage::ViewSettingGeneralPage(QWidget *parent) :
    PageSelectorView(parent),
    ui(new Ui::ViewSettingGeneralPage)
{
    ui->setupUi(this);
}

ViewSettingGeneralPage::~ViewSettingGeneralPage()
{
    delete ui;
}
