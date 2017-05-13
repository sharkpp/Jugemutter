#include "viewsettingaccountpage.h"
#include "ui_viewsettingaccountpage.h"
#include "accountmodel.h"
#include <QPushButton>
#include <QButtonGroup>
#include <QSpacerItem>
#include <QDebug>

ViewSettingAccountPage::ViewSettingAccountPage(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::ViewSettingAccountPage)
{
    ui->setupUi(this);

    AccountModel* model = new AccountModel(this);
    connect(model, &QAbstractItemModel::dataChanged,
            this, &ViewSettingAccountPage::on_account_dataChanged);

    QHeaderView *accountListVHeader = ui->accountList->verticalHeader();
    accountListVHeader->setSectionResizeMode(QHeaderView::Fixed);
    accountListVHeader->setDefaultSectionSize(48);
    ui->accountList->setShowGrid(true);

    ui->accountList->setModel(model);
    ui->accountList->setIconSize(QSize(48, 48));
//    ui->accountList->setColumnWidth(AccountModel::ColumnProfileImage, 48);
//    ui->accountList->setColumnWidth(AccountModel::ColumnSetting, 48);
//    ui->accountList->setColumnWidth(AccountModel::ColumnRemove, 48);
    //ui->accountList->setIndexWidget(ui->accountList->model()->index(0, 3, QModelIndex()), new QPushButton("****", this));
    //ui->accountList->setIndexWidget(ui->accountList->model()->index(1, 0, QModelIndex()), new QPushButton("****", this));
    //ui->accountList->setFirstColumnSpanned(1, QModelIndex(), true);
    ui->accountList->setSpan(1, AccountModel::ColumnProfileImage, 0, 2);
    ui->accountList->setSpan(1, AccountModel::ColumnSetting,      0, 2);

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
    qDebug() << topLeft << bottomRight << roles;
return;
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
                //button = new QPushButton(QIcon(":/icons.black/settings.svg"), "", this);
                //button->setIconSize(QSize(48, 48));
                //ui->accountList->setIndexWidget(index, button);
            }
            index = model->index(row, AccountModel::ColumnRemove);
            if (!(widget = ui->accountList->indexWidget(index))) {
                //button = new QPushButton(QIcon(":/icons.black/delete.svg"), "", this);
                //button->setIconSize(QSize(48, 48));
                //ui->accountList->setIndexWidget(index, button);
                QHBoxLayout *buttonGroupLayout = new QHBoxLayout(this);
                buttonGroupLayout->setSpacing(1);
                QWidget *buttons = new QWidget(this);
                buttonGroupLayout->addItem(new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));
                buttonGroupLayout->addWidget(button = new QPushButton(QIcon(":/icons.black/settings.svg"), "", this));
                button->setFlat(true);
                button->setContentsMargins(QMargins(0, 0, 0, 0));
                button->setIconSize(QSize(48, 48));
                button->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
                button->setStyleSheet("QPushButton:hover { background-color: #ff0000; }");
                buttonGroupLayout->addWidget(button = new QPushButton(QIcon(":/icons.black/delete.svg"), "", this));
                button->setFlat(true);
                button->setContentsMargins(QMargins(0, 0, 0, 0));
                button->setIconSize(QSize(48, 48));
                button->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
                buttonGroupLayout->addItem(new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));
                buttons->setLayout(buttonGroupLayout);
                buttons->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
                ui->accountList->setIndexWidget(index, buttons);
            }
        }
    }
}

void ViewSettingAccountPage::on_accountList_clicked(const QModelIndex &index)
{
    qDebug() << "on_accountList_clicked" << index;

}
