#ifndef TAGINPUT_H
#define TAGINPUT_H

#include <QTextEdit>
#include <QStringList>

class TagInput
        : public QTextEdit
{
    Q_OBJECT

public:
    TagInput(QWidget *parent);

    void append(const QString &tag);

    void remove(const QString &tag);

protected: // event
    void keyPressEvent(QKeyEvent *event);

private slots:
    void updateTags();

protected:
    QStringList m_tags;
};

#endif // TAGINPUT_H
