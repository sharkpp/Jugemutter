#ifndef TWITTER_H
#define TWITTER_H

#include <QtCore>
#include <QtNetwork>
#include <QtNetworkAuth>

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

  // API call

    bool tweet(const QString& text);

signals:
    void authenticated();
    void tweeted(const QString& tweetId);

private slots:

protected:
    QOAuthHttpServerReplyHandler *httpReplyHandler;
};

#endif // TWITTER_H
