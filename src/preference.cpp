#include "preference.h"
#include <QtCore>

static const int dataStreamVersion = QDataStream::Qt_5_8;
static const QString keyPostPrefixFreeText      = "postPrefixFreeText";
static const QString keyPostPrefixContinueText  = "postPrefixContinueText";
static const QString keyPostPrefixFinishedText  = "postPrefixFinishText";
static const QString keyPostPostfixFreeText     = "postPostfixFreeText";
static const QString keyPostPostfixContinueText = "postPostfixContinueText";
static const QString keyPostPostfixFinishedText = "postPostfixFinishText";

Preference::Preference(QObject *parent)
    : QObject(parent)
    , m_postPrefixFreeText("")
    , m_postPrefixContinueText("つづき) ")
    , m_postPrefixFinishedText("おわり) ")
    , m_postPostfixFreeText("")
    , m_postPostfixContinueText(" (つづく")
    , m_postPostfixFinishedText("( おわり")
{

}

const QString Preference::serialize() const
{
    QMap<QString, QVariant> serialized;
    QByteArray dataBytes;
    QDataStream out(&dataBytes, QIODevice::WriteOnly);
    out.setVersion(dataStreamVersion);

    serialized.insert(keyPostPrefixFreeText,      m_postPrefixFreeText);
    serialized.insert(keyPostPrefixContinueText,  m_postPrefixContinueText);
    serialized.insert(keyPostPrefixFinishedText,  m_postPrefixFinishedText);
    serialized.insert(keyPostPostfixFreeText,     m_postPostfixFreeText);
    serialized.insert(keyPostPostfixContinueText, m_postPostfixContinueText);
    serialized.insert(keyPostPostfixFinishedText, m_postPostfixFinishedText);

    out << serialized;

    return dataBytes.toBase64();
}

void Preference::deserialize(const QString &data)
{
    QMap<QString, QVariant> deserialized;
    QByteArray dataBytes = QByteArray::fromBase64(data.toUtf8());
    QDataStream in(&dataBytes, QIODevice::ReadOnly);
    in.setVersion(dataStreamVersion);
    in >> deserialized;

    m_postPrefixFreeText      = deserialized.value(keyPostPrefixFreeText,      m_postPrefixFreeText).toString();
    m_postPrefixContinueText  = deserialized.value(keyPostPrefixContinueText,  m_postPrefixContinueText).toString();
    m_postPrefixFinishedText  = deserialized.value(keyPostPrefixFinishedText,  m_postPrefixFinishedText).toString();
    m_postPostfixFreeText     = deserialized.value(keyPostPostfixFreeText,     m_postPostfixFreeText).toString();
    m_postPostfixContinueText = deserialized.value(keyPostPostfixContinueText, m_postPostfixContinueText).toString();
    m_postPostfixFinishedText = deserialized.value(keyPostPostfixFinishedText, m_postPostfixFinishedText).toString();

    emit update();
}

QString Preference::postPrefixFreeText() const
{
    return m_postPrefixFreeText;
}

void Preference::setPostPrefixFreeText(const QString &postPrefixFreeText)
{
    m_postPrefixFreeText = postPrefixFreeText;
    emit update();
}

QString Preference::postPrefixContinueText() const
{
    return m_postPrefixContinueText;
}

void Preference::setPostPrefixContinueText(const QString &postPrefixContinueText)
{
    m_postPrefixContinueText = postPrefixContinueText;
    emit update();
}

QString Preference::postPrefixFinishedText() const
{
    return m_postPrefixFinishedText;
}

void Preference::setPostPrefixFinishedText(const QString &postPrefixFinishedText)
{
    m_postPrefixFinishedText = postPrefixFinishedText;
    emit update();
}

QString Preference::postPostfixFreeText() const
{
    return m_postPostfixFreeText;
}

void Preference::setPostPostfixFreeText(const QString &postPostfixFreeText)
{
    m_postPostfixFreeText = postPostfixFreeText;
    emit update();
}

QString Preference::postPostfixContinueText() const
{
    return m_postPostfixContinueText;
}

void Preference::setPostPostfixContinueText(const QString &postPostfixContinueText)
{
    m_postPostfixContinueText = postPostfixContinueText;
    emit update();
}

QString Preference::postPostfixFinishedText() const
{
    return m_postPostfixFinishedText;
}

void Preference::setPostPostfixFinishedText(const QString &postPostfixFinishedText)
{
    m_postPostfixFinishedText = postPostfixFinishedText;
    emit update();
}
