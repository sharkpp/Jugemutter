#ifndef TWEETPREVIEW_H
#define TWEETPREVIEW_H

//#include <QtCore>
//#include <QtGui>
//#include <QtWidgets>
#include <QTextEdit>
//#include <QString>

//class QKeyEvent;

class TweetPreview
    : public QWidget
{
    Q_OBJECT

public:
    explicit TweetPreview(QWidget *parent = 0);

    QString toPlainText() const;

protected: // method

protected: // event
    void keyPressEvent(QKeyEvent *e);
    void paintEvent(QPaintEvent *event) override;

private slots:

private:
    QString rawTweet;
};

#endif // TWEETPREVIEW_H
