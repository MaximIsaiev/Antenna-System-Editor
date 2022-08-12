#pragma once

#include <QAbstractTableModel>
#include <antsystemtopology.h>
#include "antsystemui_export.h"

namespace dfpost {

class ANTSYSTEMUI_EXPORT AntSystemElementsModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit AntSystemElementsModel(QObject *parent = nullptr);

    enum AntSystemElementsRole{
        XRole = Qt::UserRole + 1,
        YRole,
        ZRole,
        RowIndexRole
    };
    Q_ENUM(AntSystemElementsRole)
    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    //    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    // Editable:
    bool setData(const QModelIndex &index, const QVariant &value,
                 int role = Qt::EditRole) override;

    // Clear model:
    void clear();

    Qt::ItemFlags flags(const QModelIndex& index) const override;

    QHash<int, QByteArray> roleNames() const override;

    std::vector<ElementOfAntennaSystem> elements() const;
    void setElements(const std::vector<ElementOfAntennaSystem> &newElements);


private:
    std::vector<ElementOfAntennaSystem> m_elements;
};

}
