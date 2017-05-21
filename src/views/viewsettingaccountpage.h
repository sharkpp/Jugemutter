#ifndef VEIWSETTINGACCOUNTPAGE_H
#define VEIWSETTINGACCOUNTPAGE_H

#include "pageselector.h"

class QListWidgetItem;

class AccountList;

namespace Ui {
class ViewSettingAccountPage;
}

class ViewSettingAccountPage
    : public PageSelectorView
{
    Q_OBJECT

public:
    explicit ViewSettingAccountPage(QWidget *parent = 0);
    ~ViewSettingAccountPage();

    void setAccountList(AccountList* accountList);

protected:
    void initToolBar();

private slots:
    void onAccountListUpdate();
//    void on_accountList_currentRowChanged(int currentRow);
//    void on_accountList_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous);
    void on_accountList_itemSelectionChanged();
    void onAccountActionAppend(bool checked);
    void onAccountActionRemove(bool checked);

private:
    Ui::ViewSettingAccountPage *ui;
    QAction *removeAccount;
    AccountList *accountList;
};

#endif // VEIWSETTINGACCOUNTPAGE_H
