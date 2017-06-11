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
#include <QScrollBar>
#include <QComboBox>

const int defaultMargins = 6;

//---------------------------------------------------------
// TagItem
//---------------------------------------------------------

TagItem::TagItem(QObject *parent)
    : QObject(parent)
    , m_unique(true)
{
}

TagItem::TagItem(const QString &tagName, QObject *parent)
    : QObject(parent)
    , m_tagName(tagName)
    , m_unique(true)
{
}

TagItem::TagItem(const QString &tagName, const QString &tagId, QObject *parent)
    : QObject(parent)
    , m_tagName(tagName)
    , m_id(tagId)
    , m_unique(true)
{
}

TagItem *TagItem::clone()
{
    TagItem *item = new TagItem(parent());
    item->setTagName(m_tagName);
    item->setId(m_id);
    item->setDescriptionText(m_descriptionText);
    return item;
}

QString TagItem::tagName() const
{
    return m_tagName;
}

void TagItem::setTagName(const QString &tagName)
{
    m_tagName = tagName;
}

QString TagItem::id() const
{
    return !m_id.isEmpty() ? m_id : m_tagName;
}

void TagItem::setId(const QString &id)
{
    m_id = id;
}

QString TagItem::descriptionText() const
{
    return !m_descriptionText.isEmpty() ? m_descriptionText : m_tagName;
}

void TagItem::setDescriptionText(const QString &descriptionText)
{
    m_descriptionText = descriptionText;
}

bool TagItem::unique() const
{
    return m_unique;
}

void TagItem::setUnique(bool unique)
{
    m_unique = unique;
}

//---------------------------------------------------------
// TagInput
//---------------------------------------------------------

TagInput::TagInput(QWidget *parent)
    : QScrollArea(parent)
    , m_dragStartOffset(0)
{
    initWidget();

#if 0
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
#endif
}

void TagInput::initWidget()
{
    //connect(this, &QTextEdit::textChanged, this, &TagInput::updateTags);

    QFontMetrics fm = fontMetrics();
    setSizePolicy(sizePolicy().horizontalPolicy(), QSizePolicy::Fixed);
    QMargins margin(contentsMargins().left() ? contentsMargins().left() : defaultMargins,
                    contentsMargins().top() ? contentsMargins().top() : defaultMargins,
                    contentsMargins().right() ? contentsMargins().right() : defaultMargins,
                    contentsMargins().bottom() ? contentsMargins().bottom() : defaultMargins
                );
    setContentsMargins(0, 0, 0, 0);

    setMaximumHeight(fm.height() + margin.top() + margin.bottom() + 4 * 2);

    setBackgroundRole(QPalette::Base);
    setFrameShadow(QFrame::Plain);
    setFrameShape(QFrame::StyledPanel);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setWidgetResizable(true);

    QWidget* base = new QWidget(this);
    base->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Maximum);
    base->setLayout(m_layout = new QHBoxLayout(base));
    setWidget(base);

    m_layout->setSpacing(defaultMargins);
    m_layout->setContentsMargins(margin.left() ? margin.left() : defaultMargins,
                                 margin.top() ? margin.top() : defaultMargins,
                                 margin.right() ? margin.right() : defaultMargins,
                                 margin.bottom() ? margin.bottom() : defaultMargins
                              );

    // list and editor
    m_tagListWidget = new QComboBox(this);
    //m_tagListWidget->setEditable(true);
    m_tagListWidget->setFrame(false);
    m_layout->addWidget(m_tagListWidget);
    connect(m_tagListWidget, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &TagInput::onTagListSelected);

    // spacer
    QWidget* spacer = new QWidget();
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum);
    m_layout->addWidget(spacer);
}

QFrame *TagInput::createTagItem(TagItem *item)
{
    // add widget
    QFrame *tagItem = new QFrame(this);
    tagItem->setObjectName(QStringLiteral("tag:%1").arg(item->id()));
    tagItem->setProperty("tagItem", QVariant::fromValue(static_cast<void *>( item )));
    tagItem->setStyleSheet(QStringLiteral(
            "QFrame { border-radius: 5px; padding: 2px; border: none;"
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
    tagLabel->setText(item->tagName());
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
    tagRemoveButton->setAttribute(Qt::WA_TransparentForMouseEvents, false);

    QRect rc = tagRemoveButtonFm.boundingRect("x");
    //qDebug() << "tagRemoveFm.w" << tagRemoveButtonFm.width("x") << tagRemoveButtonFm.height() << rc;

    QHBoxLayout *tagItemLayout = new QHBoxLayout(tagItem);
    tagItemLayout->setObjectName(QStringLiteral("layout"));
    tagItemLayout->setSpacing(3);
    tagItemLayout->setMargin(0);
    tagItemLayout->setContentsMargins(3, 0, 3, 0);
    tagItemLayout->addWidget(tagLabel);
    tagItemLayout->addWidget(tagRemoveButton);

    connect(tagRemoveButton, &QAbstractButton::clicked, this, &TagInput::onTagRemoveClick);

    return tagItem;
}

void TagInput::append(const QString &tag, const QString &tagId)
{
    append(new TagItem(tag, tagId, this));
}

void TagInput::append(TagItem *item)
{
    // check unique tag
    foreach (TagItem* ite, m_tags) {
        if (ite->id() == item->id() &&
            (ite->unique() || item->unique())) {
            return;
        }
    }

    // add tag list
    m_tags.append(item);

    // add tag widget
    QFrame *tagItem = createTagItem(item);
    m_layout->insertWidget(m_layout->count() - 2, tagItem);
}

void TagInput::removeById(const QString &tagId)
{
    int tabOrder = -1;

    int index = 0;
    foreach (TagItem* ite, m_tags) {
        if (tagId == ite->id()) {
            m_tags.removeOne(ite);
            tabOrder = index;
            break;
        }
        ++index;
    }

    if (tabOrder < 0) {
        return;
    }

    if (QLayoutItem *item = m_layout->itemAt(tabOrder)) {
        QWidget *w = item->widget();
        m_layout->removeItem(item);
        delete w;
    }
}

void TagInput::appendList(const QString &tag, const QString &tagId)
{
    appendList(new TagItem(tag, tagId, this));
}

void TagInput::appendList(TagItem *item)
{
    // check unique tag
    foreach (TagItem* ite, m_tagList) {
        if (ite->id() == item->id()) {
            return;
        }
    }

    m_tagList.append(item);

    m_tagListWidget->blockSignals(true);
    m_tagListWidget->addItem(item->tagName(), item->id());
    m_tagListWidget->setCurrentIndex(-1);
    m_tagListWidget->blockSignals(false);
}

void TagInput::removeListById(const QString &tagId)
{
    foreach (TagItem* ite, m_tagList) {
        if (tagId == ite->id()) {
            m_tagList.removeOne(ite);
            break;
        }
    }

    int index = m_tagListWidget->findData(tagId);
    if (0 <= index) {
        m_tagListWidget->removeItem(index);
    }
}

void TagInput::mousePressEvent(QMouseEvent *event)
{
    m_dragStartPos = event->pos();
    m_dragStartOffset = horizontalScrollBar()->value();
    setCursor(Qt::SizeHorCursor);
//    qDebug() << "mousePressEvent" << m_dragStartPos;
}

void TagInput::mouseMoveEvent(QMouseEvent *event)
{
    if (m_dragStartOffset < 0) {
        return;
    }

    QPoint topLeft = viewport()->rect().topLeft();

    int offset = m_dragStartOffset - (event->pos().x() - m_dragStartPos.x());
    if (offset < 0) { offset = 0; }
    if (horizontalScrollBar()->maximum() <= offset) { offset = horizontalScrollBar()->maximum(); }

    horizontalScrollBar()->setValue(offset);
    widget()->move(topLeft.x() - offset, topLeft.y());

//    qDebug() << "mouseMoveEvent" << m_dragStartPos << event->pos() << (m_dragStartPos - event->pos());
}

void TagInput::mouseReleaseEvent(QMouseEvent *event)
{
    if (m_dragStartOffset < 0) {
        return;
    }

    QPoint topLeft = viewport()->rect().topLeft();

    int offset = m_dragStartOffset - (event->pos().x() - m_dragStartPos.x());
    if (offset < 0) { offset = 0; }
    if (horizontalScrollBar()->maximum() <= offset) { offset = horizontalScrollBar()->maximum(); }

    horizontalScrollBar()->setValue(offset);
    widget()->move(topLeft.x() - offset, topLeft.y());

    unsetCursor();

    m_dragStartOffset = -1;
//    qDebug() << "mouseReleaseEvent" << m_dragStartPos << event->pos() << (m_dragStartPos - event->pos());

}

void TagInput::onTagRemoveClick(bool checked)
{
    if (QPushButton *button = qobject_cast<QPushButton*>( sender() )) {
        if (QLabel *label = button->parentWidget()->findChild<QLabel*>()) {
            if (QFrame *tagItem = qobject_cast<QFrame*>(label->parentWidget())) {
                if (TagItem *item = static_cast<TagItem *>( tagItem->property("tagItem").value<void *>() )) {
                    qDebug() << label->text();
                    removeById(item->id());
                }
            }
        }
    }

#if 0
    int tagOrder = -1;
    for (int index = 0; index < m_layout->count(); ++index) {
        if (QLayoutItem *item = m_layout->itemAt(index)) {
            if (QFrame *tagItem_ = qobject_cast<QFrame*>(item->widget())) {
                if (tagItem_ == tagItem) {
                    tabOrder = index;
                    break;
                }
            }
        }
    }
#endif
}

void TagInput::onTagClick()
{

}

void TagInput::onTagListSelected(int index)
{
    if (index < 0 ||
        m_tagList.size() <= index) {
        return;
    }

    TagItem *item = m_tagList.at(index);
    TagItem *itemClone = item->clone();

    append(itemClone);

    m_tagListWidget->setCurrentIndex(-1);
}
