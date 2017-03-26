#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "twitter.h"

const QString tokenFileName = "Jugemutter.token"; // @todo あとで設定ファイルなりにする

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , twitter(new Twitter(this))
{
    ui->setupUi(this);

#ifndef QT_NO_DEBUG
    ui->tweetEditor->setPlainText("てすてす");
    on_tweetEditor_textChanged();
#endif

    connect(twitter, &Twitter::authenticated, this, &MainWindow::handleTwitterAuthenticated);

    loadConfig();
}

MainWindow::~MainWindow()
{
    saveConfig();

    delete ui;
}

void MainWindow::loadConfig()
{
    QSettings settings;

    settings.beginGroup("mainWindow");
    resize(settings.value("size", QSize(400, 400)).toSize());
    move(settings.value("pos", QPoint(200, 200)).toPoint());
    settings.endGroup();

    settings.beginGroup("twitter");
    QVariant serialized = settings.value("0");
    if (serialized.isValid()) {
        twitter->deserialize(serialized.toString());
    }
    settings.endGroup();
}

void MainWindow::saveConfig()
{
    QSettings settings;

    settings.beginGroup("mainWindow");
    settings.setValue("size", size());
    settings.setValue("pos", pos());
    settings.endGroup();

    settings.beginGroup("twitter");
    settings.setValue("0", twitter->serialize());
    settings.endGroup();
}

void MainWindow::handleTwitterAuthenticated()
{
    if (!tweetQueue.isEmpty()) {
        twitter->tweet(tweetQueue);
    }
}

void MainWindow::on_tweetButton_clicked()
{
    QString tweetText = ui->tweetEditor->toPlainText();
//    tweetButton->;

    // まずは認証済みかチェック
    if (!twitter->isAuthenticated()) {
        tweetQueue = tweetText;
        twitter->authenticate();
        return;
    }

    twitter->tweet(tweetText);

    return;
}

void MainWindow::on_tweetEditor_textChanged()
{
    ui->textLength->setText(QString("%1").arg( ui->tweetEditor->toPlainText().size() ));
}
