#include "viewsettingaccountpage.h"
#include "ui_viewsettingaccountpage.h"
#include "mainwindow.h"
#include "accountlist.h"
#include "twitter.h"
#include <QPushButton>
#include <QButtonGroup>
#include <QSpacerItem>
#include <QDebug>

ViewSettingAccountPage::ViewSettingAccountPage(QWidget *parent)
    : PageSelectorView(parent)
    , ui(new Ui::ViewSettingAccountPage)
    , accountList(nullptr)
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

    tb->addAction(QIcon(":/icons.black/remove.svg"), "アカウントを削除",
                  this, &ViewSettingAccountPage::on_accountAction_remove);
}

void ViewSettingAccountPage::setAccountList(AccountList *accountList)
{
    this->accountList = accountList;

    connect(accountList, &AccountList::updateAccount,
            this, &ViewSettingAccountPage::on_accountList_update);
}

void ViewSettingAccountPage::on_accountList_update()
{
    QListWidget *list = ui->accountList;

    list->blockSignals(true);
    list->setUpdatesEnabled(false);

    list->clear();

    for (QList<Account*>::const_iterator
             ite = accountList->begin(),
             last= accountList->end();
         ite != last; ++ite) {
        QListWidgetItem *item = nullptr;
        switch ((*ite)->type()) {
        case AccountTypeTwitter:
            if (const TwitterAccount *account_
                    = qobject_cast<const TwitterAccount*>(*ite)) {
                item = new QListWidgetItem(account_->twitter()->icon(),
                                     account_->twitter()->name() + "\n"
                                     + account_->twitter()->screenName(), list);
            }
            break;
        default: break;
        }
        if (item) {
            list->addItem(item);
        }
    }

    list->blockSignals(false);
    list->setUpdatesEnabled(true);

}

void ViewSettingAccountPage::on_accountAction_append(bool /*checked*/)
{
    MainWindow *mainWindow;
    for (QWidget *w = parentWidget();
         w && !(mainWindow = qobject_cast<MainWindow *>(w));
         w = w->parentWidget())
    { }
    if (mainWindow) {
        mainWindow->requestAddAccount();
    }
}

void ViewSettingAccountPage::on_accountAction_remove(bool /*checked*/)
{

}
