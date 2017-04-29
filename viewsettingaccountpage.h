#ifndef VEIWSETTINGACCOUNTPAGE_H
#define VEIWSETTINGACCOUNTPAGE_H

#include <QFrame>
#include "pageselector.h"

namespace Ui {
class ViewSettingAccountPage;
}

class ViewSettingAccountPage
    : public QFrame
{
    Q_OBJECT

public:
    explicit ViewSettingAccountPage(QWidget *parent = 0);
    ~ViewSettingAccountPage();

private:
    Ui::ViewSettingAccountPage *ui;
};

#endif // VEIWSETTINGACCOUNTPAGE_H
