#include "twittertextsplitter.h"
#include <QDebug>

QDebug operator<<(QDebug dbg, const SplittedItem &item)
{
    // QDebugの状態を保存
    QDebugStateSaver saver(dbg);

        // スペース出力しないよう設定
    dbg.nospace();

    // 出力
    dbg << "SplittedItem(";
    dbg << "prefix: " << item.prefix() << ", ";
    dbg << "text: " << QString::fromLocal8Bit(item.text().toLocal8Bit()) << ", ";
    dbg << "ostfix: " << item.postfix();
    dbg << ")";

    // QDebugオブジェクトを返す
    return dbg;
}

////////////////

SplittedItem::SplittedItem()
{

}

SplittedItem::~SplittedItem()
{

}

QString SplittedItem::toString() const
{
    QString delimit = " ";
    return (m_prefix.isEmpty() ? QString() : m_prefix + delimit)
        +  m_text
        +  (m_postfix.isEmpty() ? QString() : delimit + m_postfix);
}

QString SplittedItem::prefix() const
{
    return m_prefix;
}

void SplittedItem::setPrefix(const QString &prefix)
{
    m_prefix = prefix;
}

QString SplittedItem::postfix() const
{
    return m_postfix;
}

void SplittedItem::setPostfix(const QString &postfix)
{
    m_postfix = postfix;
}

QString SplittedItem::text() const
{
    return m_text;
}

void SplittedItem::setText(const QString &text)
{
    m_text = text;
}

////////////////

TwitterTextSplitter::TwitterTextSplitter()
{

}

QString TwitterTextSplitter::prefix() const
{
    return m_prefix;
}

void TwitterTextSplitter::setPrefix(const QString &prefix)
{
    m_prefix = prefix;
}

QString TwitterTextSplitter::postfix() const
{
    return m_postfix;
}

void TwitterTextSplitter::setPostfix(const QString &postfix)
{
    m_postfix = postfix;
}

QString TwitterTextSplitter::text() const
{
    return m_text;
}

void TwitterTextSplitter::setText(const QString &text)
{
    m_text = text;
}

QList<SplittedItem> TwitterTextSplitter::split()
{
    QList<SplittedItem> r;

    int maxSplitSize = 140
                     - (m_prefix.size()  ? m_prefix.size()  + 1 : 0)
                     - (m_postfix.size() ? m_postfix.size() + 1 : 0);

    int textLen = m_text.size();

    QRegExp findTerminate("(。|、|，|,|\\.|\n| )");
    qDebug() << "maxSplitSize" << maxSplitSize;
    qDebug() << "textLen" << textLen;

    for (int offset = 0; offset < textLen; ) {
        SplittedItem item;
        if (!m_prefix.isEmpty()) {
            item.setPrefix(m_prefix);
        }
        if (!m_postfix.isEmpty()) {
            item.setPostfix(m_postfix);
        }

        QString trimText = m_text.mid(offset, maxSplitSize);

        // テスト：ケツから句読点を探す
        int trimPos = trimText.lastIndexOf(findTerminate);
qDebug() << "trimText.size()" << trimText.size() << "trimPos" << trimPos;
        //if (maxSplitSize / 2 < trimText.size() - trimPod) {
        if (trimPos) {
            trimText = trimText.mid(0, trimPos + 1);
        }

        item.setText(trimText);
        offset += trimText.size();

        r.push_back(item);
    }

    return r;
}
