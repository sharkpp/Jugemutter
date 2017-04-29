#include "pageselector.h"
#include <QStackedWidget>

struct SearchButtonInfoByAction {
    QAction *action_;
    SearchButtonInfoByAction(QAction *action) : action_(action) {
    }
    bool operator()(const PageSelectorButton* info) const {
      return info->action() == action_;
    }
};

struct SearchButtonInfoByView {
    QWidget *view_;
    SearchButtonInfoByView(QWidget *view) : view_(view) {
    }
    bool operator()(const PageSelectorButton* info) const {
      return info->view() == view_;
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
// PageSelectorButton
//---------------------------------------------------------

PageSelectorButton::PageSelectorButton(QObject *parent)
    : QObject(parent)
    , m_action(nullptr)
    , m_view(nullptr)
    , m_document(nullptr)
{

}

PageSelectorButton::PageSelectorButton(QObject *parent, QAction *action, QWidget *view, PageSelectorDocument* document)
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

QWidget *PageSelectorButton::view() const
{
    return m_view;
}

void PageSelectorButton::setView(QWidget *view)
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

QAction *PageSelector::addButton(QAction *action, QWidget *view, PageSelectorDocument* document)
{
    addAction(action);

    PageSelectorButton *button
            = new PageSelectorButton(this, action, view,
                                     document ? document : new PageSelectorDocument(this));
    m_buttons.push_back(button);

    return action;
}

QAction *PageSelector::insertButton(QAction *before, QAction *action, QWidget *view, PageSelectorDocument* document)
{
    insertAction(before, action);

    PageSelectorButton *button
            = new PageSelectorButton(this, action, view,
                                     document ? document : new PageSelectorDocument(this));
    QList<PageSelectorButton *>::iterator
            ite = std::find_if(m_buttons.begin(), m_buttons.end(),
                               SearchButtonInfoByAction(before));
    if (m_buttons.end() != ite) {
        m_buttons.insert(ite, button);
    }
    else {
        m_buttons.insert(m_buttons.begin(), button);
    }

    return action;
}

void PageSelector::setCurrentButton(PageSelectorButton *currentButton)
{
//    QAction* selectedAction = nullptr;
//
//    for (auto button : m_buttons ) {
//        if (!button.action->isCheckable()) {
//            continue;
//        }
//        bool selected = currentButton && button.action == currentButton->action;
//        button.action->setChecked( selected );
//        if (selected) {
//            selectedAction = button.action;
//            if (m_buddy) {
//                if (QStackedWidget* container = qobject_cast<QStackedWidget*>( m_buddy )) {
//                    container->setCurrentWidget(button.view);
//                }
//            }
//        }
//    }
//
//    if (selectedAction) {
//        emit actionTriggered(selectedAction);
//    }
//    else if (m_buddy) {
//        if (QStackedWidget* container = qobject_cast<QStackedWidget*>( m_buddy )) {
//            container->setCurrentWidget(nullptr);
//        }
//    }
    if (currentButton) {
        emit actionTriggered(currentButton->action());
    }
    else if (m_buddy) {
        if (QStackedWidget* container = qobject_cast<QStackedWidget*>( m_buddy )) {
            container->setCurrentWidget(nullptr);
        }
    }
}

QWidget *PageSelector::currentView() const
{
    return m_selected ? m_selected->view() : nullptr;
}

void PageSelector::setCurrentView(QWidget *currentView)
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

PageSelectorDocument *PageSelector::documentAt(QAction *action) const
{
    for (auto button : m_buttons ) {
        if (button->action() == action) {
            return button->document();
        }
    }
    return nullptr;
}

PageSelectorDocument *PageSelector::documentAt(QWidget *view) const
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
    m_selected = nullptr;

    for (auto button : m_buttons ) {
        if (button->action()->isCheckable() &&
            button->action() == currentAction) {
            m_selected = button;
            break;
        }
    }

    setCurrentButton(m_selected);
}

void PageSelector::on_actionTriggered(QAction *action)
{
    QAction* selectedAction = nullptr;

    for (auto button : m_buttons ) {
        if (!button->action()->isCheckable()) {
            continue;
        }
        bool selected = button->action() == action;
        button->action()->setChecked( selected );
        if (selected) {
            selectedAction = button->action();
            if (m_buddy) {
                if (QStackedWidget* container = qobject_cast<QStackedWidget*>( m_buddy )) {
                    container->setCurrentWidget(button->view());
                }
            }
        }
    }
}
