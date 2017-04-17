#include "twitter.h"

#define STR__(x) #x
#define STR_(x) STR__(x) // うーむ、

static const int dataStreamVersion = QDataStream::Qt_5_8;

Twitter::Twitter(QObject *parent)
    : QOAuth1(parent)
    , httpReplyHandler(nullptr)
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

    connect(this, &QOAuth1::granted, this, [=]() {
        verifyCredentials();
    });

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
    QMap<QString, QVariant> serialized;
    QByteArray dataBytes;
    QDataStream out(&dataBytes, QIODevice::WriteOnly);
    out.setVersion(dataStreamVersion);

    if (QAbstractOAuth::Status::Granted == status()) {
        serialized.insert("token", token());
        serialized.insert("tokenSecret", tokenSecret());
    }

    serialized.insert("id", m_id);
    serialized.insert("name", m_name);
    serialized.insert("screenName", m_screenName);
    serialized.insert("profileImage", m_icon);

    out << serialized;

    return dataBytes.toBase64();
}

void Twitter::deserialize(const QString& data)
{
    QMap<QString, QVariant> deserialized;
    QByteArray dataBytes = QByteArray::fromBase64(data.toUtf8());
    QDataStream in(&dataBytes, QIODevice::ReadOnly);
    in.setVersion(dataStreamVersion);
    in >> deserialized;

    m_id = deserialized.value("id").toString();
    m_name = deserialized.value("name").toString();
    m_screenName = deserialized.value("screenName").toString();
    m_icon = deserialized.value("profileImage").value<QIcon>();

    QString userToken       = deserialized.value("token").toString();
    QString userTokenSecret = deserialized.value("tokenSecret").toString();
    if (userToken.isEmpty() ||
        userTokenSecret.isEmpty()) {
        setTokenCredentials("", "");
        setStatus(QAbstractOAuth::Status::NotAuthenticated);
    }
    else {
        qDebug() << QString("USER_TOKEN='%1'").arg(userToken);
        qDebug() << QString("USER_TOKEN_SECRET='%1'").arg(userTokenSecret);
        setTokenCredentials(userToken, userTokenSecret);
        setStatus(QAbstractOAuth::Status::Granted);
    }
}

void Twitter::authenticate()
{
    // ポートをここでオープン
    if (!httpReplyHandler) {
        httpReplyHandler = new QOAuthHttpServerReplyHandler(this);
        const QString messageHtml
                = QString("<style>" \
                        "html, body { padding: 0; margin: 0; } " \
                        "body { background-color: #f5f8fb; padding: 1em; } " \
                        "body > div { vertical-align: middle; text-align: center; margin: auto 0; }" \
                    "</style>" \
                    "<div><h1>%1</h1><div>%2</div></div>")
                .arg(qApp->applicationName())
                .arg("コールバックを受け取りました。<br />このページは閉じていただいて問題ありません。");
        // https://bugreports.qt.io/browse/QTBUG-59725 が修正されるまでこのまま
        const QString postfixTag = "</body></html>";
        const int fixedPaddingSize = messageHtml.toUtf8().length() - messageHtml.length() - postfixTag.length();
        //httpReplyHandler->setCallbackText(messageHtml + postfixTag + QString(fixedPaddingSize, '*'));
        // 本来は
        httpReplyHandler->setCallbackText(messageHtml);
        // このようにしたかった
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

const QString &Twitter::id() const
{
    return m_id;
}

const QString &Twitter::screenName() const
{
    return m_screenName;
}

const QString &Twitter::name() const
{
    return m_name;
}

const QIcon &Twitter::icon() const
{
    return m_icon;
}

bool Twitter::tweet(const QString& text, const QString& inReplyToStatusId)
{
    // https://dev.twitter.com/rest/reference/post/statuses/update
    QUrl url("https://api.twitter.com/1.1/statuses/update.json");
    QUrlQuery query(url);

    QVariantMap data;
    data.insert("status", text);
    if (!inReplyToStatusId.isEmpty()) {
        data.insert("in_reply_to_status_id", inReplyToStatusId);
    }

    url.setQuery(query);

    QNetworkReply *reply = post(url, data);
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
            qDebug() << QString(resultJson).replace(QRegExp(" +"), " ");
            return;
        }

        const auto result = resultDoc.object();
        if (result.value("id_str").isUndefined()) {
            qDebug() << resultDoc.toJson();
            return;
        }

        qDebug() << "***\n" << QString(resultDoc.toJson()).replace(QRegExp(" +"), " ");

        const auto tweetId = result.value("id_str").toString();

        Q_EMIT tweeted(tweetId);
    });

    return true;
}

void Twitter::verifyCredentials(bool include_entities, bool skip_status, bool include_email)
{
    // https://dev.twitter.com/rest/reference/get/account/verify_credentials
    QUrl url("https://api.twitter.com/1.1/account/verify_credentials.json");
    QUrlQuery query(url);

    QVariantMap data;
    query.addQueryItem("include_entities", include_entities ? "true" : "false");
    query.addQueryItem("skip_status", skip_status ? "true" : "false");
    query.addQueryItem("include_email", include_email ? "true" : "false");

    url.setQuery(query);

    QNetworkReply *reply = get(url);
    connect(reply, &QNetworkReply::finished, this, [=](){
        auto reply_ = qobject_cast<QNetworkReply*>(sender());

        qDebug() << "verifyCredentials finished";

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
            qDebug() << "!resultDoc.isObject()\n" << QString(resultJson).replace(QRegExp(" +"), " ");
            return;
        }

        const auto result = resultDoc.object();
        if (result.value("id_str").isUndefined() ||
            result.value("name").isUndefined() ||
            result.value("screen_name").isUndefined() ||
            result.value("profile_image_url_https").isUndefined()) {
            qDebug() << "!result.value(\"id_str\").isUndefined()\n" << resultDoc.toJson();
            return;
        }

        qDebug() << "***\n" << QString(resultDoc.toJson()).replace(QRegExp(" +"), " ");

        m_id = result.value("id_str").toString();
        m_screenName = result.value("screen_name").toString();
        m_name = result.value("name").toString();
        const auto profileImageUrlHttps = result.value("profile_image_url_https").toString();

        qDebug() << m_id;
        qDebug() << profileImageUrlHttps;

        // アイコン取得
        QNetworkAccessManager *netManager = networkAccessManager();
        QNetworkRequest reqIcon;
        reqIcon.setUrl(QUrl(profileImageUrlHttps));
        QNetworkReply *replyIcon = netManager->get(reqIcon);
        connect(replyIcon, &QNetworkReply::finished, this, [=](){

            QImage profileImage;
            profileImage.loadFromData(replyIcon->readAll());
            m_icon = QIcon(QPixmap::fromImage(profileImage));

            Q_EMIT verified();
        });
    });
}
