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

private slots:
    void on_account_dataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles = QVector<int> ());
    void on_accountList_clicked(const QModelIndex &index);

private:
    Ui::ViewSettingAccountPage *ui;
};

#endif // VEIWSETTINGACCOUNTPAGE_H
