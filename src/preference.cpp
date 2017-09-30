#include "preference.h"
#include <QtCore>

static const int dataStreamVersion = QDataStream::Qt_5_8;
static const QString keyPostPrefixText          = "postPrefixText";
static const QString keyPostPostfixText         = "postPostfixText";
static const QString keyPostPrefixFreeText      = "postPrefixFreeText";
static const QString keyPostPrefixContinueText  = "postPrefixContinueText";
static const QString keyPostPrefixFinishedText  = "postPrefixFinishText";
static const QString keyPostPostfixFreeText     = "postPostfixFreeText";
static const QString keyPostPostfixContinueText = "postPostfixContinueText";
static const QString keyPostPostfixFinishedText = "postPostfixFinishText";

QVariantList PostTextTypeListToQVariantList(const QList<Preference::PostTextType> &list)
{
    QVariantList r;
    foreach (auto item, list) {
        r.append(QVariant::fromValue((int)item));
    }
    return r;
}

QList<Preference::PostTextType> PostTextTypeListFromQVariantList(const QVariantList &list)
{
    QList<Preference::PostTextType> r;
    foreach (auto item, list) {
        r.append((Preference::PostTextType)item.toInt());
    }
    return r;
}

Preference::Preference(QObject *parent)
    : QObject(parent)
    , m_postPrefixFreeText("")
    , m_postPrefixContinueText(tr("cont.) "))
    , m_postPrefixFinishedText(tr("end) "))
    , m_postPostfixFreeText("")
    , m_postPostfixContinueText(tr(" (cont."))
    , m_postPostfixFinishedText(tr(" (end"))
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

    serialized.insert(keyPostPrefixText,  PostTextTypeListToQVariantList(m_postPrefixText));
    serialized.insert(keyPostPostfixText, PostTextTypeListToQVariantList(m_postPostfixText));

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

    m_postPrefixText  = PostTextTypeListFromQVariantList(deserialized.value(keyPostPrefixText).toList());
    m_postPostfixText = PostTextTypeListFromQVariantList(deserialized.value(keyPostPostfixText).toList());

    emit update();
}

const QString &Preference::postPrefixFreeText() const
{
    return m_postPrefixFreeText;
}

void Preference::setPostPrefixFreeText(const QString &postPrefixFreeText)
{
    if (m_postPrefixFreeText != postPrefixFreeText) {
        m_postPrefixFreeText = postPrefixFreeText;
        emit update();
    }
}

const QString &Preference::postPrefixContinueText() const
{
    return m_postPrefixContinueText;
}

void Preference::setPostPrefixContinueText(const QString &postPrefixContinueText)
{
    if (m_postPrefixContinueText != postPrefixContinueText) {
        m_postPrefixContinueText = postPrefixContinueText;
        emit update();
    }
}

const QString &Preference::postPrefixFinishedText() const
{
    return m_postPrefixFinishedText;
}

void Preference::setPostPrefixFinishedText(const QString &postPrefixFinishedText)
{
    if (m_postPrefixFinishedText != postPrefixFinishedText) {
        m_postPrefixFinishedText = postPrefixFinishedText;
        emit update();
    }
}

const QString &Preference::postPostfixFreeText() const
{
    return m_postPostfixFreeText;
}

void Preference::setPostPostfixFreeText(const QString &postPostfixFreeText)
{
    if (m_postPostfixFreeText != postPostfixFreeText) {
        m_postPostfixFreeText = postPostfixFreeText;
        emit update();
    }
}

const QString &Preference::postPostfixContinueText() const
{
    return m_postPostfixContinueText;
}

void Preference::setPostPostfixContinueText(const QString &postPostfixContinueText)
{
    if (m_postPostfixContinueText != postPostfixContinueText) {
        m_postPostfixContinueText = postPostfixContinueText;
        emit update();
    }
}

const QString &Preference::postPostfixFinishedText() const
{
    return m_postPostfixFinishedText;
}

void Preference::setPostPostfixFinishedText(const QString &postPostfixFinishedText)
{
    if (m_postPostfixFinishedText != postPostfixFinishedText) {
        m_postPostfixFinishedText = postPostfixFinishedText;
        emit update();
    }
}

const Preference::PostTextTypeList &Preference::postPrefixText() const
{
    return m_postPrefixText;
}

void Preference::setPostPrefixText(const Preference::PostTextTypeList &postPrefixText)
{
    if (m_postPrefixText != postPrefixText) {
        m_postPrefixText = postPrefixText;
        emit update();
    }
}

const Preference::PostTextTypeList &Preference::postPostfixText() const
{
    return m_postPostfixText;
}

void Preference::setPostPostfixText(const PostTextTypeList &postPostfixText)
{
    if (m_postPostfixText != postPostfixText) {
        m_postPostfixText = postPostfixText;
        emit update();
    }
}
