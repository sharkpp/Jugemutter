#include "viewsetting.h"
#include "ui_viewsetting.h"
#include "viewsettinggeneralpage.h"
#include "viewsettingaccountpage.h"
#include "viewsettingaboutpage.h"

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
    : PageSelectorView(parent)
    , ui(new Ui::ViewSetting)
    , account(nullptr)
    , settings(new SettingPageDocument(this))
{
    ui->setupUi(this);
    ui->pageSelector->setBuddy(ui->pageContainer);
    ui->pageContainer->addWidget(generalPage = new ViewSettingGeneralPage(this));
    ui->pageContainer->addWidget(accountPage = new ViewSettingAccountPage(this));
    ui->pageContainer->addWidget(aboutPage = new ViewSettingAboutPage(this));
    initToolbar();
}

ViewSetting::~ViewSetting()
{
    delete ui;
}

void ViewSetting::setAccountList(AccountList *account)
{
    this->account = account;
}

void ViewSetting::initToolbar()
{
    PageSelector *tb = ui->pageSelector;
    QAction *action;

    // 一般設定
    action = new QAction(QIcon(":/icons.black/settings.svg"), "一般", this);
    tb->addButton(action, generalPage, settings);

    // アカウント設定
    action = new QAction(QIcon(":/icons.black/people.svg"), "アカウント", this);
    tb->addButton(action, accountPage, settings);

    // このアプリについて
    action = new QAction(QIcon(":/icons.black/info.svg"), "バージョン", this);
    tb->addButton(action, aboutPage, settings);
}
