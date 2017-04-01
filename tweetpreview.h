#ifndef TWEETPREVIEW_H
#define TWEETPREVIEW_H

//#include <QtCore>
//#include <QtGui>
//#include <QtWidgets>
#include <QTextEdit>
//#include <QString>

//class QKeyEvent;

class TweetPreview
    : public QTextEdit
{
    Q_OBJECT

public:
    explicit TweetPreview(QWidget *parent = 0);

protected: // method
    QString getTweetRawText() const;

protected: // event
    void keyPressEvent(QKeyEvent *e);

private slots:
    void on_textChanged();
};

#endif // TWEETPREVIEW_H
