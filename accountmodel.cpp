#include "accountmodel.h"
#include <QIcon>
#include <QPushButton>

AccountModel::AccountModel(QObject *parent)
    : QAbstractItemModel(parent)
{

}

int AccountModel::rowCount(const QModelIndex &parent) const
{
    return !parent.isValid() ? 2 : 0;
}

int AccountModel::columnCount(const QModelIndex &parent) const
{
    return ColumnNum;
}

QVariant AccountModel::data(const QModelIndex &index, int role) const
{
    QVariant result;

    switch (role) {
    case Qt::DisplayRole:
        switch (index.column()) {
        case ColumnProfileImage:
            //result.setValue(QIcon(":/icons.black/people.svg"));
            break;
        case ColumnName:
            result.setValue(QString("Name"));
            break;
        }
        break;
    case Qt::DecorationRole:
        switch (index.column()) {
        case ColumnProfileImage:
            result.setValue(QIcon(":/icons.black/people.svg"));
            break;
        case ColumnSetting:
            result.setValue(QIcon(":/icons.black/settings.svg"));
            break;
        case ColumnRemove:
            result.setValue(QIcon(":/icons.black/delete.svg"));
            break;
        case ColumnAppend:
            if (index.row() + 1 != rowCount()) {
                break;
            }
            result.setValue(QIcon(":/icons.black/add.svg"));
            break;
        }
        break;
    /*case Qt::TextAlignmentRole:
        switch (index.column()) {
        case ColumnProfileImage:
            if (index.row() + 1 != rowCount()) {
                break;
            }
        case ColumnSetting:
        case ColumnRemove: {
            result.setValue((int)Qt::AlignHCenter);
            break; }
        }
        break;*/
    case Qt::SizeHintRole:
        switch (index.column()) {
        case ColumnProfileImage:
            if (index.row() + 1 != rowCount()) {
                break;
            }
            result.setValue(QSize(100, 48));
        }
        break;
    }

    return result;
}

QModelIndex AccountModel::index(int row, int column, const QModelIndex &parent) const
{
    if (rowCount(parent) <= row ||
        columnCount(parent) <= column) {
        return QModelIndex();
    }
    return createIndex(row, column);
}

QModelIndex AccountModel::parent(const QModelIndex &index) const
{
    return QModelIndex();
}

Qt::ItemFlags AccountModel::flags(const QModelIndex &index) const
{
    return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
}
