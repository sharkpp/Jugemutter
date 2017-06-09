#include "taginput.h"
#include <QDebug>
#include <QKeyEvent>
#include <QTextDocument>
#include <QTextBlock>
#include <QTextDocumentFragment>
#include <QHBoxLayout>
#include <QFrame>
#include <QLabel>
#include <QPushButton>
#include <QScrollArea>

TagInput::TagInput(QWidget *parent)
    : QScrollArea(parent)
{
    //connect(this, &QTextEdit::textChanged, this, &TagInput::updateTags);

    QFontMetrics fm = fontMetrics();
    setSizePolicy(sizePolicy().horizontalPolicy(), QSizePolicy::Fixed);
    setMaximumHeight(fm.height() + 6 * 2 + 4 * 2);
    setContentsMargins(6, 6, 6, 6);

    /*
    setAutoFillBackground(true);
    QPalette pal = palette();
    pal.setColor( backgroundRole(), pal.color(QPalette::Base) );
    //pal.setColor( backgroundRole(), QColor(255,0,0) );
    setPalette( pal );
    setAutoFillBackground( true );
*/
    //setLayout(m_layout = new QHBoxLayout(this));

    setBackgroundRole(QPalette::Base);
    setFrameShadow(QFrame::Plain);
    setFrameShape(QFrame::StyledPanel);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setWidgetResizable(true);

    //vertical box that contains all the checkboxes for the filters
    QWidget* base = new QWidget(this);
    base->setObjectName("techarea");
    base->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Maximum);
    base->setLayout(m_layout = new QHBoxLayout(base));
    setWidget(base);

    m_layout->setSpacing(6);
    m_layout->setContentsMargins(6, 6, 6, 6);

    QWidget* spacer = new QWidget();
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum);
    m_layout->addWidget(spacer);

    append("test");
    append("hoge");
    append("a");
    append("ほげふが");
    append("ふふぉjのの");
    append("っっz");
    append("test5");
    append("hoge6");
    append("test7");
    append("hoge8");

}

void TagInput::append(const QString &tag)
{
    QFrame *tagItem = new QFrame(this);
    tagItem->setObjectName(QStringLiteral("tag:%1").arg(tag));
    tagItem->setStyleSheet(QStringLiteral(
            "QFrame { border-radius: 5px; padding: 3px; border: none;"
                     "background: #19BC9C; color: #FFF; }"
        ));
    tagItem->setFrameShape(QFrame::StyledPanel);
    tagItem->setFrameShadow(QFrame::Raised);

    QLabel *tagLabel = new QLabel(tagItem);
    tagLabel->setObjectName(QStringLiteral("label"));
    QFontMetrics tagLabelFm = tagLabel->fontMetrics();
    QSizePolicy sizePolicyLabel(QSizePolicy::Preferred, QSizePolicy::Fixed);
    sizePolicyLabel.setHorizontalStretch(0);
    sizePolicyLabel.setVerticalStretch(0);
    tagLabel->setMaximumHeight(tagLabelFm.height());
    tagLabel->setSizePolicy(sizePolicyLabel);
    tagLabel->setContentsMargins(0, 0, 0, 0);
    tagLabel->setText(tag);
    tagLabel->setStyleSheet(QStringLiteral("QFrame { border-radius: 0; border: none; background: #FFEEEE; color: #000; padding: 0; height: 1em; line-height: 1em; }"));
    tagLabel->setStyleSheet(QStringLiteral("QFrame { border-radius: 0; border: none; padding: 0; height: 1em; line-height: 1em; }"));

    QPushButton *tagRemoveButton = new QPushButton(tagItem);
    tagRemoveButton->setObjectName(QStringLiteral("removeButton"));
    QFontMetrics tagRemoveButtonFm = tagRemoveButton->fontMetrics();
    QSizePolicy sizePolicyRemoveButton(QSizePolicy::Fixed, QSizePolicy::Fixed);
    sizePolicyRemoveButton.setHorizontalStretch(0);
    sizePolicyRemoveButton.setVerticalStretch(0);
    //sizePolicy5.setHeightForWidth(tagRemove->sizePolicy().hasHeightForWidth());
    QFont fontRemoveButton = tagRemoveButton->font();
    fontRemoveButton.setBold(true);
    fontRemoveButton.setWeight(75);
    tagRemoveButton->setFont(fontRemoveButton);
    tagRemoveButton->setText("x");
    tagRemoveButton->setSizePolicy(sizePolicyRemoveButton);
    tagRemoveButton->setMouseTracking(true);
    tagRemoveButton->setFocusPolicy(Qt::NoFocus);
    tagRemoveButton->setContentsMargins(0, 0, 0, 0);
    tagRemoveButton->setMaximumWidth(tagRemoveButtonFm.width(tagRemoveButton->text()) + 6);
    tagRemoveButton->setMaximumHeight(tagRemoveButtonFm.height());
    tagRemoveButton->setStyleSheet(QStringLiteral(
           "QPushButton { margin: 0; padding: 0; border: none; color: #FFF; }"
           "QPushButton:pressed { border: none; background: #19BC9C; color: #F66;}"
        ));

    QRect rc = tagRemoveButtonFm.boundingRect("x");
    qDebug() << "tagRemoveFm.w" << tagRemoveButtonFm.width("x") << tagRemoveButtonFm.height() << rc;

    QHBoxLayout *tagItemLayout = new QHBoxLayout(tagItem);
    tagItemLayout->setObjectName(QStringLiteral("layout"));
    tagItemLayout->setSpacing(3);
    tagItemLayout->setMargin(0);
    tagItemLayout->setContentsMargins(3, 0, 3, 0);
    tagItemLayout->addWidget(tagLabel);
    tagItemLayout->addWidget(tagRemoveButton);

    m_layout->insertWidget(m_layout->count() - 1, tagItem);

    connect(tagRemoveButton, &QAbstractButton::clicked, this, &TagInput::onTagRemoveClick);

#if 0
    QLabel *w = new QLabel(this);
    QFontMetrics fm = w->fontMetrics();
    w->setStyleSheet("QFrame { border-radius: 5px; border: none; background: #19BC9C; color: #FFF; padding: 3px; }");
    w->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    w->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    w->setMaximumHeight(fm.height() + 3 * 2);
    w->setText(tag);
    //connect(w, &QTextEdit::textChanged, this, &TagInput::onTagClick);
    m_layout->insertWidget(m_layout->count() - 1, w);
#endif
}

void TagInput::remove(const QString &tag)
{

}

void TagInput::updateTags()
{
}

void TagInput::onTagRemoveClick(bool checked)
{
    QPushButton *button = qobject_cast<QPushButton*>( sender() );
    QLabel *label = button->parentWidget()->findChild<QLabel*>();

    if (label) {
        qDebug() << label->text();
    }
}

void TagInput::onTagClick()
{

}
