#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "pageselector.h"

class QFrame;

class Twitter;
class ViewNormalEditor;
class ViewSetting;
class AccountList;
class Account;
class Preference;

namespace Ui {
class MainWindow;
}

class EditorPageDocument
    : public PageSelectorDocument
{
    Q_OBJECT

public:
    EditorPageDocument(QObject* parent = nullptr);

    Account *account() const;
    void setAccount(Account *account);

private:
    Account *m_account;
};

class MainWindow
    : public QMainWindow
{
    Q_OBJECT

    struct ResetConfigInfo {
        ResetConfigInfo();
        //
        bool resetNeed;
        int  lazyTimerId;
    };

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    static void requestAddAccount(QWidget *parent);

protected: // method
    // UI関連
    void initToolbar();
    QAction *addAccount(Account *account);
    void attachTwitter(Twitter *twitter);
    // 設定関連
    void loadConfig();
    void saveConfig();
    void resetConfig();
    //
    bool readAccountConfig(const QString& serialized, qint32 accountType, QString& accountData);
    bool existAccount(Twitter *twitter);
    void requestAddAccount();

protected: // event
    bool event(QEvent* ev);

private slots:
    void onTwitterAuthenticated();
    void onTwitterVerified();
    void on_accountList_actionTriggered(QAction *action);
    void onAccountListUpdate();

private:
    Ui::MainWindow *ui;
    QFrame *welcomeView;
    ViewNormalEditor *editorView;
    ViewSetting *settingView;
    Preference *preference;
    AccountList *accountList;
    QAction *actionAccountAdd;
    QAction *actionSetting;
    ResetConfigInfo resetConfigInfo;
    Account* currentAccount;
};

#endif // MAINWINDOW_H
