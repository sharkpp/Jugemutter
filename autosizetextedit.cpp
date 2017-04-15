#include "autosizetextedit.h"
#include <QDebug>

AutoSizeTextEdit::AutoSizeTextEdit(QWidget *parent)
    : QPlainTextEdit(parent)
{
    connect(this, &QPlainTextEdit::textChanged, this, &AutoSizeTextEdit::on_textChanged);

    // サイズ計算
    on_textChanged();
}

// シグナル

void AutoSizeTextEdit::on_textChanged()
{
    // 行のサイズを計算して領域を広げる

    qDebug() << document()->size();

    setMinimumHeight(document()->size().height());
}
