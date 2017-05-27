#ifndef VEIWSETTINGGENERALPAGE_H
#define VEIWSETTINGGENERALPAGE_H

#include "pageselector.h"

namespace Ui {
class ViewSettingGeneralPage;
}

class ViewSettingGeneralPage
    : public PageSelectorView
{
    Q_OBJECT

public:
    explicit ViewSettingGeneralPage(QWidget *parent = 0);
    ~ViewSettingGeneralPage();

protected:
    void resizeEvent(QResizeEvent *event);

private:
    Ui::ViewSettingGeneralPage *ui;
};

#endif // VEIWSETTINGGENERALPAGE_H
