#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QFrame;

class Twitter;
class ViewNormalEditor;

namespace Ui {
class MainWindow;
}

class MainWindow
    : public QMainWindow
{
    Q_OBJECT

    friend struct SearchButtonInfoByAction;
    friend struct SearchButtonInfoByTwitter;
    friend struct SearchButtonInfoByTwitterId;

    struct ButtonInfo {
        ButtonInfo();
        ButtonInfo(QAction *action_, QFrame *frame_);
        //
        QAction *action;
        QFrame *frame;
        Twitter *twitter;
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
    void addButton(const ButtonInfo &button, QAction *actionBefore = nullptr);
    ButtonInfo addAccount(Twitter *twitter);
    void buttonSelect(QAction *action);
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
    void on_acountAdd_clicked();
    void on_acountSelect_clicked(bool checked);
    void on_setting_clicked();

private:
    Ui::MainWindow *ui;
    QFrame* welcomeView;
    ViewNormalEditor* editorView;
    QAction *actionAccountAdd;
    ResetConfigInfo resetConfigInfo;
    QList<ButtonInfo> buttons; // Twitter access class
    Twitter* currentTwitter;
};

#endif // MAINWINDOW_H
