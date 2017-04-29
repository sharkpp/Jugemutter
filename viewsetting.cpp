#include "viewsetting.h"
#include "ui_viewsetting.h"
#include "viewsettinggeneralpage.h"
#include "viewsettingaccountpage.h"

//---------------------------------------------------------
// SettingPageDocument
//---------------------------------------------------------

SettingPageDocument::SettingPageDocument(QObject *parent)
    : PageSelectorDocument(parent)
{
}

//---------------------------------------------------------
// ViewSetting
//---------------------------------------------------------

ViewSetting::ViewSetting(QWidget *parent)
    : QFrame(parent)
    , ui(new Ui::ViewSetting)
    , settings(new SettingPageDocument(this))
{
    ui->setupUi(this);
    ui->pageSelector->setBuddy(ui->pageContainer);
    ui->pageContainer->addWidget(generalPage = new ViewSettingGeneralPage(this));
    ui->pageContainer->addWidget(accountPage = new ViewSettingAccountPage(this));
    initToolbar();
}

ViewSetting::~ViewSetting()
{
    delete ui;
}

void ViewSetting::initToolbar()
{
    PageSelector *tb = ui->pageSelector;
    QAction *action;

    // 追加
    action = new QAction(QIcon(":/icons.black/settings.svg"), "一般", this);
    tb->addButton(action, generalPage, settings);

    // 追加
    action = new QAction(QIcon(":/icons.black/people.svg"), "アカウント", this);
    tb->addButton(action, accountPage, settings);
}
