#pragma once
#include <QStandardItemModel>
#include <antsystemtopology.h>
#include "antsystemui_export.h"

namespace dfpost {

class ANTSYSTEMUI_EXPORT AntSystemTreeViewModel : public QStandardItemModel
{
    Q_OBJECT
public:
    enum AntennaSystemNameRole{
        AntSystemNameRole = Qt::UserRole + 1,
        AntSystemGroupRole
    };
    Q_ENUM(AntennaSystemNameRole)

    AntSystemTreeViewModel(QObject *parent = nullptr);

    void setTopology(const std::vector<AntSystemTopology> &topology);

private:
    std::vector<AntSystemTopology> m_topology;
    QStandardItem *rootItem;
};

}

