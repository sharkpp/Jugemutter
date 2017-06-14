#include "viewsettinggeneralpage.h"
#include "ui_viewsettinggeneralpage.h"
#include "preference.h"
#include <QDebug>
#include <QRegularExpression>

const QRegularExpression matchNonAscii = QRegularExpression("[^\x21-\x7E]");
QString QLabel_getLineText(QLabel* label, int lineNo = 0) {
    QFontMetrics fm   = label->fontMetrics();
    QString      text = label->text();
    int          width= label->width();
    QString    curText;
//qDebug() << "----------------------------------";
//qDebug() << text << width;
    for (int curLineNo = 0, lineOffset = 0;
         curLineNo <= lineNo; ++curLineNo) {
        int searchMin = lineOffset;
        int searchMax = text.length();
        int searchCur = searchMin + (searchMax - searchMin) / 2;
//qDebug() << curLineNo << "init" << searchMin << searchCur << searchMax;
        for (; searchMin < searchMax; ) {
            curText = QString(text).mid(lineOffset, searchCur - lineOffset);
            int curWidth    = fm.width(curText);
//qDebug() << curLineNo << ">>" << lineOffset << searchMin << searchCur << searchMax << curWidth << curText;
            if (width < curWidth) {
                searchMax = searchCur - 1;
                searchCur = searchMin + (searchMax - searchMin) / 2;
            }
            else if (curWidth < width) {
                searchMin = searchCur + 1;
                searchCur = searchMin + (searchMax - searchMin) / 2;
            }
            else {
                break;
            }
        }
        // fix word wrap
        int curPos = lineOffset + curText.length();
        int nextSpace = text.indexOf(matchNonAscii, curPos);
        int prevSpace = text.lastIndexOf(matchNonAscii, curPos);
        if (curPos != nextSpace && matchNonAscii.match(text.at(curPos)).hasMatch() && 0 <= prevSpace) {
            curText = curText.mid(0, curText.length() - (curPos - prevSpace));
        }
        curText = curText.trimmed();
        //
        lineOffset += curText.length();
    }
    return curText;
}

ViewSettingGeneralPage::ViewSettingGeneralPage(QWidget *parent)
    : PageSelectorView(parent)
    , ui(new Ui::ViewSettingGeneralPage)
{
    ui->setupUi(this);

    //                     11111111112
    //           012345678901234567890
    QString x = "abcd efgh ijkl nmop";
    QString y = " ";
    qDebug() << x.lastIndexOf(y);           // returns 14
    qDebug() << x.lastIndexOf(y, 7);        // returns 4
    qDebug() << x.lastIndexOf(y, 11);       // returns 9

    const QString text = "寿限無 寿限無 五劫の擦り切れ 海砂利水魚の水行末 雲来末 風来末 食う寝る処に住む処 藪ら柑子の藪柑子 パイポ パイポ パイポのシューリンガン シューリンガンのグーリンダイ グーリンダイのポンポコピーのポンポコナーの長久命の長助";

    ui->postBody1->setText(text.left(3).trimmed());

    QFontMetrics fm = ui->postBody2->fontMetrics();
    ui->postBody2->setMaximumHeight(fm.height());

    ui->postBody2->setText(text.mid(3).trimmed());
    //ui->postBody2->setText("The Life and Strange Surprizing Adventures of Robinson Crusoe, of York, Mariner: Who lived Eight and Twenty Years, all alone in an un‐inhabited Island on the Coast of America, near the Mouth of the Great River of Oroonoque; Having been cast on Shore by Shipwreck, wherein all the Men perished but himself. With An Account how he was at last as strangely deliver’d by Pyrates");

    ui->prefix->setContentsMargins(3, 3, 3, 3);
    ui->prefix->appendList(ui->postFreeTextLabel->text(), "text");
    ui->prefix->appendList(ui->postContinueLabel->text(), "continue");
    ui->prefix->appendList(ui->postFinishedLabel->text(), "finish");
    ui->postfix->setContentsMargins(3, 3, 3, 3);
    ui->postfix->appendList(ui->postFreeTextLabel->text(), "text");
    ui->postfix->appendList(ui->postContinueLabel->text(), "continue");
    ui->postfix->appendList(ui->postFinishedLabel->text(), "finish");

    resizeEvent(nullptr);
}

ViewSettingGeneralPage::~ViewSettingGeneralPage()
{
    delete ui;
}

void ViewSettingGeneralPage::setPreference(Preference *preference)
{
    m_preference = preference;

    connect(m_preference, &Preference::update,
            this, &ViewSettingGeneralPage::onPreferenceUpdate);

    onPreferenceUpdate();
}

void ViewSettingGeneralPage::resizeEvent(QResizeEvent *event)
{
    if (event) {
        PageSelectorView::resizeEvent(event);
    }

    qDebug() << "--------------------------------------------------------";
    qDebug() << QLabel_getLineText(ui->postBody2, 0);

    const QString text = "寿限無 寿限無 五劫の擦り切れ 海砂利水魚の水行末 雲来末 風来末 食う寝る処に住む処 藪ら柑子の藪柑子 パイポ パイポ パイポのシューリンガン シューリンガンのグーリンダイ グーリンダイのポンポコピーのポンポコナーの長久命の長助";

    QString body2 = QLabel_getLineText(ui->postBody2, 0);

    ui->postBody3->setText(text.mid(text.indexOf(body2) + body2.length(), 3));
}

void ViewSettingGeneralPage::onPreferenceUpdate()
{
    ui->postPrefixFreeText->setText(m_preference->postPrefixFreeText());
    ui->postPrefixContinueText->setText(m_preference->postPrefixContinueText());
    ui->postPrefixFinishedText->setText(m_preference->postPrefixFinishedText());
    ui->postPostfixFreeText->setText(m_preference->postPostfixFreeText());
    ui->postPostfixContinueText->setText(m_preference->postPostfixContinueText());
    ui->postPostfixFinishedText->setText(m_preference->postPostfixFinishedText());
}

void ViewSettingGeneralPage::on_postPrefixFreeText_editingFinished()
{
    m_preference->setPostPrefixFreeText(ui->postPrefixFreeText->text());
}

void ViewSettingGeneralPage::on_postPrefixContinueText_editingFinished()
{
    m_preference->setPostPrefixContinueText(ui->postPrefixContinueText->text());
}

void ViewSettingGeneralPage::on_postPrefixFinishedText_editingFinished()
{
    m_preference->setPostPrefixFinishedText(ui->postPrefixFinishedText->text());
}

void ViewSettingGeneralPage::on_postPostfixFreeText_editingFinished()
{
    m_preference->setPostPostfixFreeText(ui->postPostfixFreeText->text());
}

void ViewSettingGeneralPage::on_postPostfixContinueText_editingFinished()
{
    m_preference->setPostPostfixContinueText(ui->postPostfixContinueText->text());
}

void ViewSettingGeneralPage::on_postPostfixFinishedText_editingFinished()
{
    m_preference->setPostPostfixFinishedText(ui->postPostfixFinishedText->text());
}

