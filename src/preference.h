#ifndef PREFERENCE_H
#define PREFERENCE_H

#include <QObject>
#include <QList>

class Preference
        : public QObject
{
    Q_OBJECT

public:

    enum PostTextType {
        postTextPrefixFreeText = 1,
        postTextPrefixContinue,
        postTextPrefixFinished,
        postTextPostfixFree,
        postTextPostfixContinue,
        postTextPostfixFinished,
    };

    typedef QList<PostTextType> PostTextTypeList;

public:
    explicit Preference(QObject *parent = 0);

    // get session save data
    const QString serialize() const;

    // set session saved data
    void deserialize(const QString& data);

    const QString &postPrefixFreeText() const;
    void setPostPrefixFreeText(const QString &postPrefixFreeText);

    const QString &postPrefixContinueText() const;
    void setPostPrefixContinueText(const QString &postPrefixContinueText);

    const QString &postPrefixFinishedText() const;
    void setPostPrefixFinishedText(const QString &postPrefixFinishedText);

    const QString &postPostfixFreeText() const;
    void setPostPostfixFreeText(const QString &postPostfixFreeText);

    const QString &postPostfixContinueText() const;
    void setPostPostfixContinueText(const QString &postPostfixContinueText);

    const QString &postPostfixFinishedText() const;
    void setPostPostfixFinishedText(const QString &postPostfixFinishedText);

    const PostTextTypeList &postPrefixText() const;
    void setPostPrefixText(const PostTextTypeList &postPrefixText);

    const PostTextTypeList &postPostfixText() const;
    void setPostPostfixText(const PostTextTypeList &postPostfixText);

signals:
    void update();

public slots:

protected:
    QString m_postPrefixFreeText;
    QString m_postPrefixContinueText;
    QString m_postPrefixFinishedText;
    QString m_postPostfixFreeText;
    QString m_postPostfixContinueText;
    QString m_postPostfixFinishedText;
    PostTextTypeList m_postPrefixText;
    PostTextTypeList m_postPostfixText;
};

#endif // PREFERENCE_H
