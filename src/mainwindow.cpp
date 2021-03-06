#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "twitter.h"
#include "twittertextsplitter.h"
#include "accountaddpopup.h"
#include "viewwelcome.h"
#include "viewnormaleditor.h"
#include "viewsetting.h"
#include "viewtestpage.h"
#include "pageselector.h"
#include "accountlist.h"
#include "preference.h"
#include <QMessageBox>
#include <algorithm>

static const int dataStreamVersion = QDataStream::Qt_5_8;

struct SearchDocumentByTwitterId {
    Twitter *twitter_;
    SearchDocumentByTwitterId(Twitter *twitter)
        : twitter_(twitter)
    { }
    bool operator()(const PageSelectorDocument* document) const {
        if (const EditorPageDocument *document_
                = qobject_cast<const EditorPageDocument*>(document)) {
            if (const TwitterAccount *account_
                    = qobject_cast<const TwitterAccount*>(document_->account())) {
              return account_->twitter()->id() == twitter_->id();
            }
      }
      return false;
    }
};

struct SearchDocumentByAccount {
    Account *account_;
    SearchDocumentByAccount(Account *account)
        : account_(account)
    { }
    bool operator()(const PageSelectorDocument* document) const {
        if (const EditorPageDocument *document_
                = qobject_cast<const EditorPageDocument*>(document)) {
            return document_->account() == account_;
      }
      return false;
    }
};

//---------------------------------------------------------
// EditorPageDocument
//---------------------------------------------------------

EditorPageDocument::EditorPageDocument(QObject *parent)
    : PageSelectorDocument(parent)
    , m_account(nullptr)
{
}

Account *EditorPageDocument::account() const
{
    return m_account;
}

void EditorPageDocument::setAccount(Account *account)
{
    m_account = account;
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
    , preference(new Preference(this))
    , accountList(new AccountList(this))
    , actionAccountAdd(nullptr)
    , actionSetting(nullptr)
    , currentAccount(nullptr)
{
    ui->setupUi(this);
    ui->accountList->setBuddy(ui->pageContainer);
    ui->accountList->setBlankView(new ViewWelcome(this));
    ui->pageContainer->addWidget(editorView = new ViewNormalEditor(this));
    ui->pageContainer->addWidget(settingView = new ViewSetting(this));
#ifndef QT_NO_DEBUG
    ui->pageContainer->addWidget(testPage = new ViewTestPage(this));
#endif
    editorView->setPreference(preference);
    settingView->setAccountList(accountList);
    settingView->setPreference(preference);
    initToolbar();

    connect(accountList, &AccountList::updateAccount,
            this, &MainWindow::onAccountListUpdate);

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
    action = actionAccountAdd = new QAction(QIcon(":/icons.white/add.svg"), tr("Add account"), this);
    tb->addButton(action);

    // スペーサー
    tb->addSpacer();

#ifndef QT_NO_DEBUG
    // テスト
    action = new QAction(QIcon(":/icons.white/bug_report.svg"), "TEST", this);
    tb->addButton(action, testPage)->setCheckable(true);
#endif

    // 設定
    action = actionSetting = new QAction(QIcon(":/icons.white/settings.svg"), tr("Settings"), this);
    tb->addButton(action, settingView)->setCheckable(true);
}

void MainWindow::requestAddAccount(QWidget *parent)
{
    // find main window
    MainWindow *mainWindow;
    for (QWidget *w = parent;
         w && !(mainWindow = qobject_cast<MainWindow *>(w));
         w = w->parentWidget())
    { }
    if (mainWindow) {
        mainWindow->requestAddAccount();
    }
}

void MainWindow::requestAddAccount()
{
    AccountAddPopup popup(this);

    // ※ Twitter::authenticate(); も中で行う

    if (!popup.exec()) {
        return;
    }

    Twitter *twitter = popup.account();

    // test exist account
    if (existAccount(twitter)) {
        return;
    }

    TwitterAccount *account = new TwitterAccount(this);
    account->setTwitter(twitter);
    currentAccount = account;
    accountList->append(account);
}

bool MainWindow::existAccount(Twitter *twitter)
{
    // すでに登録済みのアカウントかどうかのチェック
    QList<PageSelectorDocument*> documents = ui->accountList->documents();
    QList<PageSelectorDocument*>::iterator
            ite = std::find_if(documents.begin(), documents.end(),
                               SearchDocumentByTwitterId(twitter));
    return documents.end() != ite;
}

QAction *MainWindow::addAccount(Account *account)
{
    PageSelector *tb = ui->accountList;

    // 最後のアカウントの次に追加
    QAction *action = nullptr;
    switch (account->type()) {
    case AccountTypeTwitter:
        if (const TwitterAccount *account_
                = qobject_cast<const TwitterAccount*>(account)) {
            action = new QAction(account_->twitter()->icon(),
                                 account_->twitter()->name() + "\n"
                                 + account_->twitter()->screenName(), this);
        }
        break;
    default: break;
    }
    if (action) {
        action->setCheckable(true);
        EditorPageDocument *document = new EditorPageDocument(this);
        document->setAccount(account);
        accountList->append(account);
        return tb->insertButton(actionAccountAdd, action, editorView, document);
    }
    return nullptr;
}

void MainWindow::attachTwitter(Twitter *twitter)
{
    connect(twitter, &Twitter::authenticated,
            this, &MainWindow::onTwitterAuthenticated);
    connect(twitter, &Twitter::verified,
            this, &MainWindow::onTwitterVerified);
}

void MainWindow::loadConfig()
{
    QSettings settings;

    preference->deserialize(settings.value("preference").toString());

    settings.beginGroup("mainWindow");
    resize(settings.value("size", QSize(400, 400)).toSize());
    move(settings.value("pos", QPoint(200, 200)).toPoint());
    settings.endGroup();

    settings.beginGroup("account");
    int accountNum = settings.value("num", QVariant("0")).toInt();
    for (int accountIndex = 0;
         accountIndex < accountNum; ++accountIndex) {
        QVariant serialized = settings.value(QString("%1").arg(accountIndex));
        if (serialized.isValid()) {
            // split type & data
            QByteArray dataBytes = QByteArray::fromBase64(serialized.toByteArray());
            QDataStream in(&dataBytes, QIODevice::ReadOnly);
            in.setVersion(dataStreamVersion);
            // deserialize
            qint32   accountType; in >> accountType;
            QString  accountData; in >> accountData;
            // create account instanse
            switch ((AccountType)accountType) {
            case AccountTypeTwitter: { // append Twitter account
                TwitterAccount* account = new TwitterAccount(this);
                account->deserialize(accountData);
                if (existAccount(account->twitter())) { // just in case
                    delete account; // delete, if not used
                }
                else {
                    attachTwitter(account->twitter());
                    accountList->append(account);
                }
                break; }
            default: break;
            }
        }
    }
    settings.endGroup();
}

void MainWindow::saveConfig()
{
    QSettings settings;

    settings.setValue("preference", preference->serialize());

    settings.beginGroup("mainWindow");
    settings.setValue("size", size());
    settings.setValue("pos", pos());
    settings.endGroup();

    settings.beginGroup("account");
    int accountNum = 0;
    for (QList<Account*>::const_iterator
             ite = accountList->begin(),
             last= accountList->end();
         ite != last; ++ite, ++accountNum) {
        // concat type & data
        QMap<QString, QVariant> serialized;
        QByteArray dataBytes;
        QDataStream out(&dataBytes, QIODevice::WriteOnly);
        out.setVersion(dataStreamVersion);
        // serialize
        out << (qint32)(*ite)->type();
        out << (*ite)->serialize();
        // write config
        settings.setValue(QString("%1").arg(accountNum),
                          dataBytes.toBase64());
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
            Account *account = document_->account();
            document_->setAccount(nullptr);
            delete account;
            ui->accountList->removeButton(document);
        }
    }
    currentAccount = nullptr;
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
        cfgResetMsg.setText(tr("Do you want to initialize the setting?"));
        cfgResetMsg.setStandardButtons(QMessageBox::Reset | QMessageBox::No);
        cfgResetMsg.setDefaultButton(QMessageBox::No);
        if (QMessageBox::No != cfgResetMsg.exec()) {
            resetConfig();
        }
    }

    return QMainWindow::event(ev);
}

// ツイッターの認証通知
void MainWindow::onTwitterAuthenticated()
{
    Twitter *twitter = qobject_cast<Twitter*>( sender() );
    qDebug() << QString(">>USER_TOKEN='%1'").arg(twitter->token());
    qDebug() << QString(">>USER_TOKEN_SECRET='%1'").arg(twitter->tokenSecret());
}

void MainWindow::onTwitterVerified()
{
    Twitter *twitter = qobject_cast<Twitter*>( sender() );

    // test exist account
    if (existAccount(twitter)) {
        return;
    }

    TwitterAccount *account = new TwitterAccount(this);
    account->setTwitter(twitter);
    accountList->append(account);
}

void MainWindow::on_accountList_actionTriggered(QAction *action)
{
    PageSelector *tb = ui->accountList;

    currentAccount = nullptr;

    if (actionAccountAdd == action) {
        // append account
        requestAddAccount(this);
    }
    else if (EditorPageDocument *document
                = qobject_cast<EditorPageDocument*>( tb->documentAt(action) )) {
        currentAccount = document->account();
        ui->accountList->setCurrentAction(action);
    }
}

void MainWindow::onAccountListUpdate()
{
    PageSelector *tb = ui->accountList;
    QList<PageSelectorDocument*> documents = tb->documents();

    // collect exists documents
    QList<EditorPageDocument *> currentAccounts;
    QList<PageSelectorButton*> buttons = tb->buttons();
    for (auto document : documents) {
        if (EditorPageDocument *document_
                = qobject_cast<EditorPageDocument*>( document )) {
            if (document_->account()) {
                currentAccounts.append(document_);
            }
        }
    }

    // compare
    QList<EditorPageDocument *> nextAccounts;
    for (QList<Account*>::const_iterator
             ite = accountList->begin(),
             last= accountList->end();
         ite != last; ++ite) {
        // search exists document
        QList<PageSelectorDocument*>::iterator
                iteDoc = std::find_if(documents.begin(), documents.end(),
                                   SearchDocumentByAccount(*ite));
        EditorPageDocument *document = nullptr;
        if (documents.end() != iteDoc) {
            if (EditorPageDocument *document_
                    = qobject_cast<EditorPageDocument*>( *iteDoc )) {
                nextAccounts.append(document_);
            }
        }
        else {
            // create new document
            document = new EditorPageDocument(this);
            document->setAccount(*ite);
            nextAccounts.append(document);
        }
    }

    tb->setUpdatesEnabled(false);
    tb->blockSignals(true);

    // hidden append button, if account exist
    actionAccountAdd->setVisible(nextAccounts.isEmpty());

    // remove
    tb->removeButtons(documents);
    // append
    QAction *currentAction = nullptr;
    for (auto document : nextAccounts) {
        //
        QAction *action = nullptr;
        switch (document->account()->type()) {
        case AccountTypeTwitter:
            if (const TwitterAccount *account_
                    = qobject_cast<const TwitterAccount*>(document->account())) {
                action = new QAction(account_->twitter()->icon(),
                                     account_->twitter()->name() + "\n"
                                     + account_->twitter()->screenName(), this);
            }
            break;
        default: break;
        }
        if (action) {
            action->setCheckable(true);
            tb->insertButton(actionAccountAdd, action, editorView, document);
            if (currentAccount == document->account() ||
                (!currentAccount && !currentAction)) {
                currentAccount = document->account();
                currentAction = action;
            }
        }
    }

    tb->blockSignals(false);
    tb->setUpdatesEnabled(true);

    tb->setCurrentAction(currentAction);
}
