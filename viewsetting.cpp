#include "viewsetting.h"
#include "ui_viewsetting.h"

ViewSetting::ViewSetting(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::ViewSetting)
{
    ui->setupUi(this);
    initToolbar();
}

ViewSetting::~ViewSetting()
{
    delete ui;
}

void ViewSetting::initToolbar()
{
    QToolBar *tb = ui->pageSelector;
    QAction *action;

    action = new QAction(QIcon(":/icons/settings_b.svg"), "一般", this);
    action->setCheckable(true);
    tb->addAction(action);

    action = new QAction(QIcon(":/icons/people.svg"), "アカウント", this);
    action->setCheckable(true);
    tb->addAction(action);
}