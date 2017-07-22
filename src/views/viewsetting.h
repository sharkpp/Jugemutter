#ifndef VIEWSETTING_H
#define VIEWSETTING_H

#include "pageselector.h"

class ViewSettingGeneralPage;
class ViewSettingAccountPage;
class ViewSettingAboutPage;
class ViewTestPage;
class AccountList;
class Preference;

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

    void setAccountList(AccountList *accountList);

    void setPreference(Preference *preference);

protected:
    void initToolbar();

private slots:

private:
    Ui::ViewSetting *ui;
    AccountList *m_accountList;
    Preference *m_preference;
    PageSelectorDocument *settings;
    ViewSettingGeneralPage *generalPage;
    ViewSettingAccountPage *accountPage;
    ViewSettingAboutPage *aboutPage;
    ViewTestPage *testPage;
};

#endif // VIEWSETTING_H
