#ifndef PREFERENCE_H
#define PREFERENCE_H

#include <QObject>

class Preference
        : public QObject
{
    Q_OBJECT
public:
    explicit Preference(QObject *parent = 0);

    // get session save data
    const QString serialize() const;

    // set session saved data
    void deserialize(const QString& data);

    QString postPrefixFreeText() const;
    void setPostPrefixFreeText(const QString &postPrefixFreeText);

    QString postPrefixContinueText() const;
    void setPostPrefixContinueText(const QString &postPrefixContinueText);

    QString postPrefixFinishedText() const;
    void setPostPrefixFinishedText(const QString &postPrefixFinishedText);

    QString postPostfixFreeText() const;
    void setPostPostfixFreeText(const QString &postPostfixFreeText);

    QString postPostfixContinueText() const;
    void setPostPostfixContinueText(const QString &postPostfixContinueText);

    QString postPostfixFinishedText() const;
    void setPostPostfixFinishedText(const QString &postPostfixFinishedText);

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
};

#endif // PREFERENCE_H
