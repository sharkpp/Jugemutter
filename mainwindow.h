#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "pageselector.h"

class QFrame;

class Twitter;
class ViewNormalEditor;
class ViewSetting;

namespace Ui {
class MainWindow;
}

class EditorPageDocument
    : public PageSelectorDocument
{
    Q_OBJECT

public:
    EditorPageDocument(QObject* parent = nullptr);

    Twitter *twitter() const;
    void setTwitter(Twitter *twitter);

private:
    Twitter *m_twitter;
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

protected: // method
    // UI関連
    void initToolbar();
    QAction *addAccount(Twitter *twitter);
    Twitter *newTwitter(QObject *parent);
    // 設定関連
    void loadConfig();
    void saveConfig();
    void resetConfig();

protected: // event
    bool event(QEvent* ev);

private slots:
    void on_twitter_authenticated();
    void on_twitter_verified();
    void on_accountList_actionTriggered(QAction *action);

private:
    Ui::MainWindow *ui;
    QFrame* welcomeView;
    ViewNormalEditor* editorView;
    ViewSetting* settingView;
    QAction *actionAccountAdd;
    QAction *actionSetting;
    ResetConfigInfo resetConfigInfo;
    Twitter* currentTwitter;
};

#endif // MAINWINDOW_H
