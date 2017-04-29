#ifndef VIEWSETTING_H
#define VIEWSETTING_H

#include <QFrame>
#include "pageselector.h"

class ViewSettingGeneralPage;
class ViewSettingAccountPage;

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

class ViewSetting : public QFrame
{
    Q_OBJECT

public:
    explicit ViewSetting(QWidget *parent = 0);
    ~ViewSetting();

protected:
    void initToolbar();

private slots:

private:
    Ui::ViewSetting *ui;
    PageSelectorDocument *settings;
    ViewSettingGeneralPage *generalPage;
    ViewSettingAccountPage *accountPage;
};

#endif // VIEWSETTING_H
