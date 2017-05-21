#include "textcounterlabel.h"
#include <QPlainTextEdit>
#include <QPalette>

TextCounterLabel::TextCounterLabel(QWidget *parent)
    : QLabel(parent)
    , warningThreshold(140)
{
    normalColor  = palette().color(foregroundRole());
    warningColor = Qt::red;
}

void TextCounterLabel::showEvent(QShowEvent *event)
{
    QPlainTextEdit *plainTextEdit = qobject_cast<QPlainTextEdit*>( buddy() );

    if (plainTextEdit) {
        connect(plainTextEdit, &QPlainTextEdit::textChanged,
                this, &TextCounterLabel::on_buddy_textChanged);
    }

}

void TextCounterLabel::on_buddy_textChanged()
{
    QPlainTextEdit *plainTextEdit = qobject_cast<QPlainTextEdit*>( sender() );

    if (!plainTextEdit) {
        return;
    }

    const int textLen = plainTextEdit->toPlainText().length();

    setText(QString("%1").arg(textLen));

    QPalette pal = palette();
    pal.setColor(foregroundRole(), textLen <= warningThreshold ? normalColor : warningColor);
    setPalette(pal);
}
