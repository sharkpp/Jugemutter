#ifndef TAGINPUT_H
#define TAGINPUT_H

#include <QScrollArea>
#include <QList>
#include <QString>

class QHBoxLayout;
class QComboBox;

class TagItem : public QObject
{
    Q_OBJECT

public:
    TagItem(QObject *parent = nullptr);
    TagItem(const QString &tagName, QObject *parent = nullptr);
    TagItem(const QString &tagName, const QString &tagId, QObject *parent = nullptr);

    QString tagName() const;
    void setTagName(const QString &tagName);

    QString id() const;
    void setId(const QString &id);

    QString descriptionText() const;
    void setDescriptionText(const QString &descriptionText);

    bool unique() const;
    void setUnique(bool unique);

    TagItem *clone();

protected:
    QString m_tagName;
    QString m_id;
    QString m_descriptionText;
    bool m_unique;
};


class TagInput
        : public QScrollArea
{
    Q_OBJECT

public:
    TagInput(QWidget *parent);

    void clearTags();

    void addTag(const QString &tag, const QString &tagId);
    void addTag(TagItem *item);

    void addTags(QList<TagItem *> items);

    void replaceTags(QList<TagItem *> items);

    void removeTagById(const QString &tagId);

    const QList<TagItem *> &tags();

    void clearTagList();

    void addTagList(const QString &tag, const QString &tagId);
    void addTagList(TagItem *item);

    void removeTagListById(const QString &tagId);

protected:
    void initWidget();
    QFrame *createTagItem(TagItem *item);
    void onTagListSelected(int index);

protected: // event
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

signals:
    void updateTags();

private slots:
    void onTagRemoveClick(bool checked);
    void onTagClick();

protected:
    QHBoxLayout *m_layout; // layout
    QComboBox *m_tagListWidget;
    QPoint m_dragStartPos; // tag item hscroll start pos
    int m_dragStartOffset; // tag item hscroll start offset
    QList<TagItem*> m_tags; // tag list
    QList<TagItem*> m_tagList; // tag list
};

#endif // TAGINPUT_H
