#include "accountadddialog.h"
#include "ui_newaccountdialog.h"
#include "twitter.h"

AccountAddDialog::AccountAddDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::AccountAddDialog)
{
    ui->setupUi(this);
}

AccountAddDialog::~AccountAddDialog()
{
    delete ui;
}

bool AccountAddDialog::popup(Twitter *twitter)
{
    connect(twitter, &Twitter::verified,
            this, &AccountAddDialog::on_twitter_veridied);

    twitter->authenticate();

    return QDialog::Rejected != exec();
}

void AccountAddDialog::on_twitter_veridied()
{
    done(QDialog::Accepted);
}

void AccountAddDialog::on_rejected()
{
    done(QDialog::Rejected);
}
