#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "twittertextsplitter.h"

class Twitter;
class AccountAddDialog;

namespace Ui {
class MainWindow;
}

class MainWindow
    : public QMainWindow
{
    Q_OBJECT

    friend struct SearchAccountInfoByAction;
    friend struct SearchAccountInfoByTwitter;
    friend struct SearchAccountInfoByTwitterId;

    struct AccountInfo {
        AccountInfo();
        //
        QAction* action;
        Twitter* twitter;
    };

    struct ResetConfigInfo {
        ResetConfigInfo();
        //
        bool resetNeed;
        int  lazyTimerId;
    };

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected: // method
    // UI関連
    void initToolbar();
    void addAccount(Twitter *twitter);
    void selectAccount(Twitter *twitter);
    Twitter *newTwitter(QObject *parent);
    void updateSplitStatus();
    // 設定関連
    void loadConfig();
    void saveConfig();
    void resetConfig();

protected: // event
    bool event(QEvent* ev);

private slots:
    void on_twitter_authenticated();
    void on_twitter_verified();
    void on_twitter_tweeted(const QString& tweetId);
    void on_tweetButton_clicked();
    void on_acountAdd_clicked();
    void on_acountSelect_clicked(bool checked);
    void on_setting_clicked();
    void on_textPrefix_textChanged();
    void on_textPostfix_textChanged();
    void on_tweetEditor_textChanged();

private:
    Ui::MainWindow *ui;
    AccountAddDialog* accountAddDialog;
    QAction *actionAccountAdd;
    ResetConfigInfo resetConfigInfo;
    QList<AccountInfo> twitters; // Twitter access class
    Twitter* currentTwitter;
    QList<SplittedItem> tweetQueue; // queued tweet
};

#endif // MAINWINDOW_H
