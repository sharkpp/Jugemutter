#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "twitter.h"
#include <QMessageBox>

MainWindow::ResetConfigInfo::ResetConfigInfo()
    : resetNeed(false)
    , lazyTimerId(-1)
{
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , twitter(new Twitter(this))
{
    ui->setupUi(this);

#ifndef QT_NO_DEBUG
    //ui->tweetEditor->setPlainText("abcdef");
#endif

    ui->textPrefix->setPlaceholderText("各ツイートの先頭へ追加するテキスト");
    ui->textPostfix->setPlaceholderText("各ツイートの末尾へ追加するテキスト");

    connect(twitter, &Twitter::authenticated, this,
            &MainWindow::handleTwitterAuthenticated);

    loadConfig();

    if (QGuiApplication::queryKeyboardModifiers().testFlag(Qt::ControlModifier)) {
        resetConfigInfo.resetNeed = true;
    }
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

void MainWindow::resetConfig()
{
    twitter->deserialize("");
}

bool MainWindow::event(QEvent* ev)
{
    QTimerEvent *timerEvent_;

    if (QEvent::Show == ev->type() &&
        resetConfigInfo.resetNeed)
    {
        // なんでか showEvent() が飛んでこない...
        resetConfigInfo.resetNeed = false;
        resetConfigInfo.lazyTimerId = startTimer(50);
    }

    if (QEvent::Timer == ev->type() &&
        nullptr != (timerEvent_ = static_cast<QTimerEvent*>(ev)) &&
        resetConfigInfo.lazyTimerId == timerEvent_->timerId())
    {
        killTimer(timerEvent_->timerId());
        resetConfigInfo.lazyTimerId = -1;
        // 設定のリセット確認メッセージ
        QMessageBox cfgResetMsg;
        cfgResetMsg.setText("設定を初期化しますか？");
        cfgResetMsg.setStandardButtons(QMessageBox::Reset | QMessageBox::No);
        cfgResetMsg.setDefaultButton(QMessageBox::No);
        if (QMessageBox::No != cfgResetMsg.exec()) {
            resetConfig();
        }
    }

    return QMainWindow::event(ev);
}

// ツイッターの認証通知
void MainWindow::handleTwitterAuthenticated()
{
    if (!tweetQueue.isEmpty()) {
        twitter->tweet(tweetQueue);
    }
}

// ツイート投稿
void MainWindow::on_tweetButton_clicked()
{
    QString tweetText = ui->tweetEditor->toPlainText();

    // まずは認証済みかチェック
    if (!twitter->isAuthenticated()) {
        tweetQueue = tweetText;
        twitter->authenticate();
        return;
    }

    twitter->tweet(tweetText);

    return;
}
