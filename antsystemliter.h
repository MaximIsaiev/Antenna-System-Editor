#pragma once
#include "customantsystemtopology.h"


class AntSystemLiter : public CustomAntsystemTopology
{
public:
    AntSystemLiter();

    std::vector<int> childs() const;
    void setChilds(const std::vector<int> &value);
private:

    std::vector<int> m_childs;
};

