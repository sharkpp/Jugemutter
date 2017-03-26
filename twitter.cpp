#include "twitter.h"

#define STR__(x) #x
#define STR_(x) STR__(x) // うーむ、

static const int dataStreamVersion = QDataStream::Qt_5_8;

Twitter::Twitter(QObject *parent)
    : httpReplyHandler(nullptr)
{
    // 実行するとすぐにポートを開きに行くので遅延させる
    //setReplyHandler(new QOAuthHttpServerReplyHandler(this));

    // https://dev.twitter.com/oauth/reference/post/oauth/request_token
    setTemporaryCredentialsUrl(QUrl("https://api.twitter.com/oauth/request_token"));

    // https://dev.twitter.com/oauth/reference/get/oauth/authorize
    setAuthorizationUrl(QUrl("https://api.twitter.com/oauth/authenticate"));

    // https://dev.twitter.com/oauth/reference/post/oauth/access_token
    setTokenCredentialsUrl(QUrl("https://api.twitter.com/oauth/access_token"));

    connect(this, &QAbstractOAuth::authorizeWithBrowser, [=](QUrl url) {
        qDebug() << __FUNCTION__ << __LINE__ << url;
        QUrlQuery query(url);

        query.addQueryItem("force_login", "true");

        //if (!screenName.isEmpty()) {
        //    query.addQueryItem("screen_name", screenName);
        //}

        url.setQuery(query);
        QDesktopServices::openUrl(url);
    });

    connect(this, &QOAuth1::granted, this, &Twitter::authenticated);

    connect(this, &QOAuth1::requestFailed, [=](const Error error) {
        qDebug() << "QOAuth1::requestFailed" << (int)error;
    });

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
}

const QString Twitter::serialize() const
{
    QMap<QString, QString> serialized;
    QByteArray dataBytes;
    QDataStream out(&dataBytes, QIODevice::WriteOnly);
    out.setVersion(dataStreamVersion);

    if (QAbstractOAuth::Status::Granted == status()) {
        serialized.insert("token", token());
        serialized.insert("tokenSecret", tokenSecret());
    }

    out << serialized;

    return dataBytes.toBase64();
}

void Twitter::deserialize(const QString& data)
{
    QMap<QString, QString> deserialized;
    QByteArray dataBytes = QByteArray::fromBase64(data.toUtf8());
    QDataStream in(&dataBytes, QIODevice::ReadOnly);
    in.setVersion(dataStreamVersion);
    in >> deserialized;

    QString userToken       = deserialized.value("token");
    QString userTokenSecret = deserialized.value("tokenSecret");
    if (userToken.isEmpty() ||
        userTokenSecret.isEmpty()) {
        setTokenCredentials("", "");
        setStatus(QAbstractOAuth::Status::NotAuthenticated);
    }
    else {
        setTokenCredentials(userToken, userTokenSecret);
        setStatus(QAbstractOAuth::Status::Granted);
    }
}

void Twitter::authenticate()
{
    // ポートをここでオープン
    if (!httpReplyHandler) {
        httpReplyHandler = new QOAuthHttpServerReplyHandler(this);
        setReplyHandler(httpReplyHandler);
    }

    // 認証処理開始
    grant();
}

// OAuth check authentication method
bool Twitter::isAuthenticated() const
{
    return
        !token().isEmpty() &&
        !tokenSecret().isEmpty() &&
        QAbstractOAuth::Status::Granted == status();
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

    connect(reply, &QNetworkReply::finished, this, [=](){
        auto reply_ = qobject_cast<QNetworkReply*>(sender());

        qDebug() << "finished tweet";

        // {\n    \"errors\": [\n        {\n            \"code\": 170,\n            \"message\": \"Missing required parameter: status.\"\n        }\n    ]\n}\n

        QJsonParseError parseError;
        const auto resultJson = reply_->readAll();
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
    });

    return true;
}
