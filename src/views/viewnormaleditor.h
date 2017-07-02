#ifndef VIEWNORMALEDITOR_H
#define VIEWNORMALEDITOR_H

#include "pageselector.h"
#include "preference.h"
#include "twittertextsplitter.h"

class AutoSizeTextEdit;
class PostProgress;
class Twitter;

namespace Ui {
class ViewNormalEditor;
}

class ViewNormalEditor
        : public PageSelectorView
{
    Q_OBJECT

public:
    explicit ViewNormalEditor(QWidget *parent = 0);
    ~ViewNormalEditor();

    void setPreference(Preference *preference);

    virtual void setDocument(PageSelectorDocument *document);

protected:
    QWidget *getPostTextWidget(Preference::PostTextType type);
    TwitterTextSplitter::TextTypeValue textTypeValueFromPostType(Preference::PostTextType type);
    void updateSplitStatus();
    void startPost();
    void stepPost();
    void finishPost();

private slots:
    void onPreferenceUpdate();
    void onTwitterTweeted(const QString& tweetId);
    void on_tweetButton_clicked();
    void on_prefixFreeText_textChanged();
    void on_postfixFreeText_textChanged();
    void on_tweetEditor_textChanged();

private:
    Ui::ViewNormalEditor *ui;
    Preference *m_preference;
    PostProgress *postProgress;
    Twitter *currentAccount;
    QList<SplittedItem> tweetQueue; // queued tweet
};

#endif // VIEWNORMALEDITOR_H
