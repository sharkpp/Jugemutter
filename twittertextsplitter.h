#ifndef TWITTERTEXTSPLITTER_H
#define TWITTERTEXTSPLITTER_H

#include <QObject>
#include <QList>

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
    QString m_prefix;
    QString m_postfix;
    QString m_text;
};

QDebug operator<<(QDebug dbg, const SplittedItem &item);

class TwitterTextSplitter
{
public:
    TwitterTextSplitter();

    // ツイートの先頭に追加するテキスト
    QString prefix() const;
    void setPrefix(const QString &prefix);

    // ツイートの末尾に設定するテキスト
    QString postfix() const;
    void setPostfix(const QString &postfix);

    // ツイート本文
    QString text() const;
    void setText(const QString &text);

    // 継続時に追加するテキスト
    // 継続時に追加するテキストの位置(先頭(prefixの前) or 先頭(prefixの後ろ) or 末尾(postfixの前) or 末尾(postfixの後))
    // 完了時に追加するテキスト
    // 完了時に追加するテキストの位置(先頭(prefixの前) or 先頭(prefixの後ろ) or 末尾(postfixの前) or 末尾(postfixの後))

    QList<SplittedItem> split();

    int size() const;

protected:
    QString m_prefix;
    QString m_postfix;
    QString m_text;
    int m_totalLength;
};

#endif // TWITTERTEXTSPLITTER_H
