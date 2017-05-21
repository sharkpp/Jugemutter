#ifndef VEIWSETTINGACCOUNTPAGE_H
#define VEIWSETTINGACCOUNTPAGE_H

#include "pageselector.h"

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
    void on_accountList_update();
    void on_accountAction_append(bool checked);
    void on_accountAction_remove(bool checked);

private:
    Ui::ViewSettingAccountPage *ui;
    AccountList *accountList;
};

#endif // VEIWSETTINGACCOUNTPAGE_H
