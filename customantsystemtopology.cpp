#include "customantsystemtopology.h"

CustomAntsystemTopology::CustomAntsystemTopology()
{

}

unsigned int CustomAntsystemTopology::id() const
{
    return m_id;
}

void CustomAntsystemTopology::setId(const unsigned int newId)
{
    m_id = newId;
}

std::string CustomAntsystemTopology::name() const
{
    return m_name;
}

void CustomAntsystemTopology::setName(const std::string &newName)
{
    m_name = newName;
}

std::uint64_t CustomAntsystemTopology::begFreq() const
{
    return m_begFreq;
}

void CustomAntsystemTopology::setBegFreq(const std::uint64_t newBegFreq)
{
    m_begFreq = newBegFreq;
}

std::uint64_t CustomAntsystemTopology::endFreq() const
{
    return m_endFreq;
}

void CustomAntsystemTopology::setEndFreq(const std::uint64_t newEndFreq)
{
    m_endFreq = newEndFreq;
}
