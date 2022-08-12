#pragma once

#include <QAbstractListModel>
#include <memory>
#include <antsystemtopology.h>
#include <iostream>
#include <memory.h>
#include "antsystemui_export.h"

namespace dfpost {

class ANTSYSTEMUI_EXPORT AntSystemNameModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit AntSystemNameModel(QObject *parent = nullptr);

    enum AntSystemNameRole{
        NameRole = Qt::UserRole + 1,
    };
    Q_ENUM(AntSystemNameRole)
    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    // Editable:
    bool setData(const QModelIndex &index, const QVariant &value,
                 int role = Qt::EditRole) override;

    Qt::ItemFlags flags(const QModelIndex& index) const override;

    // Remove data:
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;

    QHash<int, QByteArray> roleNames() const override;

    void setVectorAntSystemName(const std::vector<AntSystemTopology> &value);

    // Add data:
    bool addData(const AntSystemTopology &value);

private:
    std::vector<AntSystemTopology> m_nameAntennaSystem;
};

}
