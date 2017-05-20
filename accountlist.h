#ifndef ACCOUNTLIST_H
#define ACCOUNTLIST_H

#include <QObject>

class Twitter;

typedef enum {
    AccountTypeUnknown = 0,
    AccountTypeTwitter = 1,
} AccountType;

class Account
    : public QObject
{
    Q_OBJECT
public:
    Account(QObject *parent);
    // get account type
    virtual AccountType type() const = 0;
    // get session save data
    virtual const QString serialize() const = 0;
    // set session saved data
    virtual void deserialize(const QString& data) = 0;
};

class TwitterAccount
    : public Account
{
    Q_OBJECT
public:
    TwitterAccount(QObject *parent);
    // get account type
    virtual AccountType type() const;
    // get session save data
    virtual const QString serialize() const ;
    // set session saved data
    virtual void deserialize(const QString& data);
    // Twitter
    Twitter *twitter() const;
    void setTwitter(Twitter *twitter);
private:
    Twitter *m_twitter;
};

class AccountList
        : public QObject
{
    Q_OBJECT

public:
    AccountList(QObject *parent);

    void append(Account* account);
    void remove(Account* account);
    void removeAt(int index);

    QList<Account *>::const_iterator begin() const;
    QList<Account *>::const_iterator end() const;

signals:
    void updateAccount();

protected:
    QList<Account *> accounts;
};

#endif // ACCOUNTLIST_H
