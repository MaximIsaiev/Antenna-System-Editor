#include "antsystemelementsmodel.h"

namespace dfpost {

AntSystemElementsModel::AntSystemElementsModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

int AntSystemElementsModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    // FIXME: Implement me!
    return static_cast<int>(m_elements.size());
}


QVariant AntSystemElementsModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    switch(role) {
    case XRole:
        return QString::number(m_elements.at(index.row()).x);
    case YRole:
        return QString::number(m_elements.at(index.row()).y);
    case ZRole:
        return QString::number(m_elements.at(index.row()).z);
    case RowIndexRole:
        return index.row();
    }
    // FIXME: Implement me!
    return QVariant();
}

bool AntSystemElementsModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid())
        return false;

    if (data(index, role) != value) {
        // FIXME: Implement me!
        switch (role) {
        case XRole:
            m_elements[index.row()].x = value.toFloat();
            break;
        case YRole:
            m_elements[index.row()].y = value.toFloat();
            break;
        case ZRole:
            m_elements[index.row()].z = value.toFloat();
            break;
        }
        emit dataChanged(index, index, {role});
        return true;
    }
    return false;
}

void AntSystemElementsModel::clear()
{
    beginResetModel();
    m_elements.clear();
    endResetModel();
}

Qt::ItemFlags AntSystemElementsModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return QAbstractItemModel::flags(index) | Qt::ItemIsEditable; // FIXME: Implement me!
}

QHash<int, QByteArray> AntSystemElementsModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[XRole] = "x";
    roles[YRole] = "y";
    roles[ZRole] = "z";
    roles[RowIndexRole] = "rowIndex";
    return roles;
}

std::vector<ElementOfAntennaSystem> AntSystemElementsModel::elements() const
{
    return m_elements;
}

void AntSystemElementsModel::setElements(const std::vector<ElementOfAntennaSystem> &newElements)
{
    beginResetModel();
    m_elements = newElements;
    endResetModel();
}

}
