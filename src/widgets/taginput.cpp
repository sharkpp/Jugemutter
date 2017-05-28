#include "taginput.h"
#include <QDebug>
#include <QKeyEvent>
#include <QTextDocument>
#include <QTextBlock>
#include <QTextDocumentFragment>

TagInput::TagInput(QWidget *parent)
    : QTextEdit(parent)
{
    //connect(this, &QTextEdit::textChanged, this, &TagInput::updateTags);
}

void TagInput::append(const QString &tag)
{

}

void TagInput::remove(const QString &tag)
{

}

void TagInput::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case ',': {
        //QTextDocument *doc = this->document();
        QTextCursor cursor = textCursor();
        cursor.insertText(",");
        updateTags();
        return; }
    case Qt::Key_Return:
        updateTags();
        return;
    case Qt::Key_Delete:
        return;
    case Qt::Key_Backspace:
        return;
    case Qt::Key_Left:
        return;
    case Qt::Key_Right:
        return;
   default:
        break;
    }

    qDebug() << event->key();

    QTextEdit::keyPressEvent(event);
}

void TagInput::updateTags()
{
    QTextDocument *doc = this->document();

    qDebug() << "updateTags()" << "blockCount=" << doc->blockCount();

    QTextBlock block = doc->firstBlock();

    QStringList tagsNew;

    for (; block.isValid(); block = block.next() )
    {
        qDebug() << "updateTags()" << block.text();
        //
        QStringList tags = block.text().split(",", QString::SkipEmptyParts);
        tagsNew.append(tags);
    }

    qDebug() << "updateTags()" << "tags" << tagsNew;
//
    QString spanStyle
            = "border-radius: 10px; "
              "border: solid 10px #f00; "
              "color: #FFFFFF;"
              "margin-right: 10px;"
            "background: #19BC9C;";
    QString pStyle
            = "border-radius: 10px; "
              "border: solid 10px #f00; "
              "color: #FFFFFF;"
              "padding: 10px;"
              "margin-right: 10px;"
            "background: #19BC9C;";
    QString tdStyle
            = "border-radius: 10px; "
              "padding: 10px;"
              "background: #19BC9C;";
    setHtml(
                "<table><tr><td style=\""+tdStyle+"\"><span style=\""+spanStyle+"\">" +
                tagsNew.join(" &times;</span></td><td style=\""+tdStyle+"\"><span style=\""+spanStyle+"\">") +
                "</span></td></tr></table>"
                );

    //doc->clear();
    //QTextCursor cursor(doc);

#if 0
    for (QStringList::const_iterator
                ite = tagsNew.cbegin(),
                last= tagsNew.cend();
            ite != last; ++ite) {
        /*QTextDocumentFragment tag
            = QTextDocumentFragment::fromHtml(
                    "<span style=\""
                        "border-radius: 10px; "
                        "border: solid 10px #f00; "
                        "color: #0f0;"
                        "margin-right: 10px;"
                    "\">" + *ite + "</span>"
                );
        cursor.insertFragment(tag);*/
        QTextBlockFormat blockFormat;
        QTextCharFormat charFormat;
        blockFormat.setNonBreakableLines(true);
        blockFormat.setRightMargin(10);
        charFormat.setForeground(QBrush(QColor(255,0,0)));
        //QTextCursor cursor2(cursor.block());
        cursor.insertText(*ite);
        cursor.mergeBlockFormat(blockFormat);
        cursor.mergeBlockCharFormat(charFormat);
    }
#endif

}
