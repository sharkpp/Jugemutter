#include "viewsettingaccountpage.h"
#include "ui_viewsettingaccountpage.h"
#include "accountmodel.h"
#include <QPushButton>

ViewSettingAccountPage::ViewSettingAccountPage(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::ViewSettingAccountPage)
{
    ui->setupUi(this);

    AccountModel* model = new AccountModel(this);
    connect(model, &QAbstractItemModel::dataChanged,
            this, &ViewSettingAccountPage::on_account_dataChanged);

    ui->accountList->setModel(model);
    ui->accountList->setIconSize(QSize(48, 48));
//    ui->accountList->setColumnWidth(AccountModel::ColumnProfileImage, 48);
//    ui->accountList->setColumnWidth(AccountModel::ColumnSetting, 48);
//    ui->accountList->setColumnWidth(AccountModel::ColumnRemove, 48);
    //ui->accountList->setIndexWidget(ui->accountList->model()->index(0, 3, QModelIndex()), new QPushButton("****", this));
    //ui->accountList->setIndexWidget(ui->accountList->model()->index(1, 0, QModelIndex()), new QPushButton("****", this));
    ui->accountList->setFirstColumnSpanned(1, QModelIndex(), true);

    on_account_dataChanged(model->index(0, AccountModel::ColumnSetting),
                           model->index(1, AccountModel::ColumnSetting));

}

ViewSettingAccountPage::~ViewSettingAccountPage()
{
    delete ui;
}

void ViewSettingAccountPage::on_account_dataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles)
{
    QAbstractItemModel* model = ui->accountList->model();
    QModelIndex index;
    QWidget *widget;
    QPushButton *button;

    for (int row = topLeft.row();
         row <= bottomRight.row(); ++row) {
        if (row + 1 == ui->accountList->model()->rowCount()) {
            index = model->index(row, AccountModel::ColumnProfileImage);
            if (!(widget = ui->accountList->indexWidget(index))) {
                button = new QPushButton(QIcon(":/icons.black/add.svg"), "追加", this);
                button->setIconSize(QSize(48, 48));
                ui->accountList->setIndexWidget(index, button);
            }
        }
        else {
            index = model->index(row, AccountModel::ColumnSetting);
            if (!(widget = ui->accountList->indexWidget(index))) {
                button = new QPushButton(QIcon(":/icons.black/settings.svg"), "", this);
                button->setIconSize(QSize(48, 48));
                ui->accountList->setIndexWidget(index, button);
            }
            index = model->index(row, AccountModel::ColumnRemove);
            if (!(widget = ui->accountList->indexWidget(index))) {
                button = new QPushButton(QIcon(":/icons.black/delete.svg"), "", this);
                button->setIconSize(QSize(48, 48));
                ui->accountList->setIndexWidget(index, button);
            }
        }
    }
}
