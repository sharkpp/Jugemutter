#ifndef TWEETTEXTEDIT_H
#define TWEETTEXTEDIT_H

#include <QTextEdit>

class TweetTextEdit : public QTextEdit
{
    Q_OBJECT

public:
    TweetTextEdit(QWidget *parent = Q_NULLPTR);

    void setPrefix(const QString &prefix);

    void setPostfix(const QString &postfix);

protected:
    void updateHighlight();

private slots:
    void on_textChanged();

private:
    QString m_prefix;
    QString m_postfix;
};

#endif // TWEETTEXTEDIT_H
