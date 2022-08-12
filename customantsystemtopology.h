#pragma once

#include <vector>
#include <string>

static const int NULL_INT = 0;
static const float NULL_FLOAT = 0;

class CustomAntsystemTopology
{
public:
    CustomAntsystemTopology();

    unsigned int id() const;
    void setId(const unsigned int newId);

    std::string name() const;
    void setName(const std::string &newName);

    std::uint64_t begFreq() const;
    void setBegFreq(const std::uint64_t newBegFreq);

    std::uint64_t endFreq() const;
    void setEndFreq(const std::uint64_t newEndFreq);

private:
    unsigned int m_id = {NULL_INT};
    std::uint64_t m_begFreq = {NULL_INT};
    std::uint64_t m_endFreq = {NULL_INT};
    std::string m_name;
};

