#ifndef VIEWWELCOME_H
#define VIEWWELCOME_H

#include "pageselector.h"

namespace Ui {
class ViewWelcome;
}

class ViewWelcome : public PageSelectorView
{
    Q_OBJECT

public:
    explicit ViewWelcome(QWidget *parent = 0);
    ~ViewWelcome();

private slots:
    void on_addAccount_clicked();

private:
    Ui::ViewWelcome *ui;
};

#endif // VIEWWELCOME_H
