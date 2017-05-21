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

    tb->setStyleSheet(QString("QToolButton:pressed { background-color: %2; border: none; }")
        .arg( tb->palette().color(QPalette::Background).toRgb().name(QColor::HexRgb) )
        .arg( tb->palette().color(QPalette::AlternateBase).toRgb().name(QColor::HexRgb) ));

    tb->addAction(QIcon(":/icons.black/add.svg"), "アカウントを追加",
                  this, &ViewSettingAccountPage::on_accountAction_append);

    removeAccount = tb->addAction(QIcon(":/icons.black/remove.svg"), "アカウントを削除",
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

void ViewSettingAccountPage::on_accountAction_append(bool /*checked*/)
{
    // find main window
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
    QMessageBox msgBox;
    msgBox.setText("登録済みのアカウントを削除しようとしています。\n"
                   "この操作は即座に適用されます。\n"
                   "対象アカウントを再度利用するには登録し直す必要があります。");
    msgBox.setInformativeText("削除してもよろしいですか？");
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
