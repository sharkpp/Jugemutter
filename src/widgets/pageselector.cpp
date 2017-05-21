#include "pageselector.h"
#include <QStackedWidget>

struct SearchButtonInfoByAction {
    QAction *action_;
    SearchButtonInfoByAction(QAction *action)
        : action_(action) {
    }
    bool operator()(const PageSelectorButton* button) const {
      return button->action() == action_;
    }
};

struct SearchButtonInfoByView {
    QWidget *view_;
    SearchButtonInfoByView(QWidget *view)
        : view_(view) {
    }
    bool operator()(const PageSelectorButton* button) const {
      return button->view() == view_;
    }
};

struct SearchButtonInfoByDocument {
    PageSelectorDocument *document_;
    SearchButtonInfoByDocument(PageSelectorDocument *document)
        : document_(document) {
    }
    bool operator()(const PageSelectorButton* button) const {
      return button->document() == document_;
    }
};

//---------------------------------------------------------
// PageSelectorDocument
//---------------------------------------------------------

PageSelectorDocument::PageSelectorDocument(QObject *parent)
    : QObject(parent)
{
}

//---------------------------------------------------------
// PageSelectorView
//---------------------------------------------------------

PageSelectorView::PageSelectorView(QWidget *parent)
    : QFrame(parent)
    , m_document(nullptr)
{
}

void PageSelectorView::setDocument(PageSelectorDocument *document)
{
    m_document = document;
}

PageSelectorDocument *PageSelectorView::document()
{
    return m_document;
}

//---------------------------------------------------------
// PageSelectorButton
//---------------------------------------------------------

PageSelectorButton::PageSelectorButton(QObject *parent)
    : QObject(parent)
    , m_action(nullptr)
    , m_view(nullptr)
    , m_document(nullptr)
{

}

PageSelectorButton::PageSelectorButton(QObject *parent, QAction *action, PageSelectorView *view, PageSelectorDocument* document)
    : QObject(parent)
    , m_action(action)
    , m_view(view)
    , m_document(document)
{
}

QAction *PageSelectorButton::action() const
{
    return m_action;
}

void PageSelectorButton::setAction(QAction *action)
{
    m_action = action;
}

PageSelectorView *PageSelectorButton::view() const
{
    return m_view;
}

void PageSelectorButton::setView(PageSelectorView *view)
{
    m_view = view;
}

PageSelectorDocument *PageSelectorButton::document() const
{
    return m_document;
}

void PageSelectorButton::setDocument(PageSelectorDocument *document)
{
    m_document = document;
}

//---------------------------------------------------------
// PageSelector
//---------------------------------------------------------

PageSelector::PageSelector(QWidget *parent)
    : QToolBar(parent)
    , m_buddy(nullptr)
    , m_blankView(nullptr)
    , m_selected(nullptr)
{
    connect(this, &QToolBar::actionTriggered,
            this, &PageSelector::on_actionTriggered);
}

QList<PageSelectorButton *> PageSelector::buttons() const
{
    return m_buttons;
}

QWidget *PageSelector::buddy() const
{
    return m_buddy;
}

void PageSelector::setBuddy(QWidget *buddy)
{
    if (QStackedWidget* container = qobject_cast<QStackedWidget*>( buddy )) {
        if (m_blankView) {
            container->addWidget(m_blankView);
        }
    }

    m_buddy = buddy;
}

QAction *PageSelector::addSpacer()
{
    QWidget* spacer = new QWidget();
    spacer->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    return addWidget(spacer);
}

QAction *PageSelector::addSpacer(QAction *before)
{
    QWidget* spacer = new QWidget();
    spacer->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    return insertWidget(before, spacer);
}

QAction *PageSelector::addButton(QAction *action, PageSelectorView *view, PageSelectorDocument* document)
{
    if (view && document) { // checkable(= page selectable), when valid view and document present
        action->setCheckable(true);
    }

    QToolBar::addAction(action);

    PageSelectorButton *button
            = new PageSelectorButton(this, action, view, document);
    m_buttons.push_back(button);

    if (m_buddy &&
        action->isCheckable() &&
        1 == m_buttons.size()) {
        setCurrentAction(action);
    }

    return action;
}

QAction *PageSelector::insertButton(QAction *before, QAction *action, PageSelectorView *view, PageSelectorDocument* document)
{
    if (document && document) { // checkable(= page selectable), when valid view and document present
        action->setCheckable(true);
    }

    QToolBar::insertAction(before, action);

    PageSelectorButton *button
            = new PageSelectorButton(this, action, view, document);
    QList<PageSelectorButton *>::iterator
            ite = std::find_if(m_buttons.begin(), m_buttons.end(),
                               SearchButtonInfoByAction(before));
    if (m_buttons.end() != ite) {
        m_buttons.insert(ite, button);
    }
    else {
        m_buttons.insert(m_buttons.begin(), button);
    }

    if (m_buddy && 1 == m_buttons.size()) {
        setCurrentAction(action);
    }

    return action;
}

void PageSelector::removeButton(QAction *action)
{
    QList<PageSelectorButton *>::iterator
            ite = std::find_if(m_buttons.begin(), m_buttons.end(),
                               SearchButtonInfoByAction(action));
    if (m_buttons.end() == ite) {
        return;
    }

    QToolBar::removeAction((*ite)->action());

    m_buttons.erase(ite);
}

void PageSelector::removeButton(PageSelectorDocument *document)
{
    QList<PageSelectorButton *>::iterator
            ite = std::find_if(m_buttons.begin(), m_buttons.end(),
                               SearchButtonInfoByDocument(document));
    if (m_buttons.end() == ite) {
        return;
    }

    QToolBar::removeAction((*ite)->action());

    m_buttons.erase(ite);

}

void PageSelector::removeButtons(QList<QAction *> actions)
{
    for (auto action : actions) {
        removeButton(action);
    }
}

void PageSelector::removeButtons(QList<PageSelectorDocument *> documents)
{
    for (auto document : documents) {
        removeButton(document);
    }
}

void PageSelector::setCurrentButton(PageSelectorButton *currentButton)
{
    if (currentButton) { // selected, emit select button action
        emit actionTriggered(currentButton->action()); // call on_actionTriggered() function
    }
    else if (m_buddy) { // no selected, clear buddied form
        if (QStackedWidget* container = qobject_cast<QStackedWidget*>( m_buddy )) {
            container->setCurrentWidget(m_blankView);
        }
    }
}

PageSelectorView *PageSelector::currentView() const
{
    return m_selected ? m_selected->view() : nullptr;
}

void PageSelector::setCurrentView(PageSelectorView *currentView)
{
    m_selected = nullptr;

    for (auto button : m_buttons ) {
        if (button->action()->isCheckable() &&
            button->view() == currentView) {
            m_selected = button;
            break;
        }
    }

    setCurrentButton(m_selected);
}

PageSelectorView *PageSelector::blankView() const
{
    return m_blankView;
}

void PageSelector::setBlankView(PageSelectorView *blankView)
{
    if (QStackedWidget* container = qobject_cast<QStackedWidget*>( m_buddy )) {
        container->removeWidget(m_blankView);
        container->addWidget(blankView);
    }

    m_blankView = blankView;
}

QList<PageSelectorDocument *> PageSelector::documents() const
{
    QList<PageSelectorDocument *> results;

    for (auto button : m_buttons) {
        if (button->action()->isCheckable() &&
            button->document()) {
            results.push_back(button->document());
        }
    }

    return results;
}

PageSelectorDocument *PageSelector::documentAt(QAction *action) const
{
    for (auto button : m_buttons ) {
        if (button->action() == action) {
            return button->document();
        }
    }
    return nullptr;
}

PageSelectorDocument *PageSelector::documentAt(PageSelectorView *view) const
{
    for (auto button : m_buttons ) {
        if (button->view() == view) {
            return button->document();
        }
    }
    return nullptr;
}

QAction *PageSelector::currentAction() const
{
    return m_selected ? m_selected->action() : nullptr;
}

void PageSelector::setCurrentAction(QAction *currentAction)
{
    PageSelectorButton *newSelectButton = nullptr;

    for (auto button : m_buttons ) {
        if (button->action()->isCheckable() &&
            button->action() == currentAction) {
            newSelectButton = button;
            break;
        }
    }

    if (m_selected != newSelectButton) {
        m_selected = newSelectButton;
        setCurrentButton(m_selected);
    }
}

void PageSelector::on_actionTriggered(QAction *action)
{
    for (auto button : m_buttons ) {
        if (!button->action()->isCheckable()) {
            continue;
        }
        bool selected = button->action() == action;
        button->action()->setChecked( selected );
        if (selected && m_buddy) {
            if (QStackedWidget* container = qobject_cast<QStackedWidget*>( m_buddy )) {
                button->view()->setDocument(button->document());
                container->setCurrentWidget(button->view());
            }
        }
    }
}
