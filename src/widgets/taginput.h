#ifndef TAGINPUT_H
#define TAGINPUT_H

#include <QScrollArea>
#include <QStringList>

class QHBoxLayout;

class TagInput
        : public QScrollArea
{
    Q_OBJECT

public:
    TagInput(QWidget *parent);

    void append(const QString &tag);

    void remove(const QString &tag);

protected:
    void updateTags();

protected: // event

private slots:
    void onTagRemoveClick(bool checked);
    void onTagClick();

protected:
    QStringList m_tags;
    QHBoxLayout *m_layout;
};

#endif // TAGINPUT_H
