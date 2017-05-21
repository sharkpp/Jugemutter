#include "accountlist.h"
#include "twitter.h"

static const int dataStreamVersion = QDataStream::Qt_5_8;

//---------------------------------------------------------
// BaseAccount
//---------------------------------------------------------

Account::Account(QObject *parent)
    : QObject(parent)
{

}

//---------------------------------------------------------
// TwitterAccount
//---------------------------------------------------------

TwitterAccount::TwitterAccount(QObject *parent)
    : Account(parent)
    , m_twitter(new Twitter(parent))
{

}

AccountType TwitterAccount::type() const
{
    return AccountTypeTwitter;
}

const QString TwitterAccount::serialize() const
{
    QByteArray dataBytes;
    QDataStream out(&dataBytes, QIODevice::WriteOnly);
    out.setVersion(dataStreamVersion);
    // serialize
    out << m_twitter->serialize();
    //
    return dataBytes.toBase64();

}

void TwitterAccount::deserialize(const QString &data)
{
    QByteArray  dataBytes = QByteArray::fromBase64(data.toUtf8());
    QDataStream in(&dataBytes, QIODevice::ReadOnly);
    in.setVersion(dataStreamVersion);
    // deserialize
    QString  serializedOfTwitter; in >> serializedOfTwitter;
    //
    m_twitter->deserialize(serializedOfTwitter);
}

Twitter *TwitterAccount::twitter() const
{
    return m_twitter;
}

void TwitterAccount::setTwitter(Twitter *twitter)
{
    m_twitter = twitter;
}

//---------------------------------------------------------
// AccountList
//---------------------------------------------------------

AccountList::AccountList(QObject *parent)
    : QObject(parent)
{

}

void AccountList::append(Account *account)
{
    accounts.append(account);
    emit updateAccount();
}

void AccountList::remove(Account *account)
{
    int index = accounts.indexOf(account);
    removeAt(index);
}

void AccountList::removeAt(int index)
{
    if (index < 0 || accounts.size() <= index) {
        return;
    }
    accounts.removeAt(index);
    emit updateAccount();
}

int AccountList::size() const
{
    return accounts.size();
}

QList<Account *>::const_iterator AccountList::begin() const
{
    return accounts.begin();
}

QList<Account *>::const_iterator AccountList::end() const
{
    return accounts.end();
}
