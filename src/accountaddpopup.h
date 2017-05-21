#ifndef ACOUNTADDPOPUP_H
#define ACOUNTADDPOPUP_H

#include <QDialog>

class Twitter;

namespace Ui {
class AccountAddPopup;
}

class AccountAddPopup : public QDialog
{
    Q_OBJECT

public:
    explicit AccountAddPopup(QWidget *parent = 0);
    ~AccountAddPopup();

    int exec();

    Twitter* account();

private slots:
    void onTwitterVeridied();
    void on_rejected();

private:
    Ui::AccountAddPopup *ui;
    Twitter *currentAccount;
};

#endif // ACOUNTADDPOPUP_H
