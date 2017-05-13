#ifndef ACCOUNTMODEL_H
#define ACCOUNTMODEL_H

#include <QAbstractItemModel>

class AccountModel : public QAbstractItemModel
{
public:

    enum {
        ColumnProfileImage,
        ColumnName,
        ColumnAppend, // for Last row span and centering icon
        ColumnSetting,
        ColumnRemove,
        ColumnNum,
    };

public:
    AccountModel(QObject *parent = nullptr);

    int	rowCount(const QModelIndex &parent = QModelIndex()) const;

    int columnCount(const QModelIndex &parent = QModelIndex()) const;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;

    QModelIndex	parent(const QModelIndex &index) const;

    Qt::ItemFlags flags(const QModelIndex &index) const;
};

#endif // ACCOUNTMODEL_H
