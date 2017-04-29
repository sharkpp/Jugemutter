#ifndef PAGESELECTOR_H
#define PAGESELECTOR_H

#include <QToolBar>
#include <QList>

class PageSelectorDocument
    : public QObject
{
    Q_OBJECT
public:
    PageSelectorDocument(QObject* parent = nullptr);
};

class PageSelectorButton
    : public QObject
{
    Q_OBJECT
public:
    PageSelectorButton(QObject* parent = nullptr);
    PageSelectorButton(QObject* parent, QAction *action, QWidget *view, PageSelectorDocument *document);

    QAction *action() const;
    void setAction(QAction *action);

    QWidget *view() const;
    void setView(QWidget *view);

    PageSelectorDocument *document() const;
    void setDocument(PageSelectorDocument *document);

protected:
    QAction *m_action;
    QWidget *m_view;
    PageSelectorDocument *m_document;
};

class PageSelector
    : public QToolBar
{
    Q_OBJECT

    //friend struct SearchButtonInfoByAction;
    //friend struct SearchButtonInfoByView;

public:
    PageSelector(QWidget *parent = 0);

    QList<PageSelectorButton *> buttons() const;

    QWidget *buddy() const;
    void setBuddy(QWidget *buddy);

    QAction *addSpacer();

    QAction *addSpacer(QAction *before);

    QAction *addButton(QAction *action, QWidget *view = nullptr, PageSelectorDocument* document = nullptr);

    QAction *insertButton(QAction *before, QAction *action, QWidget *view = nullptr, PageSelectorDocument* document = nullptr);

    QAction *currentAction() const;
    void setCurrentAction(QAction *currentAction);

    QWidget *currentView() const;
    void setCurrentView(QWidget *currentView);

    PageSelectorDocument *documentAt(QAction *action) const;
    PageSelectorDocument *documentAt(QWidget *view) const;

protected:
    void setCurrentButton(PageSelectorButton* currentButton);

protected slots:
    void on_actionTriggered(QAction *action);

private:
    QWidget* m_buddy;
    QList<PageSelectorButton *> m_buttons;
    PageSelectorButton *m_selected;
};

#endif // PAGESELECTOR_H
