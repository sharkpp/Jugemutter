#include "tweetpreview.h"
#include <QtGui>
#include <algorithm> // std::advance

TweetPreview::TweetPreview(QWidget *parent)
    : QTextEdit(parent)
{
    connect(this, &QTextEdit::textChanged, this, &TweetPreview::on_textChanged);


    QTextFrameFormat textFrameFormat;
    textFrameFormat.setBorder(1);
    textFrameFormat.setBorderStyle(QTextFrameFormat::BorderStyle_Dotted);

    QTextDocument *doc = document();
    QTextFrame *rootFrame = doc->rootFrame();
    QTextCursor cursor = rootFrame->firstCursorPosition();
    QTextFrame *curTextFrame = cursor.insertFrame(textFrameFormat);
    //QTextCursor cursor2(curTextFrame);
    //cursor2.insertText("a");
}

// prefix/postfixを除いた元々の文を取得する
QString TweetPreview::getTweetRawText() const
{
    QString rawText;

    //ui->tweetEditor->toPlainText();
    QTextDocument *doc = document();

    //QTextFrame *rootFrame = document->rootFrame();
    //for (QTextFrame::iterator ite = rootFrame->begin(),
    //                          last= rootFrame->end();
    //        ite != last; ++ite) {
    //    (*ite).firstCursorPosition()
    //}

    for (QTextBlock ite = doc->firstBlock();
            ite.isValid(); ite = ite.next()) {
        //qDebug() << QString("getTweetRawText #%1 [%2]").arg(ite.blockNumber()).arg(ite.text());
        rawText += ite.text();
    }

    return rawText;
}

void TweetPreview::keyPressEvent(QKeyEvent *e)
{
    /*
    if (Qt::Key_Backspace == e->key()) {
        QTextCursor cursor = textCursor();
        cursor.movePosition(QTextCursor::Left, QTextCursor::KeepAnchor);
        cursor.removeSelectedText();
        return;
    }
*/
    QTextEdit::keyPressEvent(e);
}

void TweetPreview::on_textChanged()
{
    QString text = getTweetRawText();

    qDebug() << "======================================";
    qDebug() << "TweetPreview::on_textChanged [" << text << "], " << text.length();

    //bool oldState = blockSignals(true);

    QTextDocument *doc = this->document();

    QTextFrame *rootFrame = doc->rootFrame();
    qDebug() << "childFrames.len=" << rootFrame->childFrames().length();

/*
        QTextFrame *curTextFrame = cursor.insertFrame(textFrameFormat);
        QTextCursor cursor2(curTextFrame);
        cursor2.insertText(QString(text.mid(i, 140) ));
  */
    QTextFrameFormat textFrameFormat;
    textFrameFormat.setBorder(1);
    textFrameFormat.setBorderStyle(QTextFrameFormat::BorderStyle_Dotted);

    //QTextFrame::iterator iteFrame  = rootFrame->begin();
    //QTextFrame::iterator lastFrame = rootFrame->end();
    for (int posText = 0, idxFrame = 0; posText < text.length(); ) {
        /*if (lastFrame == iteFrame) {
            qDebug() << "@@@@";
            QTextCursor cursor = rootFrame->lastCursorPosition();
            cursor.insertFrame(textFrameFormat);
            lastFrame = rootFrame->end();
            iteFrame  = rootFrame->begin();
            //std::advance(iteFrame, idxFrame);
            for (int i = 0; i < idxFrame; ++i, ++iteFrame) {
                continue;
            }
        }*/
        //QTextFrame* curFrame = iteFrame.currentFrame();
        //QTextCursor cursor(curFrame);
        //QTextBlock curBlock = cursor.block();
        //QTextBlock curBlock = iteFrame.currentBlock();

        qDebug() << QString("childFrames.len=%1").arg(rootFrame->childFrames().length());

        if (rootFrame->childFrames().length() <= idxFrame) {
            qDebug() << QString("childFrames.len=%1").arg(rootFrame->childFrames().length()) << QString("idxFrame=%1").arg(idxFrame);
            //QTextCursor cursor_ = rootFrame->lastCursorPosition();
            QTextCursor cursor_(rootFrame);
            cursor_.insertFrame(textFrameFormat);
        }
        QTextFrame* curFrame = rootFrame->childFrames()[idxFrame];
        QTextCursor cursor(curFrame);
        QTextBlock curBlock = cursor.block();

        QString curTweet = text.mid(posText, 140);

        qDebug() << QString("posText=%1").arg(posText) << QString("idxFrame=%1").arg(idxFrame)
                 << QString("blockNumber=%1").arg(curBlock.blockNumber())
                 << QString("childFrames.len=%1").arg(rootFrame->childFrames().length())
                 << curTweet << curBlock.text().mid(0, 256);
        qDebug() << "-----------";

        if (curTweet != curBlock.text()) {
            cursor.clearSelection();
            cursor.select(QTextCursor::BlockUnderCursor);
            cursor.insertText(curTweet);
            return;
        }

        posText += 140;
        //++iteFrame;
        ++idxFrame;
    }

    //blockSignals(oldState);

    qDebug() << "======================================";

}
