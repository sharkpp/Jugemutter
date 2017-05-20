#include "viewsettingaccountpage.h"
#include "ui_viewsettingaccountpage.h"
#include <QPushButton>
#include <QButtonGroup>
#include <QSpacerItem>
#include <QDebug>

ViewSettingAccountPage::ViewSettingAccountPage(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::ViewSettingAccountPage)
{
    ui->setupUi(this);
    initToolBar();
}

ViewSettingAccountPage::~ViewSettingAccountPage()
{
    delete ui;
}

void ViewSettingAccountPage::initToolBar()
{
    QToolBar *tb = ui->accountAction;

    tb->setIconSize(QSize(16, 16));

    tb->setStyleSheet(QString("QToolButton:pressed { background-color: %2; border: none; }")
        .arg( tb->palette().color(QPalette::Background).toRgb().name(QColor::HexRgb) )
        .arg( tb->palette().color(QPalette::AlternateBase).toRgb().name(QColor::HexRgb) ));

    tb->addAction(QIcon(":/icons.black/add.svg"), "アカウントを追加",
                  this, &ViewSettingAccountPage::on_accountAction_append);

    tb->addAction(QIcon(":/icons.black/remove.svg"), "アカウントを追加",
                  this, &ViewSettingAccountPage::on_accountAction_remove);
}

void ViewSettingAccountPage::on_accountAction_append(bool checked)
{

}

void ViewSettingAccountPage::on_accountAction_remove(bool checked)
{

}
