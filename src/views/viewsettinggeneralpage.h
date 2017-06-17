#ifndef VEIWSETTINGGENERALPAGE_H
#define VEIWSETTINGGENERALPAGE_H

#include <QString>
#include <QList>
#include "pageselector.h"
#include "preference.h"

class TagItem;

namespace Ui {
class ViewSettingGeneralPage;
}

class ViewSettingGeneralPage
    : public PageSelectorView
{
    Q_OBJECT

public:
    explicit ViewSettingGeneralPage(QWidget *parent = 0);
    ~ViewSettingGeneralPage();

    void setPreference(Preference *preference);

protected:
    const QString& tagFromPostTextType(Preference::PostTextType type);
    QString tagNameFromPostTextType(Preference::PostTextType type);
    Preference::PostTextType tagToPostTextType(const QString &type);

protected:
    void resizeEvent(QResizeEvent *event);

private slots:
    void onPreferenceUpdate();
    void on_postPrefixFreeText_editingFinished();
    void on_postPrefixContinueText_editingFinished();
    void on_postPrefixFinishedText_editingFinished();
    void on_postPostfixFreeText_editingFinished();
    void on_postPostfixContinueText_editingFinished();
    void on_postPostfixFinishedText_editingFinished();
    void onPrefixUpdateTags();
    void onPostfixUpdateTags();

private:
    Ui::ViewSettingGeneralPage *ui;
    Preference *m_preference;
};

#endif // VEIWSETTINGGENERALPAGE_H
