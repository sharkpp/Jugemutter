#include "tweetpreview.h"
#include <QtGui>

TweetPreview::TweetPreview(QWidget *parent)
    : QTextEdit(parent)
{
    //connect(this, &QTextEdit::textChanged, this, &TweetPreview::on_textChanged);
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
        qDebug() << QString("getTweetRawText #%1 [%2]").arg(ite.blockNumber()).arg(ite.text());
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

    qDebug() << "on_tweetEditor_textChanged [" << text << "]";

    //if (1<ui->partitionCount->text().toInt()) {
    //    //return;
    //}

    //ui->textLength->setText(QString("%1").arg( text.size() ));

    //ui->partitionCount->setText(QString("%1").arg(ui->partitionCount->text().toInt()+1));

    bool oldState = blockSignals(true);

    QTextDocument *doc = this->document();

    qDebug() << doc->rootFrame()->childFrames().length();

    doc->clear();

    QTextFrame *rootFrame = doc->rootFrame();

    qDebug() << rootFrame->childFrames().length();

    QTextCursor cursor = rootFrame->firstCursorPosition();

    QTextFrameFormat textFrameFormat;
    textFrameFormat.setBorder(1);
    textFrameFormat.setBorderStyle(QTextFrameFormat::BorderStyle_Dotted);

    for (int i = 0; i < text.length(); ) {

        //cursor.insertText(QString("[%1]").arg(i));
        //cursor.movePosition(QTextCursor::PreviousBlock, QTextCursor::MoveAnchor);
        //cursor.select(QTextCursor::BlockUnderCursor);
        QTextFrame *curTextFrame = cursor.insertFrame(textFrameFormat);
        QTextCursor cursor2(curTextFrame);
        cursor2.insertText(QString(text.mid(i, 140) ));
        i += 140;
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

    qDebug() << rootFrame->childFrames().length() << getTweetRawText();

    blockSignals(oldState);

}
