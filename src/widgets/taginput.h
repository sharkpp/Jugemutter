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
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

private slots:
    void onTagRemoveClick(bool checked);
    void onTagClick();

protected:
    QPoint m_dragStartPos;
    int m_dragStartOffset;
    QStringList m_tags;
    QHBoxLayout *m_layout;
};

#endif // TAGINPUT_H
