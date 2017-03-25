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
    ui->tweetEditor->setPlainText("書き込みてすと #qtjp");
    on_tweetEditor_textChanged();
#endif

    connect(twitter, &Twitter::authenticated, this, &MainWindow::handleTwitterAuthenticated);

    tokensLoad();
}

MainWindow::~MainWindow()
{
    tokensSave();
    delete ui;
}

void MainWindow::tokensLoad()
{
    QFile file(QFileInfo(QDir::homePath(), tokenFileName).filePath());
    file.open(QIODevice::ReadOnly);
    QDataStream in(&file);

    QMap<QString, QString> tokenAndService;
    in >> tokenAndService;

    QMap<QString, QString>::const_iterator ite = tokenAndService.constBegin();
    while (ite != tokenAndService.constEnd()) {
        if (ite.key() == "twitter") {
            twitter->deserialize(ite.value());
        }
        ++ite;
    }
}

void MainWindow::tokensSave()
{
    QMap<QString, QString> tokenAndService;

    tokenAndService.insert("twitter", twitter->serialize());

    QFile file(QFileInfo(QDir::homePath(), tokenFileName).filePath());
    file.open(QIODevice::WriteOnly);
    QDataStream out(&file);
    out << tokenAndService;
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
