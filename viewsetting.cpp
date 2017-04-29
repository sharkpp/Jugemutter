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

    action = new QAction(QIcon(":/icons.black/settings.svg"), "一般", this);
    action->setCheckable(true);
    tb->addAction(action);

    action = new QAction(QIcon(":/icons.black/people.svg"), "アカウント", this);
    action->setCheckable(true);
    tb->addAction(action);

    connect(tb, &QToolBar::actionTriggered,
            this, &ViewSetting::on_pageSelect_actionTriggered);
}

void ViewSetting::on_pageSelect_actionTriggered(QAction *action)
{
    QToolBar *tb = ui->pageSelector;

}


