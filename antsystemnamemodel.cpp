#include "antsystemnamemodel.h"

namespace dfpost {

AntSystemNameModel::AntSystemNameModel(QObject *parent)
    : QAbstractListModel(parent)
{}

int AntSystemNameModel::rowCount(const QModelIndex &parent) const
{
    // For list models only the root node (an invalid parent) should return the list's size. For all
    // other (valid) parents, rowCount() should return 0 so that it does not become a tree model.
    if (parent.isValid())
        return 0;

    // FIXME: Implement me!
    return static_cast<int>(m_nameAntennaSystem.size());
}

QVariant AntSystemNameModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    // FIXME: Implement me!
    switch(role) {
    case NameRole:
        return QString::fromStdString(m_nameAntennaSystem.at(index.row()).name());

    }

    return QVariant();
}

bool AntSystemNameModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (data(index, role) != value) {
        // FIXME: Implement me!
        switch(role) {
        case NameRole:
            m_nameAntennaSystem[index.row()].setName(value.toString().toStdString());
            break;
        }
        emit dataChanged(index, index, {role});
        return true;
    }
    return false;
}

Qt::ItemFlags AntSystemNameModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return QAbstractItemModel::flags(index) | Qt::ItemIsEditable; // FIXME: Implement me!
}

bool AntSystemNameModel::removeRows(int row, int count, const QModelIndex &parent)
{
    // проверка на валидность row    
    if ((row < 0) || (row >= static_cast<int>(m_nameAntennaSystem.size())))
        return false;

    //проверка на валидность count
    if ((count <= 0) || (row + count > static_cast<int>(m_nameAntennaSystem.size())))
        return false;

    beginRemoveRows(parent, row, row + count - 1);
    // удаление определенного элемента
    for (auto it = m_nameAntennaSystem.cbegin(); it != m_nameAntennaSystem.end(); it++) {
        if (std::distance(m_nameAntennaSystem.cbegin(), it) == row) {
            m_nameAntennaSystem.erase(it);
            break;
        }
    }
    endRemoveRows();
    return true;
}

QHash<int, QByteArray> AntSystemNameModel::roleNames() const
{
    QHash<int, QByteArray> names;
    names[NameRole] = "asName";
    return names;
}

void AntSystemNameModel::setVectorAntSystemName(const std::vector<AntSystemTopology> &value)
{
    beginResetModel();
    m_nameAntennaSystem = value;
    endResetModel();
}

bool AntSystemNameModel::addData(const AntSystemTopology &value)
{
    beginInsertRows(QModelIndex(), static_cast<int>(m_nameAntennaSystem.size()), static_cast<int>(m_nameAntennaSystem.size()));
    m_nameAntennaSystem.push_back(value);
    endInsertRows();
    return true;
}

}
