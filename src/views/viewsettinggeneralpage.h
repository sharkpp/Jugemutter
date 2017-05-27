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
    void updateStateList();

protected:
    void resizeEvent(QResizeEvent *event);

private slots:
    void on_postContinueText_textChanged(const QString &arg1);

    void on_postFinishedText_textChanged(const QString &arg1);

private:
    Ui::ViewSettingGeneralPage *ui;
};

#endif // VEIWSETTINGGENERALPAGE_H
