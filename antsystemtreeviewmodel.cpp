#include "antsystemtreeviewmodel.h"
#include <QStandardItem>

namespace dfpost {

AntSystemTreeViewModel::AntSystemTreeViewModel(QObject *parent)
    : QStandardItemModel {parent}
{
    setColumnCount(1);
    rootItem = invisibleRootItem();
    rootItem->setText("ROOT");
}

void AntSystemTreeViewModel::setTopology(const std::vector<AntSystemTopology> &topology)
{
    beginResetModel();
    bool isChild = false;
    m_topology = topology;
    setColumnCount(1);
    rootItem = invisibleRootItem();
    rootItem->setText("ROOT");
    std::vector<QStandardItem*> allChildsAndParents;

    //  сначала добавляем группы элементов
    for (auto it = m_topology.begin(); it != m_topology.end(); it++) {
        if (it->isLiter()) {
            QStandardItem *group = new QStandardItem;
            group->setText(QString::fromStdString(it->name()));
            for (auto &&childs : it->childs()) {
                for (auto jt = m_topology.begin(); jt != m_topology.end(); jt++)  {
                    if (jt->id() == childs) {
                        QStandardItem *value = new QStandardItem;
                        value->setText(QString::fromStdString(jt->name()));
                        group->appendRow(value);
                        allChildsAndParents.push_back(value);
                    }
                }
            }
            allChildsAndParents.push_back(group);
            rootItem->appendRow(group);
        }
    }

    // добавляем одиночные системы
    for (auto &&currSystem : m_topology) {

        const QString currentSystemName = QString::fromStdString(currSystem.name());
        for (auto &&child : allChildsAndParents) {
            if (currentSystemName == child->text()) {
                isChild = true;
                break;
            }
            isChild = false;
        }

        if (!isChild) {
           QStandardItem *group = new QStandardItem;
           group->setText(currentSystemName);
           rootItem->appendRow(group);
        }
    }
    endResetModel();
    emit dataChanged(QModelIndex(), QModelIndex(), {Qt::EditRole});
}

}
