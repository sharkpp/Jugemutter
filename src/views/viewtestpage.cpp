#include "viewtestpage.h"
#include "ui_viewtestpage.h"
#include "twittertextsplitter.h"

ViewTestPage::ViewTestPage(QWidget *parent)
    : PageSelectorView(parent)
    , ui(new Ui::ViewTestPage)
{
    ui->setupUi(this);
}

ViewTestPage::~ViewTestPage()
{
    delete ui;
}

void ViewTestPage::on_testRun_clicked()
{
    TwitterTextSplitter splitter;

// 分割処理のテスト
#define TEST1(prefix, body, postfix, compare, maxTweetLength) \
for (int flags = 1; flags; flags--, \
  cursor.insertText( QString("\nL%1 OK [\"%2\"]").arg(__LINE__).arg((QStringList compare).join("\", \"")), formatOK ) ) { \
    QStringList comp = QStringList compare; \
    QStringList tmp1 = QStringList prefix; \
    QList<TwitterTextSplitter::TextTypeValue> list1; \
    list1.append(TwitterTextSplitter::TextTypeValue(TwitterTextSplitter::textPrefixFreeText, tmp1.size() < 0 ? "" : tmp1[0])); \
    list1.append(TwitterTextSplitter::TextTypeValue(TwitterTextSplitter::textPrefixContinue, tmp1.size() < 1 ? "" : tmp1[1])); \
    list1.append(TwitterTextSplitter::TextTypeValue(TwitterTextSplitter::textPrefixFinished, tmp1.size() < 2 ? "" : tmp1[2])); \
    splitter.setPrefix(list1); \
    QStringList tmp2 = QStringList postfix; \
    QList<TwitterTextSplitter::TextTypeValue> list2; \
    list2.append(TwitterTextSplitter::TextTypeValue(TwitterTextSplitter::textPostfixFreeText, tmp2.size() < 0 ? "" : tmp2[0])); \
    list2.append(TwitterTextSplitter::TextTypeValue(TwitterTextSplitter::textPostfixContinue, tmp2.size() < 1 ? "" : tmp2[1])); \
    list2.append(TwitterTextSplitter::TextTypeValue(TwitterTextSplitter::textPostfixFinished, tmp2.size() < 2 ? "" : tmp2[2])); \
    splitter.setPostfix(list2); \
    splitter.setText(body); \
    splitter.setMaxTweetLength(maxTweetLength); \
    QList<SplittedItem> tweetList = splitter.split(); \
    QStringList tmp3; \
    foreach (const SplittedItem& item, tweetList) { \
        tmp3.push_back(item.toString()); \
    } \
    if (tmp3 != comp) { \
        QString s; \
        cursor.insertText( QString("\nL%1 NG [\"%2\"] != [\"%3\"]").arg(__LINE__).arg(tmp3.join("\", \"")).arg(comp.join("\", \"")) , formatNG ); \
        break; \
    } \
}

    ui->output->setPlainText("");

    QTextCharFormat formatOK, formatNG;
    formatOK.setForeground (QColor(64,64,64));
    formatNG.setForeground(QColor(255,0,0));

    QTextCursor cursor = ui->output->textCursor();

    // 分割テスト
    // TEST1(({ free,cont,fin }, body, ), { free,cont,fin }, {comp}, splitNum)

    cursor.insertText( "------ 分割処理テスト ------" );

    TEST1(({"","",""}), "あいうえお", ({"","",""}), ({"あいうえお"}), 140);
    TEST1(({"","",""}), "あいうえおかきくけこ", ({"","",""}), ({"あいうえお","かきくけこ"}), 5);
    TEST1(({"#","",""}), "あいうえおかきくけこ", ({"@","",""}), ({"#あいうえお@","#かきくけこ@"}), 7);

    TEST1(({"","@",""}), "あいうえおかきくけこ", ({"","",""}), ({"あいうえ","@おかき","@くけこ"}), 4);
    TEST1(({"","@",""}), "あいうえおかきくけこ", ({"","*",""}), ({"あいうえ*","@おかき*","@くけこ"}), 5);
    TEST1(({"","","@"}), "あいうえおかきくけこ", ({"","",""}), ({"あいうえ","おかきく","@けこ"}), 4);
    TEST1(({"","",""}), "あいうえおかきくけこ", ({"","","@"}), ({"あいうえ","おかきく","けこ@"}), 4);

    TEST1(({"","@@",""}), "あいうえおかきくけこ", ({"","",""}), ({"あいうえおか","@@きくけこ"}), 6);
    TEST1(({"","@@@",""}), "あいうえおかきくけこ", ({"","",""}), ({"あいうえおか","@@@きくけ","@@@こ"}), 6);

    TEST1(({"*","@",""}), "あいうえおかきくけこ", ({"","",""}), ({"*あいうえおか","*@きくけこ"}), 7);
    TEST1(({"*","@",""}), "あいうえおかきくけこ", ({"","",""}), ({"*あいうえお","*@かきくけ","*@こ"}), 6);
    TEST1(({"*","@@",""}), "あいうえおかきくけこ", ({"","",""}), ({"*あいうえお","*@@かきく","*@@けこ"}), 6);

    TEST1(({"","",""}), "あいうえおかきくけこ", ({"","@@",""}), ({"あいうえ@@","おかきくけこ"}), 6);
    TEST1(({"","",""}), "あいうえおかきくけこ", ({"","@@@",""}), ({"あいう@@@","えおか@@@","きくけこ"}), 6);

    TEST1(({"*","",""}), "あいうえおかきくけこ", ({"","@",""}), ({"*あいうえお@","*かきくけこ"}), 7);
    TEST1(({"*","",""}), "あいうえおかきくけこ", ({"","@",""}), ({"*あいうえ@","*おかきく@","けこ"}), 6);
    TEST1(({"*","",""}), "あいうえおかきくけこ", ({"","@@",""}), ({"*あいう@@","*えおか@@","きくけこ"}), 6);

    TEST1(({"","",""}), "あいうえおかきくけこ", ({"*","@",""}), ({"あいうえお*@","かきくけこ*"}), 7);
    TEST1(({"","",""}), "あいうえおかきくけこ", ({"*","@",""}), ({"あいうえ*@","おかきく*@","けこ*"}), 6);
    TEST1(({"","",""}), "あいうえおかきくけこ", ({"*","@@",""}), ({"あいう*@@","えおか*@@","きくけこ*"}), 6);

    TEST1(({"+","",""}), "あいうえおかきくけこ", ({"*","@",""}), ({"+あいうえ*@","+おかきく*@","+けこ*"}), 7);
    TEST1(({"+","",""}), "あいうえおかきくけこ", ({"*","@",""}), ({"+あいう*@","+えおか*@","+きくけこ*"}), 6);
    TEST1(({"+","",""}), "あいうえおかきくけこ", ({"*","@@",""}), ({"+あいう*@@","えおか*@@","きくけこ*"}), 6);








}
