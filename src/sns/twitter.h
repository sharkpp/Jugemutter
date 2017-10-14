#ifndef TWITTER_H
#define TWITTER_H

#include <QtCore>
#include <QtNetwork>
#include <QtNetworkAuth>
#include <QIcon>

class Twitter
    : public QOAuth1
{
    Q_OBJECT
public:

    explicit Twitter(QObject *parent = 0);

  // OAuth auth method

    // get session save data
    const QString serialize() const;

    // set session saved data
    void deserialize(const QString& data);

    // OAuth authentication method
    void authenticate();

    // OAuth check authentication method
    bool isAuthenticated() const;

  // account info

    const QString &id() const;

    const QString &screenName() const;

    const QString &name() const;

    const QIcon &icon() const;

  // API call

    bool tweet(const QString& text, const QString &inReplyToStatusId = "");

protected:

    void verifyCredentials(bool include_entities = false, bool skip_status = true, bool include_email = true);

    QNetworkReply *post_(const QUrl &url, const QVariantMap &parameters);

signals:
    void authenticated();
    void verified();
    void tweeted(const QString& tweetId);
    void tweetFailure(const QList<QPair<int, QString> > & errors);

private slots:

protected:
    QOAuthHttpServerReplyHandler *httpReplyHandler;
    QString m_id;
    QString m_screenName;
    QString m_name;
    QIcon m_icon;
};

#endif // TWITTER_H
