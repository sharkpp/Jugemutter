#ifndef VIEWSETTINGABOUTPAGE_H
#define VIEWSETTINGABOUTPAGE_H

#include "pageselector.h"

namespace Ui {
class ViewSettingAboutPage;
}

class ViewSettingAboutPage : public PageSelectorView
{
    Q_OBJECT

public:
    explicit ViewSettingAboutPage(QWidget *parent = 0);
    ~ViewSettingAboutPage();

private:
    Ui::ViewSettingAboutPage *ui;
};

#endif // VIEWSETTINGABOUTPAGE_H
