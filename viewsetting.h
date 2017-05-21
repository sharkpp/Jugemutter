#ifndef VIEWSETTING_H
#define VIEWSETTING_H

#include "pageselector.h"

class ViewSettingGeneralPage;
class ViewSettingAccountPage;
class ViewSettingAboutPage;
class AccountList;

namespace Ui {
class ViewSetting;
}

class SettingPageDocument
    : public PageSelectorDocument
{
    Q_OBJECT

public:
    SettingPageDocument(QObject* parent = nullptr);

private:
};

class ViewSetting
        : public PageSelectorView
{
    Q_OBJECT

public:
    explicit ViewSetting(QWidget *parent = 0);
    ~ViewSetting();

    void setAccountList(AccountList* accountList);

protected:
    void initToolbar();

private slots:

private:
    Ui::ViewSetting *ui;
    AccountList *m_accountList;
    PageSelectorDocument *settings;
    ViewSettingGeneralPage *generalPage;
    ViewSettingAccountPage *accountPage;
    ViewSettingAboutPage *aboutPage;
};

#endif // VIEWSETTING_H
