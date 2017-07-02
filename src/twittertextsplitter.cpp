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

int SplittedItem::size() const
{
    return (m_prefix.isEmpty() ? 0 : m_prefix.size() + 1)
        +  m_text.size()
        +  (m_postfix.isEmpty() ? 0 : 1 + m_postfix.size());

}

////////////////

TwitterTextSplitter::TwitterTextSplitter()
    : m_totalLength(-1)
{

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

    qDebug() << "textLen" << textLen;

/*

+ = prefix
@ = body
* = postfix

<-----------------><----------------->
@@@@@@@@@@@@@@@@@@
++++@@@@@@@@@@@@@@@++++@@@


*/

    // 本文前の文字列を生成
    //   各パターンでの文字を生成
    //          dec bin :         2                     1
    //   flags =  0  00 :                 (none)
    //   flags =  1  01 :                    | textPrefixContinue
    //   flags =  2  10 : textPrefixFinished
    //   flags =  3  11 : textPrefixFinished | textPrefixContinue
    QString prefixTextList[4];
    for (int flags = 0; flags < 4; ++flags) {
        QString &s = prefixTextList[flags];
        foreach (auto typePair, m_prefix) {
            switch (typePair.first) {
            default: break;
            case textPrefixFreeText:
                s += typePair.second;
                break;
            case textPrefixContinue:
                if (flags & 1) {
                    s += typePair.second;
                }
                break;
            case textPrefixFinished:
                if (flags & 2) {
                    s += typePair.second;
                }
                break;
            }
        }
    }

    // 本文後の文字列を生成
    //   各パターンでの文字を生成
    //          dec bin :          2                      1
    //   flags =  0  00 :                  (none)
    //   flags =  1  01 :                       textPostfixContinue
    //   flags =  2  10 : textPostfixFinished
    //   flags =  3  11 : textPostfixFinished | textPostfixContinue
    QString postfixTextList[4];
    for (int flags = 0; flags < 4; ++flags) {
        QString &s = postfixTextList[flags];
        foreach (auto typePair, m_postfix) {
            switch (typePair.first) {
            default: break;
            case textPostfixFreeText:
                s += typePair.second;
                break;
            case textPostfixContinue:
                if (flags & 1) {
                    s += typePair.second;
                }
                break;
            case textPostfixFinished:
                if (flags & 2) {
                    s += typePair.second;
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

        int prefixContinueMask  = 0 == postNo ? ~0 : ~1;

        int flagsSplit = -1; // 確定したフラグ
        int sizeSplit  = 0; // 読み取りサイズ

        // 戦略
        //  1. できるだけ本文を多く消費するように
        //  2. prefix/postfixは優先

        // 戦略に沿って切り出し範囲を決定
        for (int flags = 0; flags < 15 + 1; ++flags) {

            int flagsPrefix  = flags &  3;
            int flagsPostfix = flags >> 2;

            int splitSize = 140; // 最大値をセット

            // 本文前の文字列を状態によってマスクする
            if (flagsPrefix != (flagsPrefix & prefixContinueMask)) {
                continue;
            }

            // 本文前の文字列分を消費
            splitSize -= prefixTextList[flagsPrefix].size();

            // 本文後の文字列文を消費
            splitSize -= postfixTextList[flagsPostfix].size();

            // テスト：末尾から句読点を探す
            int trimOffset = -1;
            for (int pos = offset;
                 -1 != (pos = m_text.indexOf(findTerminate, pos)); ++pos) {
                if (offset + splitSize <= pos) {
                    break;
                }
                trimOffset = pos + 1;
            }

            if (offset <= trimOffset) {
                splitSize = trimOffset - offset;
            }

            if (offset + splitSize <= textLen) {
                // 全部消費しきった
                if (!(flagsPostfix & 1)) { // textPostfixContinue
                    // 続きあり、の場合はそれを設定できないのでスキップ
                    continue;
                }
            }

            if (sizeSplit < splitSize) {
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

        qDebug() << "@@@@@@@@@@@@@@@@@@@@";
        qDebug() << "@@@@" << (flagsSplit&3) << (flagsSplit>>2) << sizeSplit << item.toString().size();
        qDebug() << "@@@@" << trimText;
        qDebug() << "@@@@" << item.toString();

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
