#ifndef TWITTERTEXTSPLITTER_H
#define TWITTERTEXTSPLITTER_H

#include <QObject>
#include <QList>
#include <QPair>

// フォーマット
// ----------------------------
// {prefix}
// {text}
// {postfix}
// ----------------------------

class SplittedItem
{
    //friend QDebug operator<<(QDebug dbg, const SplittedItem &item);

public:
    SplittedItem();
    virtual ~SplittedItem();

    QString toString() const;

    QString prefix() const;
    void setPrefix(const QString &prefix);

    QString postfix() const;
    void setPostfix(const QString &postfix);

    QString text() const;
    void setText(const QString &text);

    int size() const;

protected:
    QString m_prefix;  // 本文前の文字列
    QString m_postfix; // 本文後の文字列
    QString m_text;    // 本文
};

QDebug operator<<(QDebug dbg, const SplittedItem &item);

class TwitterTextSplitter
{
public:

    enum TextType {
        textPrefixFreeText = 1,
        textPrefixContinue,
        textPrefixFinished,
        textPostfixFreeText,
        textPostfixContinue,
        textPostfixFinished,
    };

    typedef QPair<TextType, QString> TextTypeValue;

public:
    TwitterTextSplitter();

    // ツイートの先頭に追加するテキスト
    const QList<TextTypeValue> prefix() const;
    void setPrefix(const QList<TextTypeValue> &prefix);

    // ツイートの末尾に設定するテキスト
    const QList<TextTypeValue> postfix() const;
    void setPostfix(const QList<TextTypeValue> &postfix);

    // ツイート本文
    QString text() const;
    void setText(const QString &text);

    // 分割
    QList<SplittedItem> split();

    int size() const;

protected:
    QList<TextTypeValue> m_prefix;
    QList<TextTypeValue> m_postfix;
    QString m_text;
    int m_totalLength;
};

#endif // TWITTERTEXTSPLITTER_H
