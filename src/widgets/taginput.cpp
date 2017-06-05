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
    setMaximumHeight(fm.height() + 6 * 2 + 3 * 2);
    setContentsMargins(6, 6, 6, 6);
    setContentsMargins(0,0,0,0);

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
    append("test1");
    append("hoge2");
    append("test3");
    append("hoge4");
    append("test5");
    append("hoge6");
    append("test7");
    append("hoge8");

}

void TagInput::append(const QString &tag)
{
    QLabel *w = new QLabel(this);
    QFontMetrics fm = w->fontMetrics();
    w->setStyleSheet("QFrame { border-radius: 5px; border: none; background: #19BC9C; color: #FFF; padding: 3px; }");
    w->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    w->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    w->setMaximumHeight(fm.height() + 3 * 2);
    w->setText(tag);
    m_layout->insertWidget(m_layout->count() - 1, w);
}

void TagInput::remove(const QString &tag)
{

}

void TagInput::updateTags()
{
}
