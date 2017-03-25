#include "twitter.h"

#define STR__(x) #x
#define STR_(x) STR__(x) // うーむ、

Twitter::Twitter(QObject *parent)
{
    setReplyHandler(new QOAuthHttpServerReplyHandler(this));

    setModifyParametersFunction([&](Stage stage, QVariantMap* data) {
        qDebug() << "stage:" << (int)stage << data;
    });

    // https://dev.twitter.com/oauth/reference/post/oauth/request_token
    setTemporaryCredentialsUrl(QUrl("https://api.twitter.com/oauth/request_token"));

    // https://dev.twitter.com/oauth/reference/get/oauth/authorize
    setAuthorizationUrl(QUrl("https://api.twitter.com/oauth/authenticate"));

    // https://dev.twitter.com/oauth/reference/post/oauth/access_token
    setTokenCredentialsUrl(QUrl("https://api.twitter.com/oauth/access_token"));

    QObject::connect(this, &QAbstractOAuth::authorizeWithBrowser,
                     this, &Twitter::handleAuthorizeWithBrowser);

    QObject::connect(this, &QOAuth1::granted, this, &Twitter::authenticated);

    qDebug() << QString("TWITTER_APP_KEY='%1'").arg(STR_(TWITTER_APP_KEY));
    qDebug() << QString("TWITTER_APP_SECRET='%1'").arg(STR_(TWITTER_APP_SECRET));

    // QAbstractOAuth::setClientIdentifier()
    // > qmake ... DEFINES+=TWITTER_APP_KEY="{App key}"
    // --> https://apps.twitter.com/app/{App key}/keys
    setClientIdentifier(STR_(TWITTER_APP_KEY));

    // QAbstractOAuth::setClientSharedSecret()
    // > qmake ... DEFINES+=TWITTER_APP_SECRET="{App secret}"
    // --> https://apps.twitter.com/app/{App key}/keys
    setClientSharedSecret(STR_(TWITTER_APP_SECRET));

     grant();
}

void Twitter::handleAuthorizeWithBrowser(QUrl url)
{
qDebug() << __FUNCTION__ << __LINE__ << url;
    QUrlQuery query(url);

    //query.addQueryItem("force_login", "true");

    //if (!screenName.isEmpty()) {
    //    query.addQueryItem("screen_name", screenName);
    //}

    url.setQuery(query);
    QDesktopServices::openUrl(url);
}

const QString Twitter::serialize() const
{
    if (QAbstractOAuth::Status::Granted != status()) {
        return "";
    }
    return token() + ":" + tokenSecret();
}

void Twitter::deserialize(const QString& tokenAndTokenSecret)
{
    if (tokenAndTokenSecret.isEmpty()) {
        return;
    }

    QStringList splitedTokenAndTokenSecret = tokenAndTokenSecret.split(':');

    if (splitedTokenAndTokenSecret.count() < 2) {
        return;
    }

    setTokenCredentials(splitedTokenAndTokenSecret[0], splitedTokenAndTokenSecret[1]);
    setStatus(QAbstractOAuth::Status::Granted);
}

void Twitter::authenticate()
{
    grant();
}

// OAuth check authentication method
bool Twitter::isAuthenticated() const
{
    return !token().isEmpty() && !tokenSecret().isEmpty();
}

bool Twitter::tweet(const QString& text)
{
    // https://dev.twitter.com/rest/reference/post/statuses/update
    QUrl url("https://api.twitter.com/1.1/statuses/update.json");
    QUrlQuery query(url);

    QVariantMap data;
    //data.insert("status", text);
    query.addQueryItem("status", text); // required, 140 char

    url.setQuery(query);
    QNetworkReply *reply = post(url);

    connect(reply, &QNetworkReply::finished, this, &Twitter::tweetFinished);
}

void Twitter::tweetFinished()
{
    auto reply = qobject_cast<QNetworkReply*>(sender());

    qDebug() << "finished tweet";

    // {\n    \"errors\": [\n        {\n            \"code\": 170,\n            \"message\": \"Missing required parameter: status.\"\n        }\n    ]\n}\n

    QJsonParseError parseError;
    const auto resultJson = reply->readAll();
    const auto resultDoc = QJsonDocument::fromJson(resultJson, &parseError);
    if (parseError.error) {
        qDebug() << QString(resultJson);
        qCritical() << "Twitter::tweet() finished Error at:" << parseError.offset
                    << parseError.errorString();
        return;
    }
    else if (!resultDoc.isObject()) {
        qDebug() << QString(resultJson);
        return;
    }

    const auto result = resultDoc.object();
    if (!result.value("id_str").isUndefined()) {
        qDebug() << resultDoc.toJson();
        return;
    }

    qDebug() << "***\n" << resultDoc.toJson();

    const auto tweetId = result.value("id_str").toString();

    Q_EMIT tweeted(tweetId);
}
