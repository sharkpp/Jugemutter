#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "twitter.h"
#include "twittertextsplitter.h"
#include <QMessageBox>
#include <algorithm>

struct SearchAccountInfoByAction {
    QAction *action_;
    SearchAccountInfoByAction(QAction *action) : action_(action) {
    }
    bool operator()(const MainWindow::AccountInfo& info) const {
      return info.action == action_;
    }
};

struct SearchAccountInfoByTwitter {
    Twitter *twitter_;
    SearchAccountInfoByTwitter(Twitter *twitter) : twitter_(twitter) {
    }
    bool operator()(const MainWindow::AccountInfo& info) const {
      return info.twitter == twitter_;
    }
};

struct SearchAccountInfoByTwitterId {
    Twitter *twitter_;
    SearchAccountInfoByTwitterId(Twitter *twitter) : twitter_(twitter) {
    }
    bool operator()(const MainWindow::AccountInfo& info) const {
      return info.twitter->id() == twitter_->id();
    }
};

MainWindow::AccountInfo::AccountInfo()
    : action(nullptr)
    , twitter(nullptr)
{
}

MainWindow::ResetConfigInfo::ResetConfigInfo()
    : resetNeed(false)
    , lazyTimerId(-1)
{
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , currentTwitter(nullptr)
{
    ui->setupUi(this);
    initToolbar();

    loadConfig();

    // 先頭を選択
    on_acountSelect_clicked(true);

    if (QGuiApplication::queryKeyboardModifiers().testFlag(Qt::ControlModifier)) {
        resetConfigInfo.resetNeed = true;
    }
}

MainWindow::~MainWindow()
{
    saveConfig();

    delete ui;
}

void MainWindow::initToolbar()
{
    QToolBar *tb = ui->accountList;

    actionAccountAdd
        = tb->addAction(QIcon(":/icons/add.svg"),
                        "アカウントの追加", this, &MainWindow::on_acountAdd_clicked);

    // スペーサー
    QWidget* spacer = new QWidget();
    spacer->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    tb->addWidget(spacer);

    tb->addAction(QIcon(":/icons/setting.svg"),
                  "設定", this, &MainWindow::on_setting_clicked);
}

void MainWindow::addAccount(Twitter *twitter)
{
    QAction *action = new QAction(twitter->icon(), twitter->name() + "\n" + twitter->screenName(), this);
    connect(action, &QAction::triggered,
            this, &MainWindow::on_acountSelect_clicked);
    action->setCheckable(true);
    ui->accountList->insertAction(actionAccountAdd, action);

    AccountInfo info;
    info.twitter = twitter;
    info.action = action;
    twitters.push_back(info);
}

void MainWindow::selectAccount(Twitter *twitter)
{
    for (QList<AccountInfo>::iterator
            ite = twitters.begin(),
            last= twitters.end();
        ite != last; ++ite) {
        ite->action->setChecked( ite->twitter == twitter );
    }
    currentTwitter = twitter;

    ui->tweetButton->setEnabled(true);
}

void MainWindow::loadConfig()
{
    QSettings settings;

    settings.beginGroup("mainWindow");
    resize(settings.value("size", QSize(400, 400)).toSize());
    move(settings.value("pos", QPoint(200, 200)).toPoint());
    settings.endGroup();

    settings.beginGroup("twitter");
    int accountNum = settings.value("num", QVariant("0")).toInt();
    for (int accountIndex = 0;
         accountIndex < accountNum; ++accountIndex) {
        QVariant serialized = settings.value(QString("%1").arg(accountIndex));
        if (serialized.isValid()) {
            Twitter *twitter = new Twitter(this);
            connect(twitter, &Twitter::authenticated,
                    this, &MainWindow::on_twitter_authenticated);
            connect(twitter, &Twitter::verified,
                    this, &MainWindow::on_twitter_verified);
            twitter->deserialize(serialized.toString());
            addAccount(twitter);
        }
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
    settings.setValue("num", QString("%1").arg(twitters.size()));
    int accountIndex = 0;
    for (QList<AccountInfo>::iterator
            ite = twitters.begin(),
            last= twitters.end();
        ite != last; ++ite, ++accountIndex) {
        settings.setValue(QString("%1").arg(accountIndex), ite->twitter->serialize());
    }
    settings.endGroup();
}

void MainWindow::resetConfig()
{
    for (QList<AccountInfo>::iterator
            ite = twitters.begin(),
            last= twitters.end();
        ite != last; ++ite) {
        ui->accountList->removeAction(ite->action);
        delete ite->twitter;
    }
    twitters.clear();
    currentTwitter = nullptr;
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
void MainWindow::on_twitter_authenticated()
{    
}

void MainWindow::on_twitter_verified()
{
    Twitter *twitter = qobject_cast<Twitter*>( sender() );

    if (twitter == currentTwitter) {
        // すでに登録済みのアカウントかどうかのチェック
        QList<AccountInfo>::iterator
                ite = std::find_if(twitters.begin(), twitters.end(),
                                   SearchAccountInfoByTwitterId(twitter));
        if (twitters.end() != ite) {
            // すでに登録済みっぽい
            return;
        }

        addAccount(twitter);

        selectAccount(twitter);

        return;
    }
}

// ツイート投稿
void MainWindow::on_tweetButton_clicked()
{
    QString tweetText = ui->tweetEditor->toPlainText();

    TwitterTextSplitter splitter;

    splitter.setPrefix( ui->textPrefix->toPlainText() );
    splitter.setText( tweetText );
    splitter.setPostfix( ui->textPostfix->toPlainText() );

    QList<SplittedItem> items = splitter.split();

//    qDebug() << items;

    QString msg;
    QList<SplittedItem>::iterator i;
    for (i = items.begin(); i != items.end(); ++i)
        msg += QString("[%1]\n").arg(i->toString());
    QMessageBox::information(this, "", msg);

return;
    // まずは認証済みかチェック
    /*if (!twitter->isAuthenticated()) {
        tweetQueue = tweetText;
        twitter->authenticate();
        return;
    }

    twitter->tweet(tweetText);
*/
    return;
}

// アカウント追加
void MainWindow::on_acountAdd_clicked()
{
    if (!currentTwitter) {
        currentTwitter = new Twitter(this);
        connect(currentTwitter, &Twitter::authenticated, this,
                &MainWindow::on_twitter_authenticated);
        connect(currentTwitter, &Twitter::verified,
                this, &MainWindow::on_twitter_verified);
    }
    currentTwitter->authenticate();
}

// 登録済みのどれかのアカウントを選択
void MainWindow::on_acountSelect_clicked(bool checked)
{
    QAction *action = qobject_cast<QAction*>( sender() );

    if (!checked) {
        action->setChecked(true);
        return;
    }

    ui->tweetButton->setEnabled(false);

    if (!action) {
        // 誰も選択していない？
        if (twitters.isEmpty()) {
            return;
        }
        action = twitters.front().action;
    }

    QList<AccountInfo>::iterator
            ite = std::find_if(twitters.begin(), twitters.end(),
                               SearchAccountInfoByAction(action));
    if (twitters.end() == ite) {
        return;
    }

    selectAccount(ite->twitter);
}

// 設定
void MainWindow::on_setting_clicked()
{
    //QMessageBox::information(this, "", "on_setting_clicked");
    qDebug() << "MainWindow::on_setting_clicked";
}
