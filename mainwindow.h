#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class Twitter;
//class TwitterTextSplitter;

namespace Ui {
class MainWindow;
}

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
    // 設定関連
    void loadConfig();
    void saveConfig();
    void resetConfig();
    // 画面初期化関連
    void initToolbar();

protected: // event
    bool event(QEvent* ev);

private slots:
    void on_twitter_authenticated();
    void on_tweetButton_clicked();
    void on_setting_clicked();

private:
    Ui::MainWindow *ui;
    ResetConfigInfo resetConfigInfo;
    Twitter *twitter; // Twitter access class
    QString tweetQueue; // queued tweet
};

#endif // MAINWINDOW_H
