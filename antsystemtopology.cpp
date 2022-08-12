#include "antsystemtopology.h"
namespace dfpost {

std::vector<ElementOfAntennaSystem> AntSystemTopology::element() const
{
    return m_elements;
}

void AntSystemTopology::setElements(const std::vector<ElementOfAntennaSystem> &newElements)
{
    m_elements = newElements;
}

uint32_t AntSystemTopology::id() const
{
    return m_id;
}

void AntSystemTopology::setId(uint32_t newId)
{
    m_id = newId;
}

std::string AntSystemTopology::name() const
{
    return m_name;
}

void AntSystemTopology::setName(const std::string &newName)
{
    m_name = newName;
}

float AntSystemTopology::pitch() const
{
    return m_pitch;
}

void AntSystemTopology::setPitch(float newPitch)
{
    m_pitch = newPitch;
}

float AntSystemTopology::roll() const
{
    return m_roll;
}

void AntSystemTopology::setRoll(float newRoll)
{
    m_roll = newRoll;
}

Frequency AntSystemTopology::begFreq() const
{
    return m_begFreq;
}

void AntSystemTopology::setBegFreq(const Frequency &newBegFreq)
{
    m_begFreq = newBegFreq;
}

Frequency AntSystemTopology::endFreq() const
{
    return m_endFreq;
}

void AntSystemTopology::setEndFreq(const Frequency &newEndFreq)
{
    m_endFreq = newEndFreq;
}

float AntSystemTopology::yaw() const
{
    return m_yaw;
}

void AntSystemTopology::setYaw(float newYaw)
{
    m_yaw = newYaw;
}

float AntSystemTopology::getMinAz() const
{
    return minAz;
}

void AntSystemTopology::setMinAz(float newMinAz)
{
    minAz = newMinAz;
}

float AntSystemTopology::getMaxAz() const
{
    return maxAz;
}

void AntSystemTopology::setMaxAz(float newMaxAz)
{
    maxAz = newMaxAz;
}

float AntSystemTopology::getMinAng() const
{
    return minAng;
}

void AntSystemTopology::setMinAng(float newMinAng)
{
    minAng = newMinAng;
}

float AntSystemTopology::getMaxAng() const
{
    return maxAng;
}

void AntSystemTopology::setMaxAng(float newMaxAng)
{
    maxAng = newMaxAng;
}

bool AntSystemTopology::isLiter() const
{
    return m_isLiter;
}

void AntSystemTopology::setIsLiter(bool isLiter)
{
    m_isLiter = isLiter;
}

std::vector<unsigned int> AntSystemTopology::childs() const
{
    return m_childs;
}

void AntSystemTopology::setChilds(const std::vector<unsigned int> &childs)
{
    m_childs = childs;
}

}


