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

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:

    void loadConfig();
    void saveConfig();

private slots:
    void handleTwitterAuthenticated();
    void on_tweetButton_clicked();

    void on_tweetEditor_textChanged();

private:
    Ui::MainWindow *ui;
    Twitter *twitter; // Twitter access class
    QString tweetQueue; // queued tweet
};

#endif // MAINWINDOW_H
