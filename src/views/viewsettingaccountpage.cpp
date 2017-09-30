#include "viewsettingaccountpage.h"
#include "ui_viewsettingaccountpage.h"
#include "mainwindow.h"
#include "accountlist.h"
#include "twitter.h"
#include <QMessageBox>
#include <QDebug>

ViewSettingAccountPage::ViewSettingAccountPage(QWidget *parent)
    : PageSelectorView(parent)
    , ui(new Ui::ViewSettingAccountPage)
    , removeAccount(nullptr)
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

    tb->setStyleSheet(QString("QToolButton:pressed { background-color: %1; border: none; }")
        .arg( tb->palette().color(QPalette::AlternateBase).toRgb().name(QColor::HexRgb) ));

    tb->addAction(QIcon(":/icons.black/add.svg"), tr("Add account"),
                  this, &ViewSettingAccountPage::onAccountActionAppend);

    removeAccount = tb->addAction(QIcon(":/icons.black/remove.svg"), tr("Unregister account"),
                                  this, &ViewSettingAccountPage::onAccountActionRemove);
}

void ViewSettingAccountPage::setAccountList(AccountList *accountList)
{
    this->accountList = accountList;

    connect(accountList, &AccountList::updateAccount,
            this, &ViewSettingAccountPage::onAccountListUpdate);
}

void ViewSettingAccountPage::onAccountListUpdate()
{
    QListWidget *list = ui->accountList;

    QListWidgetItem *item = list->currentItem();
    Account *selectedAccount
                = item ? static_cast<Account *>( item->data(Qt::UserRole).value<void *>() )
                       : nullptr;
    int selectedRow = -1;

    if (0 == accountList->size()) {
        list->clear();
    }
    else {

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
                item->setData(Qt::UserRole, QVariant::fromValue(static_cast<void *>( *ite )) );
                list->addItem(item);
                // set selected row index
                if (selectedAccount == *ite) {
                    selectedRow = list->row(item);
                }
            }
        }

        list->blockSignals(false);
        list->setUpdatesEnabled(true);
    }

    list->setCurrentRow(selectedRow < 0 ? 0 : selectedRow);

}

//void ViewSettingAccountPage::on_accountList_currentRowChanged(int currentRow)
//{
//    qDebug() << (void*)ui->accountList->currentItem() << ui->accountList->selectedItems().size() << "on_accountList_currentRowChanged(" << currentRow << ")";
//}

//void ViewSettingAccountPage::on_accountList_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous)
//{
//    qDebug() << (void*)ui->accountList->currentItem() << ui->accountList->selectedItems().size() << "on_accountList_currentItemChanged(" << (void*)current << "," << (void*)previous << ")";
//}

void ViewSettingAccountPage::on_accountList_itemSelectionChanged()
{
//    qDebug() << (void*)ui->accountList->currentItem() << ui->accountList->selectedItems().size() << "on_accountList_itemSelectionChanged()";
    QListWidgetItem *item = ui->accountList->currentItem();
    removeAccount->setEnabled(!!item);
}

void ViewSettingAccountPage::onAccountActionAppend(bool /*checked*/)
{
    MainWindow::requestAddAccount(this);
}

void ViewSettingAccountPage::onAccountActionRemove(bool /*checked*/)
{
    QMessageBox msgBox;
    msgBox.setText(tr("You are about to delete a registered account.\n"
                      "This operation will be applied immediately.\n"
                      "To use the target account again, you need to register again."));
    msgBox.setInformativeText(tr("Are you sure you want to unregister?"));
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::No);
    if (QMessageBox::Yes != msgBox.exec()) {
        return;
    }

    //
    QListWidgetItem *item = ui->accountList->currentItem();

    Account *account = static_cast<Account *>( item->data(Qt::UserRole).value<void *>() );
    accountList->remove(account);
}
