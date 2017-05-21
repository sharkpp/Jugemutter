#ifndef VIEWNORMALEDITOR_H
#define VIEWNORMALEDITOR_H

#include "pageselector.h"
#include "twittertextsplitter.h"

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

    virtual void setDocument(PageSelectorDocument *document);

protected:
    void updateSplitStatus();
    void startPost();
    void stepPost();
    void finishPost();

private slots:
    void onTwitterTweeted(const QString& tweetId);
    void on_tweetButton_clicked();
    void on_textPrefix_textChanged();
    void on_textPostfix_textChanged();
    void on_tweetEditor_textChanged();

private:
    Ui::ViewNormalEditor *ui;
    PostProgress *postProgress;
    Twitter *currentAccount;
    QList<SplittedItem> tweetQueue; // queued tweet
};

#endif // VIEWNORMALEDITOR_H
