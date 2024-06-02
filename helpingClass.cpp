#include "helpingClass.h"
#include<cstring>

bool Helper::IsValidName(const std::string name)
{
    for (int i = 0; i < name.size(); i++)
    {
        if(name[i]==' ')
        {
            return false;
        }
    }
    return true;
}
void Helper::moveElemToDest(std::vector<State*>&destination,std::vector<State*>&&source)
{
    for (int i = 0; i < source.size(); i++)
    {
        destination.push_back(source[i]);
    }

}
