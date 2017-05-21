#include "viewnormaleditor.h"
#include "ui_viewnormaleditor.h"
#include "mainwindow.h"
#include "twitter.h"
#include "accountlist.h"
#include "postprogress.h"
#include <QMessageBox>

ViewNormalEditor::ViewNormalEditor(QWidget *parent)
    : PageSelectorView(parent)
    , ui(new Ui::ViewNormalEditor)
    , postProgress(new PostProgress(this))
    , currentAccount(nullptr)
{
    ui->setupUi(this);
}

ViewNormalEditor::~ViewNormalEditor()
{
    delete postProgress;
    delete ui;
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
            ui->textPrefix->setPlainText("");
            ui->tweetEditor->setPlainText("");
            ui->textPostfix->setPlainText("");
            ui->tweetEditor->setFocus();
        }
    }
}

void ViewNormalEditor::updateSplitStatus()
{
    TwitterTextSplitter splitter;

    QString text = ui->tweetEditor->toPlainText();

    splitter.setPrefix( ui->textPrefix->toPlainText() );
    splitter.setText( text );
    splitter.setPostfix( ui->textPostfix->toPlainText() );

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

    splitter.setPrefix( ui->textPrefix->toPlainText() );
    splitter.setText( ui->tweetEditor->toPlainText() );
    splitter.setPostfix( ui->textPostfix->toPlainText() );

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
    ui->tweetEditor->setPrefix(ui->textPrefix->toPlainText());
    updateSplitStatus();
}

void ViewNormalEditor::on_textPostfix_textChanged()
{
    ui->tweetEditor->setPostfix(ui->textPostfix->toPlainText());
    updateSplitStatus();
}

void ViewNormalEditor::on_tweetEditor_textChanged()
{
    updateSplitStatus();
}
