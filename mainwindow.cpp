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
    ui->tweetEditor->setPlainText("abcdef");
    on_tweetEditor_textChanged();
#endif

    connect(twitter, &Twitter::authenticated, this, &MainWindow::handleTwitterAuthenticated);

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

// prefix/postfixを除いた元々の文を取得する
QString MainWindow::getTweetRawText() const
{
    QString rawText;

    //ui->tweetEditor->toPlainText();
    QTextDocument *document = ui->tweetEditor->document();

    //QTextFrame *rootFrame = document->rootFrame();
    //for (QTextFrame::iterator ite = rootFrame->begin(),
    //                          last= rootFrame->end();
    //        ite != last; ++ite) {
    //    (*ite).firstCursorPosition()
    //}

    for (QTextBlock ite = document->firstBlock();
            ite.isValid(); ite = ite.next()) {
        qDebug() << QString("getTweetRawText #%1 [%2]").arg(ite.blockNumber()).arg(ite.text());
        rawText += ite.text();
    }

    return rawText;
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
    QString text = getTweetRawText();

    qDebug() << "on_tweetEditor_textChanged [" << text << "]";

    if (1<ui->partitionCount->text().toInt()) {
        //return;
    }

    ui->textLength->setText(QString("%1").arg( text.size() ));

    ui->partitionCount->setText(QString("%1").arg(ui->partitionCount->text().toInt()+1));

    bool oldState = ui->tweetEditor->blockSignals(true);

    QTextDocument *document = ui->tweetEditor->document();

    qDebug() << document->rootFrame()->childFrames().length();

    document->clear();

    QTextFrame *rootFrame = document->rootFrame();

    qDebug() << rootFrame->childFrames().length();

    QTextCursor cursor = rootFrame->firstCursorPosition();

    QTextFrameFormat textFrameFormat;
    textFrameFormat.setBorder(1);
    textFrameFormat.setBorderStyle(QTextFrameFormat::BorderStyle_Dotted);

    for (int i = 0; i < text.length(); ++i) {

        //cursor.insertText(QString("[%1]").arg(i));
        //cursor.movePosition(QTextCursor::PreviousBlock, QTextCursor::MoveAnchor);
        //cursor.select(QTextCursor::BlockUnderCursor);
        QTextFrame *curTextFrame = cursor.insertFrame(textFrameFormat);
        QTextCursor cursor2(curTextFrame);
        cursor2.insertText(QString(text.at(i) ));
        cursor.movePosition(QTextCursor::End, QTextCursor::MoveAnchor);

        /*
        //cursor.joinPreviousEditBlock();
        cursor.movePosition(QTextCursor::NextBlock, QTextCursor::MoveAnchor);
        QTextFrame *curTextFrame = cursor.insertFrame(textFrameFormat);
        //cursor.movePosition(QTextCursor::End, QTextCursor::MoveAnchor);
        QTextCursor cursor2(curTextFrame);
        //cursor.insertBlock();
        //qDebug() << "blockNumber" << cursor.blockNumber();
        cursor2.insertText(QString(text.at(i) ) + "\n@");

        //cursor2.movePosition(QTextCursor::StartOfBlock, QTextCursor::MoveAnchor);

        cursor2 = curTextFrame->firstCursorPosition();
        QTextBlock textBlock = cursor2.block();
        textBlock = textBlock.previous();
        qDebug() << QString(">> #%1 [%2]").arg(textBlock.blockNumber()).arg(textBlock.text());

        QTextCursor cursor3(textBlock);
        //cursor3.select(QTextCursor::BlockUnderCursor);
        cursor3.movePosition(QTextCursor::StartOfBlock, QTextCursor::MoveAnchor);
        cursor3.movePosition(QTextCursor::NextRow, QTextCursor::KeepAnchor);
        //cursor3.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor);
        //cursor3.removeSelectedText();
        cursor3.insertText("*");*/
    }

    qDebug() << rootFrame->childFrames().length();

    qDebug() << getTweetRawText();

    ui->tweetEditor->blockSignals(oldState);
}
