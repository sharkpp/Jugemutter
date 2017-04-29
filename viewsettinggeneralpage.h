#ifndef VEIWSETTINGGENERALPAGE_H
#define VEIWSETTINGGENERALPAGE_H

#include <QFrame>
#include "pageselector.h"

namespace Ui {
class ViewSettingGeneralPage;
}

class ViewSettingGeneralPage
    : public QFrame
{
    Q_OBJECT

public:
    explicit ViewSettingGeneralPage(QWidget *parent = 0);
    ~ViewSettingGeneralPage();

private:
    Ui::ViewSettingGeneralPage *ui;
};

#endif // VEIWSETTINGGENERALPAGE_H
