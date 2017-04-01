#include "tweetpreview.h"
#include <QtGui>
#include <algorithm> // std::advance

TweetPreview::TweetPreview(QWidget *parent)
    : QWidget(parent)
{
    setBackgroundRole(QPalette::Base);
    setAutoFillBackground(true);
    setFocusPolicy(Qt::StrongFocus);
}

QString TweetPreview::toPlainText() const
{
    return rawTweet;
}

void TweetPreview::keyPressEvent(QKeyEvent *e)
{
    qDebug() << e->key() << e->text();
    rawTweet += e->text();
    this->update();
    return;
    /*
    if (Qt::Key_Backspace == e->key()) {
        QTextCursor cursor = textCursor();
        cursor.movePosition(QTextCursor::Left, QTextCursor::KeepAnchor);
        cursor.removeSelectedText();
        return;
    }
*/
    QWidget::keyPressEvent(e);
}

void TweetPreview::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QRect geo = this->geometry();

    painter.drawEllipse(0,0,100,100);

    for (int pos = 0; pos < rawTweet.length();) {
        const QString tweet = rawTweet.mid(pos, 140);


        const QRect rectangle = geo;
        QRect boundingRect;
        painter.drawText(rectangle, Qt::TextWordWrap, tweet, &boundingRect);


        pos += 140;
    }
}
