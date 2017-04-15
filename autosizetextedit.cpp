#include "autosizetextedit.h"
#include <QDebug>
#include <algorithm>

AutoSizeTextEdit::AutoSizeTextEdit(QWidget *parent)
    : QPlainTextEdit(parent)
{
    connect(this, &QPlainTextEdit::textChanged, this, &AutoSizeTextEdit::on_textChanged);

    setSizePolicy(sizePolicy().horizontalPolicy(), QSizePolicy::Fixed);

    // サイズ計算
    on_textChanged();
}

// シグナル

void AutoSizeTextEdit::on_textChanged()
{
    // 行のサイズを計算して領域を広げる

    qDebug() << document()->size();

    //const QMargins margins = contentsMargins();

    int widgetHeight
            = 6
            + (std::min)((int)document()->size().height(), 3) * fontMetrics().height()
            + 6;

    setMinimumHeight(widgetHeight);
    setMaximumHeight(widgetHeight);
}
