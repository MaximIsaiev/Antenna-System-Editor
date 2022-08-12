#include "antsystemfilehandling.h"
#include <QVariant>
#include <frequtils.h>

namespace dfpost {

namespace XmlAsKeyConstants {
static const auto AS_ELEMENT_ANTSYS_COORD_KEY = QLatin1String("antsys");
static const auto AS_ELEMENT_KIND_COORD_KEY = QLatin1String("kind");
static const auto AS_ELEMENT_ID_COORD_KEY = QLatin1String("id");
static const auto AS_ELEMENT_NAME_COORD_KEY = QLatin1String("Name");
static const auto AS_ELEMENT_ROLL_COORD_KEY = QLatin1String("Roll");
static const auto AS_ELEMENT_PITCH_COORD_KEY = QLatin1String("Pitch");
static const auto AS_ELEMENT_AE_COORD_KEY = QLatin1String("ae");
static const auto AS_ELEMENT_X_COORD_KEY = QLatin1String("x");
static const auto AS_ELEMENT_Y_COORD_KEY = QLatin1String("y");
static const auto AS_ELEMENT_Z_COORD_KEY = QLatin1String("z");
static const auto AS_ELEMENT_PHASE_COORD_KEY = QLatin1String("phase");
static const auto AS_ELEMENT_BEGFREQ_COORD_KEY = QLatin1String("BegFreq");
static const auto AS_ELEMENT_ENDFREQ_COORD_KEY = QLatin1String("EndFreq");
static const auto AS_ELEMENT_MINAZ_COORD_KEY = QLatin1String("MinAz");
static const auto AS_ELEMENT_MAXAZ_COORD_KEY = QLatin1String("MaxAz");
static const auto AS_ELEMENT_MINANG_COORD_KEY = QLatin1String("MinAng");
static const auto AS_ELEMENT_MAXANG_COORD_KEY = QLatin1String("MaxAng");
static const auto AS_ELEMENT_YAW_COORD_KEY = QLatin1String("Yaw");
static const auto AS_ELEMENT_IS_LITER_COORD_KEY = QLatin1String("IsLiter");
static const auto AS_ELEMENT_CHILD_COORD_KEY = QLatin1String("child");
}

AntSystemFileHandling::AntSystemFileHandling()
{}

bool AntSystemFileHandling::load(const std::string &newFilePath)
{
    QFile antSystemFile(QString::fromStdString(newFilePath));
    if (!antSystemFile.open(QFile::ReadOnly)) {
        return false;
    }
    m_Topology.clear();
    m_topologyBackup.clear();

    // локальный массив элементов одной антенной системы
    std::vector<ElementOfAntennaSystem> elements;

    QXmlStreamReader xmlReader;
    xmlReader.setDevice(&antSystemFile);
    xmlReader.readNext();

    while (!xmlReader.atEnd()) {
        if (xmlReader.isStartElement()) {

            // локальная переменная антенной системы
            AntSystemTopology kind;


            if (xmlReader.name() == XmlAsKeyConstants::AS_ELEMENT_KIND_COORD_KEY) {
                for (auto && attr : xmlReader.attributes()) {
                    if (attr.name().toString() == XmlAsKeyConstants::AS_ELEMENT_ID_COORD_KEY)
                        kind.setId(attr.value().toUInt());
                    else if (attr.name().toString() == XmlAsKeyConstants::AS_ELEMENT_NAME_COORD_KEY)
                        kind.setName(attr.value().toString().toStdString());
                    else if (attr.name().toString() == XmlAsKeyConstants::AS_ELEMENT_ROLL_COORD_KEY)
                        kind.setRoll(attr.value().toFloat());
                    else if (attr.name().toString() == XmlAsKeyConstants::AS_ELEMENT_PITCH_COORD_KEY)
                        kind.setPitch(attr.value().toFloat());
                    else if (attr.name().toString() == XmlAsKeyConstants::AS_ELEMENT_YAW_COORD_KEY)
                        kind.setYaw(attr.value().toFloat());
                    else if (attr.name().toString() == XmlAsKeyConstants::AS_ELEMENT_BEGFREQ_COORD_KEY)
                        kind.setBegFreq(strToFreq(attr.value().toString()));
                    else if (attr.name().toString() == XmlAsKeyConstants::AS_ELEMENT_ENDFREQ_COORD_KEY)
                        kind.setEndFreq(strToFreq(attr.value().toString()));
                    else if (attr.name().toString() == XmlAsKeyConstants::AS_ELEMENT_MINAZ_COORD_KEY)
                        kind.setMinAz(attr.value().toFloat());
                    else if (attr.name().toString() == XmlAsKeyConstants::AS_ELEMENT_MAXAZ_COORD_KEY)
                        kind.setMaxAz(attr.value().toFloat());
                    else if (attr.name().toString() == XmlAsKeyConstants::AS_ELEMENT_MINANG_COORD_KEY)
                        kind.setMinAng(attr.value().toFloat());
                    else if (attr.name().toString() == XmlAsKeyConstants::AS_ELEMENT_MAXANG_COORD_KEY)
                        kind.setMaxAng(attr.value().toFloat());
                    else if (attr.name().toString() == XmlAsKeyConstants::AS_ELEMENT_IS_LITER_COORD_KEY)
                        kind.setIsLiter(QVariant(attr.value().toString()).toBool());

                }
                m_Topology.push_back(kind);
                elements.clear();
            }
            if (xmlReader.name() == XmlAsKeyConstants::AS_ELEMENT_AE_COORD_KEY) {

                // элемент антенной системы
                ElementOfAntennaSystem element;

                for (auto && attr : xmlReader.attributes()) {
                    if (attr.name().toString() == XmlAsKeyConstants::AS_ELEMENT_ID_COORD_KEY)
                        element.id = attr.value().toUInt();
                    else if (attr.name().toString() == XmlAsKeyConstants::AS_ELEMENT_X_COORD_KEY)
                        element.x = attr.value().toFloat();
                    else if (attr.name().toString() == XmlAsKeyConstants::AS_ELEMENT_Y_COORD_KEY)
                        element.y = attr.value().toFloat();
                    else if (attr.name().toString() == XmlAsKeyConstants::AS_ELEMENT_Z_COORD_KEY)
                        element.z = attr.value().toFloat();
                    else if (attr.name().toString() == XmlAsKeyConstants::AS_ELEMENT_PHASE_COORD_KEY)
                        element.phase = attr.value().toFloat();
                }
                // игнорируем элемент с ID = 0
                if (element.id != 0) {
                    elements.push_back(element);
                    m_Topology.back().setElements(elements);
                }
            }

            if (xmlReader.name() == XmlAsKeyConstants::AS_ELEMENT_CHILD_COORD_KEY)  {
                for (auto && attr : xmlReader.attributes()) {
                    if (attr.name().toString() == XmlAsKeyConstants::AS_ELEMENT_ID_COORD_KEY) {
                        auto childs = m_Topology.back().childs();
                        childs.push_back(attr.value().toInt());
                        m_Topology.back().setChilds(childs);
                    }
                }
            }
        }
        xmlReader.readNext();
    }
    setTopologyBackup(m_Topology);
    antSystemFile.close();
    return true;
}
bool AntSystemFileHandling::save(const std::string &newFilePath) const
{
    QFile antSystemFile(QString::fromStdString(newFilePath));
    if (!antSystemFile.open(QFile::WriteOnly))
        return false;
    QXmlStreamWriter xmlWriter(&antSystemFile);
    xmlWriter.setAutoFormatting(true);
    xmlWriter.writeStartDocument();
    xmlWriter.writeStartElement(XmlAsKeyConstants::AS_ELEMENT_ANTSYS_COORD_KEY);
    for (auto &&topology : m_Topology) {
        xmlWriter.writeStartElement(XmlAsKeyConstants::AS_ELEMENT_KIND_COORD_KEY);
        xmlWriter.writeAttribute(XmlAsKeyConstants::AS_ELEMENT_ID_COORD_KEY, QString::number(topology.id()));
        xmlWriter.writeAttribute(XmlAsKeyConstants::AS_ELEMENT_NAME_COORD_KEY,QString::fromStdString(topology.name()));
        xmlWriter.writeAttribute(XmlAsKeyConstants::AS_ELEMENT_ROLL_COORD_KEY, QString::number(topology.roll()));
        xmlWriter.writeAttribute(XmlAsKeyConstants::AS_ELEMENT_PITCH_COORD_KEY, QString::number(topology.pitch()));
        xmlWriter.writeAttribute(XmlAsKeyConstants::AS_ELEMENT_YAW_COORD_KEY, QString::number(topology.yaw()));
        xmlWriter.writeAttribute(XmlAsKeyConstants::AS_ELEMENT_BEGFREQ_COORD_KEY, freqToStr(topology.begFreq()));
        xmlWriter.writeAttribute(XmlAsKeyConstants::AS_ELEMENT_ENDFREQ_COORD_KEY, freqToStr(topology.endFreq()));
        xmlWriter.writeAttribute(XmlAsKeyConstants::AS_ELEMENT_MINAZ_COORD_KEY, QString::number(topology.getMinAz()));
        xmlWriter.writeAttribute(XmlAsKeyConstants::AS_ELEMENT_MAXAZ_COORD_KEY, QString::number(topology.getMaxAz()));
        xmlWriter.writeAttribute(XmlAsKeyConstants::AS_ELEMENT_MINANG_COORD_KEY, QString::number(topology.getMinAng()));
        xmlWriter.writeAttribute(XmlAsKeyConstants::AS_ELEMENT_MAXANG_COORD_KEY, QString::number(topology.getMaxAng()));
        xmlWriter.writeAttribute(XmlAsKeyConstants::AS_ELEMENT_IS_LITER_COORD_KEY, QVariant(topology.isLiter()).toString());
        for (auto &&elements : topology.element()) {
            xmlWriter.writeStartElement(XmlAsKeyConstants::AS_ELEMENT_AE_COORD_KEY);
            xmlWriter.writeAttribute(XmlAsKeyConstants::AS_ELEMENT_ID_COORD_KEY, QString::number(elements.id));
            xmlWriter.writeAttribute(XmlAsKeyConstants::AS_ELEMENT_X_COORD_KEY, QString::number(elements.x));
            xmlWriter.writeAttribute(XmlAsKeyConstants::AS_ELEMENT_Y_COORD_KEY, QString::number(elements.y));
            xmlWriter.writeAttribute(XmlAsKeyConstants::AS_ELEMENT_Z_COORD_KEY, QString::number(elements.z));
            xmlWriter.writeAttribute(XmlAsKeyConstants::AS_ELEMENT_PHASE_COORD_KEY, QString::number(elements.phase));
            xmlWriter.writeEndElement();
        }
        if (topology.isLiter()) {
            for (auto  &&child : topology.childs()) {
                xmlWriter.writeStartElement(XmlAsKeyConstants::AS_ELEMENT_CHILD_COORD_KEY);
                xmlWriter.writeAttribute(XmlAsKeyConstants::AS_ELEMENT_ID_COORD_KEY, QString::number(child));
                xmlWriter.writeEndElement();
            }
        }
        xmlWriter.writeEndElement();
    }
    xmlWriter.writeEndElement();
    xmlWriter.writeEndDocument();
    antSystemFile.close();
    return true;
}

std::vector<AntSystemTopology> AntSystemFileHandling::topology() const
{
    return m_Topology;
}

void AntSystemFileHandling::setTopology(const std::vector<AntSystemTopology> &newTopology)
{
    m_Topology = newTopology;
}

std::vector<AntSystemTopology> AntSystemFileHandling::topologyBackup() const
{
    return m_topologyBackup;
}

void AntSystemFileHandling::setTopologyBackup(const std::vector<AntSystemTopology> &newTopologyBackup)
{
    m_topologyBackup = newTopologyBackup;
}

Frequency AntSystemFileHandling::strToFreq(const QString &value) const
{
    return FrequencyUtils::smartFrequencyRecognizer(value, FrequencyUnit::Hz);;
}

QString AntSystemFileHandling::freqToStr(const Frequency &value) const
{
    return FrequencyUtils::printFrequency(value, FrequencyUnit::Hz, false, 0);
}

}



