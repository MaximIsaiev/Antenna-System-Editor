#include "antsystemliter.h"

AntSystemLiter::AntSystemLiter()
{

}

std::vector<int> AntSystemLiter::childs() const
{
    return m_childs;
}

void AntSystemLiter::setChilds(const std::vector<int> &value)
{
    m_childs = value;
}
