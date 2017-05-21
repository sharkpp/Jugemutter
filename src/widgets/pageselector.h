#ifndef PAGESELECTOR_H
#define PAGESELECTOR_H

#include <QToolBar>
#include <QList>
#include <QFrame>

class PageSelectorDocument
    : public QObject
{
    Q_OBJECT
public:
    PageSelectorDocument(QObject* parent = nullptr);
};

class PageSelectorView
    : public QFrame
{
    Q_OBJECT
public:
    PageSelectorView(QWidget* parent = nullptr);
    //
    virtual void setDocument(PageSelectorDocument *document);
    PageSelectorDocument *document();
protected:
    PageSelectorDocument *m_document;
};

class PageSelectorButton
    : public QObject
{
    Q_OBJECT
public:
    PageSelectorButton(QObject* parent = nullptr);
    PageSelectorButton(QObject* parent, QAction *action, PageSelectorView *view, PageSelectorDocument *document);

    QAction *action() const;
    void setAction(QAction *action);

    PageSelectorView *view() const;
    void setView(PageSelectorView *view);

    PageSelectorDocument *document() const;
    void setDocument(PageSelectorDocument *document);

protected:
    QAction *m_action;
    PageSelectorView *m_view;
    PageSelectorDocument *m_document;
};

class PageSelector
    : public QToolBar
{
    Q_OBJECT

    void addAction(QAction *action);
    void addActions(QList<QAction *> actions);
    void insertAction(QAction *before, QAction *action);
    void insertActions(QAction *before, QList<QAction *> actions);
    void removeAction(QAction *action);

public:
    PageSelector(QWidget *parent = 0);

    QList<PageSelectorButton *> buttons() const;

    QWidget *buddy() const;
    void setBuddy(QWidget *buddy);

    QAction *addSpacer();

    QAction *addSpacer(QAction *before);

    QAction *addButton(QAction *action, PageSelectorView *view = nullptr, PageSelectorDocument* document = nullptr);

    QAction *insertButton(QAction *before, QAction *action, PageSelectorView *view = nullptr, PageSelectorDocument* document = nullptr);

    void removeButton(QAction *action);

    void removeButton(PageSelectorDocument *document);

    void removeButtons(QList<QAction *> actions);

    void removeButtons(QList<PageSelectorDocument *> documents);

    template <class Predicate> inline void removeButtonIf(Predicate pred);

    QAction *currentAction() const;
    void setCurrentAction(QAction *currentAction);

    PageSelectorView *currentView() const;
    void setCurrentView(PageSelectorView *currentView);

    PageSelectorView *blankView() const;
    void setBlankView(PageSelectorView *blankView);

    QList<PageSelectorDocument *> documents() const;

    PageSelectorDocument *documentAt(QAction *action) const;
    PageSelectorDocument *documentAt(PageSelectorView *view) const;

protected:
    void setCurrentButton(PageSelectorButton* currentButton);

protected slots:
    void on_actionTriggered(QAction *action);

private:
    QWidget* m_buddy;
    QList<PageSelectorButton *> m_buttons;
    PageSelectorView *m_blankView;
    PageSelectorButton *m_selected;
};

template<class Predicate>
inline
void PageSelector::removeButtonIf(Predicate pred)
{
    QList<QAction*> removeActions;
    for (auto button : m_buttons) {
        QAction *action = button->action();
        if (action && pred(action)) {
            removeActions.push_back(action);
        }
    }
    removeButtons(removeActions);
}

#endif // PAGESELECTOR_H
