#pragma once

#include <QObject>
#include <QXmlStreamReader>
#include <QFile>
#include <antsystemtopology.h>
#include "antsystemui_export.h"
#include <frequency.h>


namespace dfpost {

class ANTSYSTEMUI_EXPORT AntSystemFileHandling
{
public:
    AntSystemFileHandling();
    bool load(const std::string &newFilePath);
    bool save(const std::string &newFilePath) const;

    std::vector<AntSystemTopology> topology() const;
    void setTopology (const std::vector<AntSystemTopology> &newTopology);

    std::vector<AntSystemTopology> topologyBackup() const ;
    void setTopologyBackup (const std::vector<AntSystemTopology> &newTopologyBackup);

private:
    Frequency strToFreq(const QString &value) const;
    QString freqToStr(const Frequency &value) const;

    std::vector<AntSystemTopology> m_Topology; // по итогу хранит в себе данные из распарсенного xml-файла
    std::vector<AntSystemTopology> m_topologyBackup;
};

}

