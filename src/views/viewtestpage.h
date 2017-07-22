#ifndef VIEWTESTPAGE_H
#define VIEWTESTPAGE_H

#include "pageselector.h"

namespace Ui {
class ViewTestPage;
}

class ViewTestPage : public PageSelectorView
{
    Q_OBJECT

public:
    explicit ViewTestPage(QWidget *parent = 0);
    ~ViewTestPage();

private slots:
    void on_testRun_clicked();

private:
    Ui::ViewTestPage *ui;
};

#endif // VIEWTESTPAGE_H
