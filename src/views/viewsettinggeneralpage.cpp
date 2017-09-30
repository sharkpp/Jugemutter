#include "viewsettinggeneralpage.h"
#include "ui_viewsettinggeneralpage.h"
#include "preference.h"
#include <QDebug>
#include <QRegularExpression>

const QString tagPrefixPostFreeText = "textPre";
const QString tagPrefixPostContinue = "continuePre";
const QString tagPrefixPostFinished = "finishedPre";
const QString tagPostfixPostFreeText = "textPost";
const QString tagPostfixPostContinue = "continuePost";
const QString tagPostfixPostFinished = "finishedPost";

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

    const QString text = tr("Jugemu Jugemu Gokō-no surikire Kaijarisuigyo-no Suigyōmatsu Unraimatsu Fūraimatsu Kuunerutokoro-ni Sumutokoro Yaburakōji-no burakōji Paipopaipo Paipo-no-shūringan Shūringan-no Gūrindai Gūrindai-no Ponpokopī-no Ponpokonā-no Chōkyūmei-no Chōsuke");

    ui->postBody1->setText(text.left(3).trimmed());

    QFontMetrics fm = ui->postBody2->fontMetrics();
    ui->postBody2->setMaximumHeight(fm.height());

    ui->postBody2->setText(text.mid(3).trimmed());
    //ui->postBody2->setText("The Life and Strange Surprizing Adventures of Robinson Crusoe, of York, Mariner: Who lived Eight and Twenty Years, all alone in an un‐inhabited Island on the Coast of America, near the Mouth of the Great River of Oroonoque; Having been cast on Shore by Shipwreck, wherein all the Men perished but himself. With An Account how he was at last as strangely deliver’d by Pyrates");

    ui->prefix->setContentsMargins(3, 3, 3, 3);
    ui->prefix->addTagList(ui->postFreeTextLabel->text(), tagPrefixPostFreeText);
    ui->prefix->addTagList(ui->postContinueLabel->text(), tagPrefixPostContinue);
    ui->prefix->addTagList(ui->postFinishedLabel->text(), tagPrefixPostFinished);
    ui->postfix->setContentsMargins(3, 3, 3, 3);
    ui->postfix->addTagList(ui->postFreeTextLabel->text(), tagPostfixPostFreeText);
    ui->postfix->addTagList(ui->postContinueLabel->text(), tagPostfixPostContinue);
    ui->postfix->addTagList(ui->postFinishedLabel->text(), tagPostfixPostFinished);

    connect(ui->prefix,  &TagInput::updateTags, this, &ViewSettingGeneralPage::onPrefixUpdateTags);
    connect(ui->postfix, &TagInput::updateTags, this, &ViewSettingGeneralPage::onPostfixUpdateTags);

    resizeEvent(nullptr);
}

ViewSettingGeneralPage::~ViewSettingGeneralPage()
{
    delete ui;
}

QString ViewSettingGeneralPage::tagNameFromPostTextType(Preference::PostTextType type)
{
    const QString tmp = "";
    switch (type) {
    case Preference::postTextPrefixFreeText:    return ui->postFreeTextLabel->text();
    case Preference::postTextPrefixContinue:    return ui->postContinueLabel->text();
    case Preference::postTextPrefixFinished:    return ui->postFinishedLabel->text();
    case Preference::postTextPostfixFreeText:       return ui->postFreeTextLabel->text();
    case Preference::postTextPostfixContinue:   return ui->postContinueLabel->text();
    case Preference::postTextPostfixFinished:   return ui->postFinishedLabel->text();
    default:
        return tmp;
    }
}

const QString& ViewSettingGeneralPage::tagFromPostTextType(Preference::PostTextType type)
{
    const QString tmp = "";
    switch (type) {
    case Preference::postTextPrefixFreeText:    return tagPrefixPostFreeText;
    case Preference::postTextPrefixContinue:    return tagPrefixPostContinue;
    case Preference::postTextPrefixFinished:    return tagPrefixPostFinished;
    case Preference::postTextPostfixFreeText:       return tagPostfixPostFreeText;
    case Preference::postTextPostfixContinue:   return tagPostfixPostContinue;
    case Preference::postTextPostfixFinished:   return tagPostfixPostFinished;
    default:
        return tmp;
    }
}

Preference::PostTextType ViewSettingGeneralPage::tagToPostTextType(const QString &type)
{
    if (tagPrefixPostFreeText == type) {
        return Preference::postTextPrefixFreeText;
    }
    else if (tagPrefixPostContinue == type) {
        return Preference::postTextPrefixContinue;
    }
    else if (tagPrefixPostFinished == type) {
        return Preference::postTextPrefixFinished;
    }
    else if (tagPostfixPostFreeText == type) {
        return Preference::postTextPostfixFreeText;
    }
    else if (tagPostfixPostContinue == type) {
        return Preference::postTextPostfixContinue;
    }
    else if (tagPostfixPostFinished == type) {
        return Preference::postTextPostfixFinished;
    }
    return (Preference::PostTextType)-1;
}

void ViewSettingGeneralPage::setPreference(Preference *preference)
{
    m_preference = preference;

    connect(m_preference, &Preference::update,
            this, &ViewSettingGeneralPage::onPreferenceUpdate);

//    ui->prefix->blockSignals(true);
//    ui->postfix->blockSignals(true);
    onPreferenceUpdate();
//    ui->prefix->blockSignals(false);
//    ui->postfix->blockSignals(false);
}

void ViewSettingGeneralPage::resizeEvent(QResizeEvent *event)
{
    if (event) {
        PageSelectorView::resizeEvent(event);
    }

    //qDebug() << "--------------------------------------------------------";
    //qDebug() << QLabel_getLineText(ui->postBody2, 0);

    const QString text = tr("Jugemu Jugemu Gokō-no surikire Kaijarisuigyo-no Suigyōmatsu Unraimatsu Fūraimatsu Kuunerutokoro-ni Sumutokoro Yaburakōji-no burakōji Paipopaipo Paipo-no-shūringan Shūringan-no Gūrindai Gūrindai-no Ponpokopī-no Ponpokonā-no Chōkyūmei-no Chōsuke");

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

    QList<TagItem *> items;

    foreach (auto ite, m_preference->postPrefixText()) {
        items.append(new TagItem(tagNameFromPostTextType(ite),
                                 tagFromPostTextType(ite),
                                 this));
    }
    ui->prefix->addTags(items);
    items.clear();

    foreach (auto ite, m_preference->postPostfixText()) {
        items.append(new TagItem(tagNameFromPostTextType(ite),
                                 tagFromPostTextType(ite),
                                 this));
    }
    ui->postfix->addTags(items);
    items.clear();
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

void ViewSettingGeneralPage::onPrefixUpdateTags()
{
    Preference::PostTextTypeList tags;
    foreach (auto tag, ui->prefix->tags()) {
        tags.append(tagToPostTextType(tag->id()));
    }
    m_preference->setPostPrefixText(tags);
}

void ViewSettingGeneralPage::onPostfixUpdateTags()
{
    Preference::PostTextTypeList tags;
    foreach (auto tag, ui->postfix->tags()) {
        tags.append(tagToPostTextType(tag->id()));
    }
    m_preference->setPostPostfixText(tags);
}
