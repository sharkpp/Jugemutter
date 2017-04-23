#ifndef ACOUNTAPPENDDIALOG_H
#define ACOUNTAPPENDDIALOG_H

#include <QDialog>

class Twitter;

namespace Ui {
class AccountAddDialog;
}

class AccountAddDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AccountAddDialog(QWidget *parent = 0);
    ~AccountAddDialog();

    bool popup(Twitter* twitter);

private slots:
    void on_twitter_veridied();

    void on_rejected();

private:
    Ui::AccountAddDialog *ui;
};

#endif // ACOUNTAPPENDDIALOG_H
