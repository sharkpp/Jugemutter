#ifndef VEIWSETTINGACCOUNTPAGE_H
#define VEIWSETTINGACCOUNTPAGE_H

#include "pageselector.h"

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

protected:
    void initToolBar();

private slots:
    void on_accountAction_append(bool checked);
    void on_accountAction_remove(bool checked);

private:
    Ui::ViewSettingAccountPage *ui;
};

#endif // VEIWSETTINGACCOUNTPAGE_H
