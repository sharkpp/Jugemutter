#ifndef AUTOSIZETEXTEDIT_H
#define AUTOSIZETEXTEDIT_H

#include <QPlainTextEdit>

class AutoSizeTextEdit : public QPlainTextEdit
{
    Q_OBJECT

public:
    AutoSizeTextEdit(QWidget *parent = Q_NULLPTR);

private slots:
    void on_textChanged();
};

#endif // AUTOSIZETEXTEDIT_H
