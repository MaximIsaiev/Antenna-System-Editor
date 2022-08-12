#pragma once

#include <vector>
#include <string>
#include <frequency.h>
#include "antsystemdefs_export.h"

namespace dfpost {

static const float NULL_FLOAT = 0.0f;
static const int NULL_INT = 0;


struct ANTSYSTEMDEFS_EXPORT ElementOfAntennaSystem
{
    uint32_t id = {NULL_INT};
    float x = {NULL_FLOAT};
    float y = {NULL_FLOAT};
    float z = {NULL_FLOAT};
    float phase = {NULL_FLOAT};
};

class ANTSYSTEMDEFS_EXPORT AntSystemTopology
{
public:

    std::vector<ElementOfAntennaSystem> element() const;
    void setElements(const std::vector<ElementOfAntennaSystem> &newElements);

    uint32_t id() const;
    void setId(uint32_t newId);

    std::string name() const;
    void setName(const std::string &newName);

    float pitch() const;
    void setPitch(float newPitch);

    float roll() const;
    void setRoll(float newRoll);

    Frequency begFreq() const;
    void setBegFreq(const Frequency &newBegFreq);

    Frequency endFreq() const;
    void setEndFreq(const Frequency &newEndFreq);

    float yaw() const;
    void setYaw(float newYaw);

    float getMinAz() const;
    void setMinAz(float newMinAz);

    float getMaxAz() const;
    void setMaxAz(float newMaxAz);

    float getMinAng() const;
    void setMinAng(float newMinAng);

    float getMaxAng() const;
    void setMaxAng(float newMaxAng);

    bool isLiter() const;
    void setIsLiter(bool isLiter);

    std::vector<unsigned int> childs() const;
    void setChilds(const std::vector<unsigned int> &childs);

private:
    uint32_t m_id = {NULL_INT};
    float m_roll = {NULL_FLOAT};
    float m_pitch = {NULL_FLOAT};
    float m_yaw = {NULL_FLOAT};
    Frequency m_begFreq = {Frequency::Hz(0)};
    Frequency m_endFreq = {Frequency::Hz(0)};
    float minAz = {NULL_FLOAT};
    float maxAz = {NULL_FLOAT};
    float minAng = {NULL_FLOAT};
    float maxAng = {NULL_FLOAT};
    bool m_isLiter = {false};
    std::vector<unsigned int> m_childs;
    std::string m_name;
    std::vector<ElementOfAntennaSystem> m_elements;
};

}
