#include "accountaddpopup.h"
#include "ui_accountaddpopup.h"
#include "twitter.h"

AccountAddPopup::AccountAddPopup(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::AccountAddPopup)
    , currentAccount(nullptr)
{
#if defined(Q_OS_MAC)
    setWindowFlags(Qt::Sheet);
#endif

    ui->setupUi(this);

    adjustSize();
}

AccountAddPopup::~AccountAddPopup()
{
    delete ui;
}

int AccountAddPopup::exec()
{
    currentAccount = new Twitter(parent());

    connect(currentAccount, &Twitter::verified,
            this, &AccountAddPopup::onTwitterVeridied);

    currentAccount->authenticate();

    return QDialog::exec();
}

Twitter *AccountAddPopup::account()
{
    return currentAccount;
}
void AccountAddPopup::onTwitterVeridied()
{
    done(QDialog::Accepted);
}

void AccountAddPopup::on_rejected()
{
    delete currentAccount;
    currentAccount = nullptr;

    done(QDialog::Rejected);
}
