#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class Twitter;

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
    // 設定関連
    void loadConfig();
    void saveConfig();
    void resetConfig();

protected: // event
    bool event(QEvent* ev);

private slots:
    void on_twitter_authenticated();
    void on_tweetButton_clicked();
    void on_acountAdd_clicked();
    void on_acountSelect_clicked(bool checked);
    void on_setting_clicked();

private:
    Ui::MainWindow *ui;
    QAction *actionAccountAdd;
    ResetConfigInfo resetConfigInfo;
    QList<AccountInfo> twitters; // Twitter access class
    Twitter* currentTwitter;
    QString tweetQueue; // queued tweet
};

#endif // MAINWINDOW_H
