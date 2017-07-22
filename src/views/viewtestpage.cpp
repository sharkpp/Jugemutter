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


#define TEST1(prefix, body, postfix, compare, maxTweetLength) \
for (int flags = 1; flags; flags--, \
  ui->output->setPlainText( ui->output->toPlainText() + \
        QString("\nL%1 OK [\"%2\"]").arg(__LINE__).arg((QStringList compare).join("\", \"")) ) ) { \
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
        ui->output->setPlainText( ui->output->toPlainText() + QString("\nL%1 NG [\"%2\"] != [\"%3\"]").arg(__LINE__).arg(tmp3.join("\", \"")).arg(comp.join("\", \"")) ); \
        break; \
    } \
}

    ui->output->setPlainText("----------------\n"); \

    TEST1(({"","",""}), "あいうえお", ({"","",""}), ({"あいうえお"}), 140);
    TEST1(({"","",""}), "あいうえおかきくけこ", ({"","",""}), ({"あいうえお","かきくけこ"}), 5);
    TEST1(({"#","",""}), "あいうえおかきくけこ", ({"@","",""}), ({"#あいうえお@","#かきくけこ@"}), 7);
    TEST1(({"","@",""}), "あいうえおかきくけこ", ({"","",""}), ({"@あいう","@えおか","きくけこ"}), 4);
    TEST1(({"","@",""}), "あいうえおかきくけこ", ({"","*",""}), ({"@あいう*","@えおか*","きくけこ"}), 5);
    TEST1(({"","","@"}), "あいうえおかきくけこ", ({"","",""}), ({"あいうえ","おかきく","@けこ"}), 4);
    TEST1(({"","",""}), "あいうえおかきくけこ", ({"","","@"}), ({"あいうえ","おかきく","けこ@"}), 4);
















}
