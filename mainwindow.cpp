#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "twitter.h"
#include "twittertextsplitter.h"
#include "accountaddpopup.h"
#include "viewnormaleditor.h"
#include "viewsetting.h"
#include <QMessageBox>
#include <algorithm>

struct SearchButtonInfoByAction {
    QAction *action_;
    SearchButtonInfoByAction(QAction *action) : action_(action) {
    }
    bool operator()(const MainWindow::ButtonInfo& info) const {
      return info.action == action_;
    }
};

struct SearchButtonInfoByTwitter {
    Twitter *twitter_;
    SearchButtonInfoByTwitter(Twitter *twitter) : twitter_(twitter) {
    }
    bool operator()(const MainWindow::ButtonInfo& button) const {
      return button.twitter && button.twitter == twitter_;
    }
};

struct SearchButtonInfoByTwitterId {
    Twitter *twitter_;
    SearchButtonInfoByTwitterId(Twitter *twitter) : twitter_(twitter) {
    }
    bool operator()(const MainWindow::ButtonInfo& button) const {
      return button.twitter && button.twitter->id() == twitter_->id();
    }
};

MainWindow::ButtonInfo::ButtonInfo()
    : action(nullptr)
    , frame(nullptr)
    , twitter(nullptr)
{
}

MainWindow::ButtonInfo::ButtonInfo(QAction *action_, QFrame *frame_)
    : action(action_)
    , frame(frame_)
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
    , welcomeView(nullptr)
    , editorView(nullptr)
    , settingView(nullptr)
    , actionAccountAdd(nullptr)
    , actionSetting(nullptr)
    , currentTwitter(nullptr)
{
    ui->setupUi(this);
    ui->pageContainer->addWidget(welcomeView = new QFrame(this));
    ui->pageContainer->addWidget(editorView = new ViewNormalEditor(this));
    ui->pageContainer->addWidget(settingView = new ViewSetting(this));
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

    delete editorView;
    delete welcomeView;
    delete ui;
}

void MainWindow::initToolbar()
{
    QToolBar *tb = ui->accountList;
    QAction *action;

    // 追加
    action = actionAccountAdd = new QAction(QIcon(":/icons/add.svg"), "アカウントの追加", this);
    connect(action, &QAction::triggered,
            this, &MainWindow::on_acountAdd_clicked);
    addButton(ButtonInfo(action, welcomeView));

    // スペーサー
    QWidget* spacer = new QWidget();
    spacer->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    tb->addWidget(spacer);

    // 設定
    action = actionSetting = new QAction(QIcon(":/icons/settings.svg"), "設定", this);
    action->setCheckable(true);
    connect(action, &QAction::triggered,
            this, &MainWindow::on_setting_clicked);
    addButton(ButtonInfo(action, welcomeView));
}

void MainWindow::addButton(const ButtonInfo &button, QAction *actionBefore)
{
    QToolBar *tb = ui->accountList;

    if (actionBefore) {
        tb->insertAction(actionBefore, button.action);
        //
        QList<ButtonInfo>::iterator
                ite = std::find_if(buttons.begin(), buttons.end(),
                                   SearchButtonInfoByAction(actionBefore));
        if (buttons.end() != ite) {
            buttons.insert(ite, button);
        }
        else {
            buttons.push_back(button);
        }
    }
    else {
        tb->addAction(button.action);
        buttons.push_back(button);
    }
}

MainWindow::ButtonInfo MainWindow::addAccount(Twitter *twitter)
{
/*    QAction *actionBefore = nullptr;
    QList<ButtonInfo>::iterator
            ite = std::find_if(buttons.begin(), buttons.end(),
                               SearchButtonInfoByAction(actionAccountAdd));
    if (buttons.begin() != ite) {
        ite--;
        actionBefore = ite->action;
    }*/


    // 最後のアカウントの次に追加
    QAction *action = new QAction(twitter->icon(),
                                  twitter->name() + "\n" + twitter->screenName(), this);
    action->setCheckable(true);
    connect(action, &QAction::triggered,
            this, &MainWindow::on_acountSelect_clicked);

    ButtonInfo button(action, editorView);
    button.twitter = twitter;

    addButton(button, actionAccountAdd);

    return button;
}

void MainWindow::buttonSelect(QAction *action)
{
    for (QList<ButtonInfo>::iterator
            ite = buttons.begin(),
            last= buttons.end();
        ite != last; ++ite) {
        ite->action->setChecked( ite->action == action );
        if (ite->twitter &&
            ite->action == action) {
            ViewNormalEditor *frame = qobject_cast<ViewNormalEditor*>( ite->frame );
            if (frame) {
                frame->setAccount(ite->twitter);
                ui->pageContainer->setCurrentWidget(ite->frame);
            }
        }
    }
}

Twitter *MainWindow::newTwitter(QObject *parent)
{
    Twitter *twitter = new Twitter(parent);

    connect(twitter, &Twitter::authenticated,
            this, &MainWindow::on_twitter_authenticated);
    connect(twitter, &Twitter::verified,
            this, &MainWindow::on_twitter_verified);

    return twitter;
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
            Twitter *twitter = newTwitter(this);
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
    int accountIndex = 0, accountNum = 0;
    for (QList<ButtonInfo>::iterator
            ite = buttons.begin(),
            last= buttons.end();
        ite != last; ++ite, ++accountIndex) {
        if (ite->twitter) {
            settings.setValue(QString("%1").arg(accountIndex),
                              ite->twitter->serialize());
            accountNum++;
        }
    }
    settings.setValue("num", QString("%1").arg(accountNum));
    settings.endGroup();
}

void MainWindow::resetConfig()
{
    for (QList<ButtonInfo>::iterator
            ite = buttons.begin(),
            last= buttons.end();
        ite != last; ) {
        if (!ite->twitter) {
            ++ite;
        }
        else {
            ButtonInfo button = *ite;
            ui->accountList->removeAction(button.action);
            delete button.twitter;
            ite = buttons.erase(ite);
            last= buttons.end();
        }
    }
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
    Twitter *twitter = qobject_cast<Twitter*>( sender() );
    qDebug() << QString(">>USER_TOKEN='%1'").arg(twitter->token());
    qDebug() << QString(">>USER_TOKEN_SECRET='%1'").arg(twitter->tokenSecret());
}

void MainWindow::on_twitter_verified()
{
    Twitter *twitter = qobject_cast<Twitter*>( sender() );

    // すでに登録済みのアカウントかどうかのチェック
    QList<ButtonInfo>::iterator
            ite = std::find_if(buttons.begin(), buttons.end(),
                               SearchButtonInfoByTwitterId(twitter));
    if (buttons.end() != ite) {
        // すでに登録済みっぽい
        return;
    }

    ButtonInfo button = addAccount(twitter);
    buttonSelect(button.action);
}

// アカウント追加
void MainWindow::on_acountAdd_clicked()
{
    AccountAddPopup popup(this);

    // ※ currentTwitter->authenticate(); も中で行う

    if (popup.exec()) {
        Twitter *twitter = popup.account();
        ButtonInfo button = addAccount(twitter);
        buttonSelect(button.action);
    }
}

// 登録済みのどれかのアカウントを選択
void MainWindow::on_acountSelect_clicked(bool checked)
{
    QAction *action = qobject_cast<QAction*>( sender() );

    if (!checked) {
        action->setChecked(true);
        return;
    }

    if (!action) {
        // 誰も選択していない？
        if (buttons.isEmpty() ||
            !buttons.front().twitter) {
            return;
        }
        action = buttons.front().action;
    }

    QList<ButtonInfo>::iterator
            ite = std::find_if(buttons.begin(), buttons.end(),
                               SearchButtonInfoByAction(action));
    if (buttons.end() == ite ||
        !ite->twitter) {
        return;
    }

    buttonSelect(ite->action);
}

// 設定
void MainWindow::on_setting_clicked()
{
    //QMessageBox::information(this, "", "on_setting_clicked");
    qDebug() << "MainWindow::on_setting_clicked";

    buttonSelect(actionSetting);
    ui->pageContainer->setCurrentWidget(settingView);
}
