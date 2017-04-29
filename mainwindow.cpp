#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "twitter.h"
#include "twittertextsplitter.h"
#include "accountaddpopup.h"
#include "viewnormaleditor.h"
#include "viewsetting.h"
#include "pageselector.h"
#include <QMessageBox>
#include <algorithm>

struct SearchDocumentByTwitterId {
    Twitter *twitter_;
    SearchDocumentByTwitterId(Twitter *twitter)
        : twitter_(twitter)
    { }
    bool operator()(const PageSelectorDocument* document) const {
      if (const EditorPageDocument *document_
              = qobject_cast<const EditorPageDocument*>(document)) {
          return document_->twitter()->id() == twitter_->id();
      }
      return false;
    }
};

//---------------------------------------------------------
// EditorPageDocument
//---------------------------------------------------------

EditorPageDocument::EditorPageDocument(QObject *parent)
    : PageSelectorDocument(parent)
    , m_twitter(nullptr)
{
}

Twitter *EditorPageDocument::twitter() const
{
    return m_twitter;
}

void EditorPageDocument::setTwitter(Twitter *twitter)
{
    m_twitter = twitter;
}

//---------------------------------------------------------
// MainWindow::ResetConfigInfo
//---------------------------------------------------------

MainWindow::ResetConfigInfo::ResetConfigInfo()
    : resetNeed(false)
    , lazyTimerId(-1)
{
}

//---------------------------------------------------------
// MainWindow
//---------------------------------------------------------

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
    ui->accountList->setBuddy(ui->pageContainer);
    ui->pageContainer->addWidget(editorView = new ViewNormalEditor(this));
    ui->pageContainer->addWidget(settingView = new ViewSetting(this));
    initToolbar();

    loadConfig();

    // 先頭を選択
    ui->accountList->setCurrentAction(ui->accountList->buttons().at(0)->action());

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
    PageSelector *tb = ui->accountList;
    QAction *action;

    // 追加
    action = actionAccountAdd = new QAction(QIcon(":/icons.white/add.svg"), "アカウントの追加", this);
    tb->addButton(action);

    // スペーサー
    tb->addSpacer();

    // 設定
    action = actionSetting = new QAction(QIcon(":/icons.white/settings.svg"), "設定", this);
    action->setCheckable(true);
    tb->addButton(action, settingView);
}

QAction *MainWindow::addAccount(Twitter *twitter)
{
    PageSelector *tb = ui->accountList;

    // 最後のアカウントの次に追加
    QAction *action = new QAction(twitter->icon(),
                                  twitter->name() + "\n" + twitter->screenName(), this);
    action->setCheckable(true);
    EditorPageDocument *document = new EditorPageDocument(this);
    document->setTwitter(twitter);
    return tb->insertButton(actionAccountAdd, action, editorView, document);
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
    PageSelector *tb = ui->accountList;
    QSettings settings;

    settings.beginGroup("mainWindow");
    settings.setValue("size", size());
    settings.setValue("pos", pos());
    settings.endGroup();

    settings.beginGroup("twitter");
    int accountNum = 0;
    QList<PageSelectorButton*> buttons = ui->accountList->buttons();
    for (auto button : buttons) {
        if (EditorPageDocument *document
                = qobject_cast<EditorPageDocument*>( tb->documentAt(button->action()) )) {
            settings.setValue(QString("%1").arg(accountNum),
                              document->twitter()->serialize());
            accountNum++;
        }
    }
    settings.setValue("num", QString("%1").arg(accountNum));
    settings.endGroup();
}

void MainWindow::resetConfig()
{
    QList<PageSelectorDocument*> documents
            = ui->accountList->documents();
    for (auto document : documents) {
        if (EditorPageDocument *document_
                    = qobject_cast<EditorPageDocument*>( document )) {
            Twitter *twitter = document_->twitter();
            document_->setTwitter(nullptr);
            delete twitter;
            ui->accountList->removeButton(document);
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
    QList<PageSelectorDocument*> documents = ui->accountList->documents();
    QList<PageSelectorDocument*>::iterator
            ite = std::find_if(documents.begin(), documents.end(),
                               SearchDocumentByTwitterId(twitter));
    if (documents.end() != ite) {
        // すでに登録済みっぽい
        return;
    }

    QAction *action = addAccount(twitter);
    ui->accountList->setCurrentAction(action);
}

void MainWindow::on_accountList_actionTriggered(QAction *action)
{
    PageSelector *tb = ui->accountList;

    if (actionAccountAdd == action) {
        // アカウント追加
        AccountAddPopup popup(this);

        // ※ currentTwitter->authenticate(); も中で行う

        if (popup.exec()) {
            Twitter *twitter = popup.account();
            QAction *action = addAccount(twitter);
            ui->accountList->setCurrentAction(action);
        }
    }
    else if (EditorPageDocument *document
                = qobject_cast<EditorPageDocument*>( tb->documentAt(action) )) {
        //document
    }
}
