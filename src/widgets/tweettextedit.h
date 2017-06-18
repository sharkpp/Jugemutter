#ifndef TWEETTEXTEDIT_H
#define TWEETTEXTEDIT_H

#include <QTextEdit>
#include "twittertextsplitter.h"

class TweetTextEdit : public QTextEdit
{
    Q_OBJECT

public:
    TweetTextEdit(QWidget *parent = Q_NULLPTR);

    void setTextSplitter(const TwitterTextSplitter &textSplitter);

protected:
    void updateHighlight();

private slots:
    void on_textChanged();

private:
    TwitterTextSplitter m_textSplitter;
};

#endif // TWEETTEXTEDIT_H
