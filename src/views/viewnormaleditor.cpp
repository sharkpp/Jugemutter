#include "viewnormaleditor.h"
#include "ui_viewnormaleditor.h"
#include "mainwindow.h"
#include "preference.h"
#include "autosizetextedit.h"
#include "twitter.h"
#include "accountlist.h"
#include "postprogress.h"
#include <QMessageBox>

ViewNormalEditor::ViewNormalEditor(QWidget *parent)
    : PageSelectorView(parent)
    , ui(new Ui::ViewNormalEditor)
    , m_preference(nullptr)
    , postProgress(new PostProgress(this))
    , currentAccount(nullptr)
{
    ui->setupUi(this);
    ui->prefixFreeText->setVisible(false);
    ui->prefixContinueText->setVisible(false);
    ui->prefixFinishedText->setVisible(false);
    ui->postfixFreeText->setVisible(false);
    ui->postfixContinueText->setVisible(false);
    ui->postfixFinishedText->setVisible(false);
}

ViewNormalEditor::~ViewNormalEditor()
{
    delete postProgress;
    delete ui;
}

QWidget *ViewNormalEditor::getPostTextWidget(Preference::PostTextType type)
{
    switch (type) {
    case Preference::postTextPrefixFreeText:
        return ui->prefixFreeText;
    case Preference::postTextPrefixContinue:
        ui->prefixContinueText->setText(m_preference->postPrefixContinueText());
        return ui->prefixContinueText;
    case Preference::postTextPrefixFinished:
        ui->prefixFinishedText->setText(m_preference->postPrefixFinishedText());
        return ui->prefixFinishedText;
    case Preference::postTextPostfixFree:
        return ui->postfixFreeText;
    case Preference::postTextPostfixContinue:
        ui->postfixContinueText->setText(m_preference->postPostfixContinueText());
        return ui->postfixContinueText;
    case Preference::postTextPostfixFinished:
        ui->postfixFinishedText->setText(m_preference->postPostfixFinishedText());
        return ui->postfixFinishedText;
    default:
        return nullptr;
    }
}

void ViewNormalEditor::setPreference(Preference *preference)
{
    m_preference = preference;

    connect(m_preference, &Preference::update,
            this, &ViewNormalEditor::onPreferenceUpdate);

//    ui->prefix->blockSignals(true);
//    ui->postfix->blockSignals(true);
    onPreferenceUpdate();
//    ui->prefix->blockSignals(false);
//    ui->postfix->blockSignals(false);
}

void ViewNormalEditor::setDocument(PageSelectorDocument *document)
{
    PageSelectorView::setDocument(document);

    currentAccount = nullptr;

    if (EditorPageDocument *document_
            = qobject_cast<EditorPageDocument*>( document )) {
        if (const TwitterAccount *account_
                = qobject_cast<const TwitterAccount*>(document_->account())) {

            currentAccount = account_->twitter();

            connect(currentAccount, &Twitter::tweeted,
                  this, &ViewNormalEditor::onTwitterTweeted);

            ui->tweetButton->setEnabled(currentAccount->isAuthenticated());
            ui->prefixFreeText->setPlainText("");
            ui->tweetEditor->setPlainText("");
            ui->postfixFreeText->setPlainText("");
            ui->tweetEditor->setFocus();
        }
    }
}

void ViewNormalEditor::updateSplitStatus()
{
    TwitterTextSplitter splitter;

    QString text = ui->tweetEditor->toPlainText();

    splitter.setPrefix( ui->prefixFreeText->toPlainText() );
    splitter.setText( text );
    splitter.setPostfix( ui->postfixFreeText->toPlainText() );

    tweetQueue = splitter.split();

    ui->partitionCount->setText(QString("%1").arg(tweetQueue.size()));
    ui->textLength->setText(QString("%1").arg(text.size()));
    ui->totalLength->setText(QString("%1").arg(splitter.size()));
}

void ViewNormalEditor::startPost()
{
    postProgress->setCount( tweetQueue.size() );
    postProgress->show();
    postProgress->raise();
    postProgress->activateWindow();
}

void ViewNormalEditor::stepPost()
{
    postProgress->step();
}

void ViewNormalEditor::finishPost()
{
    postProgress->step();
    postProgress->hide();
}

void ViewNormalEditor::onPreferenceUpdate()
{
    for (QLayoutItem *child;
         (child = ui->prefixLayout->takeAt(0)) != 0;) {
        child->widget()->setVisible(false);
        delete child;
    }
    foreach (auto type, m_preference->postPrefixText()) {
        QWidget *w = getPostTextWidget(type);
        w->setVisible(true);
        ui->prefixLayout->addWidget(w);
    }

    for (QLayoutItem *child;
         (child = ui->postfixLayout->takeAt(0)) != 0;) {
        child->widget()->setVisible(false);
        delete child;
    }
    foreach (auto type, m_preference->postPostfixText()) {
        QWidget *w = getPostTextWidget(type);
        w->setVisible(true);
        ui->postfixLayout->addWidget(w);
    }
}

void ViewNormalEditor::onTwitterTweeted(const QString &tweetId)
{
    qDebug() << "add tweet" << tweetId;

    if (tweetQueue.isEmpty()) {
        finishPost();
        QMessageBox::information(this, qAppName(), "投稿を完了しました。");
        return;
    }

    // まだ残りがあれば投稿する

    stepPost();

    QString tweetText = tweetQueue.front().toString();
    tweetQueue.pop_front();

    currentAccount->tweet(tweetText, tweetId);
}

// ツイート投稿
void ViewNormalEditor::on_tweetButton_clicked()
{
    tweetQueue.clear();

    // まずは認証済みかチェック
    if (!currentAccount ||
        !currentAccount->isAuthenticated()) {
        return;
    }

    TwitterTextSplitter splitter;

    splitter.setPrefix( ui->prefixFreeText->toPlainText() );
    splitter.setText( ui->tweetEditor->toPlainText() );
    splitter.setPostfix( ui->postfixFreeText->toPlainText() );

    tweetQueue = splitter.split();
    if (tweetQueue.isEmpty()) {
        return;
    }

    startPost();

#if 1
    QString msg;
    QList<SplittedItem>::iterator i;
    for (i = tweetQueue.begin(); i != tweetQueue.end(); ++i)
        msg += QString("[%1]\n").arg(i->toString());
    QMessageBox::information(this, "", msg);
#endif

    QString tweetText = tweetQueue.front().toString();
    tweetQueue.pop_front();

    currentAccount->tweet(tweetText);
}

void ViewNormalEditor::on_textPrefix_textChanged()
{
    ui->tweetEditor->setPrefix(ui->prefixFreeText->toPlainText());
    updateSplitStatus();
}

void ViewNormalEditor::on_textPostfix_textChanged()
{
    ui->tweetEditor->setPostfix(ui->postfixFreeText->toPlainText());
    updateSplitStatus();
}

void ViewNormalEditor::on_tweetEditor_textChanged()
{
    updateSplitStatus();
}
