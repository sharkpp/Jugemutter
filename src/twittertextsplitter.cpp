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
    QString delimit = "";
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

int SplittedItem::size() const
{
    return (m_prefix.isEmpty() ? 0 : m_prefix.size() + 1)
        +  m_text.size()
        +  (m_postfix.isEmpty() ? 0 : 1 + m_postfix.size());

}

////////////////

TwitterTextSplitter::TwitterTextSplitter()
    : m_maxTweetLength(140)
    , m_totalLength(-1)
{

}

int TwitterTextSplitter::maxTweetLength() const
{
    return m_maxTweetLength;
}

void TwitterTextSplitter::setMaxTweetLength(int maxTweetLength)
{
    m_maxTweetLength = maxTweetLength;
}

// ツイートの先頭に追加するテキスト
const QList<TwitterTextSplitter::TextTypeValue> TwitterTextSplitter::prefix() const
{
    return m_prefix;
}

void TwitterTextSplitter::setPrefix(const QList<TwitterTextSplitter::TextTypeValue> &prefix)
{
    QList<TwitterTextSplitter::TextTypeValue> list;

    foreach (auto typePair, prefix) {
        if (typePair.first == textPrefixFreeText ||
            typePair.first == textPrefixContinue ||
            typePair.first == textPrefixFinished ) {
            list.append(typePair);
        }
    }

    m_prefix = list;
}

// ツイートの末尾に設定するテキスト
const QList<TwitterTextSplitter::TextTypeValue> TwitterTextSplitter::postfix() const
{
    return m_postfix;
}

void TwitterTextSplitter::setPostfix(const QList<TwitterTextSplitter::TextTypeValue> &postfix)
{
    QList<TwitterTextSplitter::TextTypeValue> list;

    foreach (auto typePair, postfix) {
        if (typePair.first == textPostfixFreeText ||
            typePair.first == textPostfixContinue ||
            typePair.first == textPostfixFinished ) {
            list.append(typePair);
        }
    }

    m_postfix = list;
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

    m_totalLength = 0;

    int textLen = m_text.size();

    QRegExp findTerminate("(。|、|，|,|\\.|\n| )");

    //qDebug() << "m_text=" << m_text;
    //qDebug() << "m_text.len=" << textLen;
    //{ QString s; foreach (auto typePair, m_prefix) { if ("" == typePair.second) continue; switch (typePair.first) { default: break; case textPrefixFreeText: s += "free "; break; case textPrefixContinue: s += "cont "; break; case textPrefixFinished: s += "fin "; break; } } qDebug() << "prefix" << s; }
    //{ QString s; foreach (auto typePair, m_postfix) { if ("" == typePair.second) continue; switch (typePair.first) { default: break; case textPostfixFreeText: s += "free "; break; case textPostfixContinue: s += "cont "; break; case textPostfixFinished: s += "fin "; break; } } qDebug() << "postfix" << s; }

    int flagsSkipMask = 15;

    // 本文前の文字列を生成
    //   各パターンでの文字を生成
    //   ※ textPrefixFreeText は常に含まれる
    //          dec bin :         2          |          1
    //   flags =  0  00 :                 (none)
    //   flags =  1  01 :                    | textPrefixContinue
    //   flags =  2  10 : textPrefixFinished |
    //   flags =  3  11 : textPrefixFinished | textPrefixContinue
    const int prefixTextListFlagsMax = 4;
    QString prefixTextList[prefixTextListFlagsMax];
    //bool    prefixTextListValid[prefixTextListFlagsMax] = {};
    for (int flags = 0; flags < 4; ++flags) {
        QString &s = prefixTextList[flags];
        foreach (auto typePair, m_prefix) {
            switch (typePair.first) {
            default: break;
            case textPrefixFreeText:
                s += typePair.second;
                break;
            case textPrefixContinue:
                if (0 != (flags & 1) &&
                        !typePair.second.isEmpty()) {
                    s += typePair.second;
                    flagsSkipMask &= ~1;
                }
                break;
            case textPrefixFinished:
                if (0 != (flags & 2) &&
                        !typePair.second.isEmpty()) {
                    s += typePair.second;
                    flagsSkipMask &= ~2;
                }
                break;
            }
        }
    }

    // 本文後の文字列を生成
    //   各パターンでの文字を生成
    //   ※ textPostfixFreeText は常に含まれる
    //          dec bin :          2          |           1
    //   flags =  0  00 :                  (none)
    //   flags =  1  01 :                     | textPostfixContinue
    //   flags =  2  10 : textPostfixFinished |
    //   flags =  3  11 : textPostfixFinished | textPostfixContinue
    const int postfixTextListFlagsMax = 4;
    QString postfixTextList[postfixTextListFlagsMax];
    //bool    postfixTextListValid[postfixTextListFlagsMax] = {};
    for (int flags = 0; flags < 4; ++flags) {
        QString &s = postfixTextList[flags];
        foreach (auto typePair, m_postfix) {
            switch (typePair.first) {
            default: break;
            case textPostfixFreeText:
                s += typePair.second;
                break;
            case textPostfixContinue:
                if (0 != (flags & 1) &&
                        !typePair.second.isEmpty()) {
                    s += typePair.second;
                    flagsSkipMask &= ~(1 << 2);
                }
                break;
            case textPostfixFinished:
                if (0 != (flags & 2) &&
                        !typePair.second.isEmpty()) {
                    s += typePair.second;
                    flagsSkipMask &= ~(2 << 2);
                }
                break;
            }
        }
    }

    for (int offset = 0, postNo = 0; offset < textLen; ++postNo) {
        SplittedItem item;

        //   各パターンでの文字を生成
        //                      textPostfixFinished
        //                      |   textPostfixContinue ※末尾の「つづく」は次回のポストがある時のみ
        //                      |   |   textPrefixFinished
        //                      |   |   |   textPrefixContinue ※先頭の「つづき」は前回のポストがある時のみ
        //                      |   |   |   |
        //                bit   8   4   2   1
        //          dec   bin : |   |   |   |
        //   flags =  0  0000 : 0   0   0   0
        //   flags =  1  0001 : 0   0   0   1
        //   flags =  2  0010 : 0   0   1   0
        //   flags =  3  0011 : 0   0   1   1
        //   flags =  4  0100 : 0   1   0   0
        //   flags =  5  0101 : 0   1   0   1
        //   flags =  6  0110 : 0   1   1   0
        //   flags =  7  0111 : 0   1   1   1
        //   flags =  8  1000 : 1   0   0   0
        //   flags =  9  1001 : 1   0   0   1
        //   flags = 10  1010 : 1   0   1   0
        //   flags = 11  1011 : 1   0   1   1
        //   flags = 12  1100 : 1   1   0   0
        //   flags = 13  1101 : 1   1   0   1
        //   flags = 14  1110 : 1   1   1   0
        //   flags = 15  1111 : 1   1   1   1

        const int textListFlagsMax = 16;

        int prefixContinueMask  = 0 == postNo ? 2 : 3; // 二つ目以降のポストの場合にのみ続きフラグをつける

        //qDebug() << "prefixContinueMask" << prefixContinueMask;
        //qDebug() << "flagsSkipMask" << flagsSkipMask;

        int flagsSplit = -1; // 確定したフラグ
        int sizeSplit  = 0; // 読み取りサイズ

        // 戦略
        //  1. prefix/postfixは優先
        //  2. できるだけ本文を多く消費するように

        // 戦略に沿って切り出し範囲を決定
        for (int flags = textListFlagsMax - 1; 0 <= flags; --flags) {

            int flagsPrefix  = flags &  3; // 本文前のテキストの追加フラグ
            int flagsPostfix = flags >> 2; // 本文後のテキストの追加フラグ

            int splitSize = m_maxTweetLength; // 最大値をセット

            if (flags && 0 != (flags & flagsSkipMask)) {
                //qDebug() << "skip#1 " << "flags(" << flags << ") & flagsSkipMask(" << flagsSkipMask << ") =" << (flags & flagsSkipMask);
                continue;
            }

            // 本文前の文字列を状態によってマスクする
            if (flagsPrefix != (flagsPrefix & prefixContinueMask)) {
                //qDebug() << "skip#2 " << "flagsPrefix(" << flagsPrefix << ") & prefixContinueMask(" << prefixContinueMask << ") =" << (flagsPrefix & prefixContinueMask);
                continue;
            }

            //qDebug() << "#3 " << flags;

            // 本文前の文字列分を消費
            splitSize -= prefixTextList[flagsPrefix].size();

            // 本文後の文字列文を消費
            splitSize -= postfixTextList[flagsPostfix].size();

            // 末尾から句読点を探す
            int trimOffset = -1;
            if (m_text.size() - offset < splitSize) {
                // 削らないと収まらない場合に限り...
                for (int pos = offset;
                     -1 != (pos = m_text.indexOf(findTerminate, pos)); ++pos) {
                    if (offset + splitSize <= pos) {
                        break;
                    }
                    trimOffset = pos + 1;
                }
            }

            if (offset <= trimOffset) {
                splitSize = trimOffset - offset;
            }

            const bool isRestEmpty = textLen <= offset + splitSize;

            if (isRestEmpty) {
                // 全部消費しきった
                if (flagsPostfix & 1) { // textPostfixContinue
                    // 続きあり、の場合は次のポストが存在しないためそれを設定できないのでスキップ
                    //qDebug() << "#4 " << flags;
                    continue;
                }
            }
            else { // 次のポストが生成できそうだ
                if (0 != (flagsPrefix  & 2) ||
                    0 != (flagsPostfix & 2)) { // textPrefixFinished / textPostfixFinished
                    // 終了、の場合はそれを設定できないのでスキップ
                    //qDebug() << "#7 " << flags;
                    continue;
                }
            }

            if (//sizeSplit < splitSize
                   //&& (flagsSplit < 0 || flags)
                    flagsSplit < 0 ||
                    (0 == flags && isRestEmpty
                         && 0 == (flagsSplit & 10) // fin
                         && 0 == (flagsSplit & 1) // pre cont
                     )
                    ) {
                flagsSplit = flags;
                sizeSplit  = splitSize;
            }
        }

        // なんかうまく構築できなかった
        // たぶん、前後の文字列が多すぎる...
        if (flagsSplit < 0) {
            r.clear();
            return r;
        }

        QString trimText = m_text.mid(offset, sizeSplit);

        item.setPrefix (prefixTextList [flagsSplit  & 3]);
        item.setPostfix(postfixTextList[flagsSplit >> 2]);
        item.setText(trimText);

        //qDebug() << "@@@@" << (flagsSplit&1?"prefixCont":"") << (flagsSplit&2?"prefixFin":"")
        //                   << (flagsSplit&4?"postfixCont":"") << (flagsSplit&8?"postfixFin":"");
        //qDebug() << "    " << "flagsSkipMask =" << flagsSkipMask << "splitLen =" << sizeSplit << "/ text =" << trimText
        //                   << "/ curLen =" << item.toString().size() << item.toString();

        offset        += sizeSplit;
        m_totalLength += sizeSplit;

        r.push_back(item);
    }

    return r;
}

int TwitterTextSplitter::size() const
{
    return m_totalLength;
}
