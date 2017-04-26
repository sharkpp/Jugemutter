#include "accountaddpopup.h"
#include "ui_accountaddpopup.h"
#include "twitter.h"

AccountAddPopup::AccountAddPopup(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::AccountAddPopup)
    , currentAccount(nullptr)
{
    ui->setupUi(this);

    setWindowFlags(Qt::Sheet);
}

AccountAddPopup::~AccountAddPopup()
{
    delete ui;
}

int AccountAddPopup::exec()
{
    currentAccount = new Twitter(parent());

    connect(currentAccount, &Twitter::verified,
            this, &AccountAddPopup::on_twitter_veridied);

    currentAccount->authenticate();

    return QDialog::exec();
}

Twitter *AccountAddPopup::account()
{
    return currentAccount;
}
void AccountAddPopup::on_twitter_veridied()
{
    done(QDialog::Accepted);
}

void AccountAddPopup::on_rejected()
{
    delete currentAccount;
    currentAccount = nullptr;

    done(QDialog::Rejected);
}