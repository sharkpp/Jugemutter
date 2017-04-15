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
    void loadConfig();
    void saveConfig();
    void resetConfig();

protected: // event
    bool event(QEvent* ev);

private slots:
    void handleTwitterAuthenticated();
    void on_tweetButton_clicked();

private:
    Ui::MainWindow *ui;
    ResetConfigInfo resetConfigInfo;
    Twitter *twitter; // Twitter access class
    QString tweetQueue; // queued tweet
};

#endif // MAINWINDOW_H
