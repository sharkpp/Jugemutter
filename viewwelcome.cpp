#include "viewwelcome.h"
#include "ui_viewwelcome.h"
#include "mainwindow.h"

ViewWelcome::ViewWelcome(QWidget *parent)
    : PageSelectorView(parent)
    , ui(new Ui::ViewWelcome)
{
    ui->setupUi(this);
}

ViewWelcome::~ViewWelcome()
{
    delete ui;
}

void ViewWelcome::on_addAccount_clicked()
{
    MainWindow::requestAddAccount(this);
}
