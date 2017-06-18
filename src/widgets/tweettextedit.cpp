#include "tweettextedit.h"
#include "twittertextsplitter.h"

TweetTextEdit::TweetTextEdit(QWidget *parent)
    : QTextEdit(parent)
{
    connect(this, &QTextEdit::textChanged, this, &TweetTextEdit::on_textChanged);

}

void TweetTextEdit::updateHighlight()
{
    m_textSplitter.setText( toPlainText() );

    QList<SplittedItem> tweet = m_textSplitter.split();
    if (tweet.isEmpty()) {
        return;
    }

    blockSignals(true);

    QTextCursor cursor = textCursor();
    cursor.movePosition(QTextCursor::Start, QTextCursor::MoveAnchor);

    QTextCharFormat formatOdd, formatEven;
    formatOdd.setForeground (QColor(0,0,0));
    formatOdd.setBackground (QColor(255,255,255));
    formatEven.setForeground(QColor(0,0,0));
    formatEven.setBackground(QColor(220,236,246));
    bool odd = false;

    for (QList<SplittedItem>::iterator
            ite = tweet.begin(),
            last= tweet.end();
         ite != last; ++ite, odd = !odd) {
        cursor.setPosition(cursor.position() + ite->text().size(), QTextCursor::KeepAnchor);
        cursor.mergeCharFormat(odd ? formatOdd : formatEven);
        cursor.setPosition(cursor.position(), QTextCursor::MoveAnchor);
    }

    blockSignals(false);

}

void TweetTextEdit::on_textChanged()
{
    updateHighlight();
}

void TweetTextEdit::setTextSplitter(const TwitterTextSplitter &textSplitter)
{
    m_textSplitter = textSplitter;
    updateHighlight();
}
