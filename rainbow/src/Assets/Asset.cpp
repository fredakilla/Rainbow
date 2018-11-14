#include "../Core/Base.h"
#include "../Assets/Asset.h"

namespace rainbow
{

Asset::Asset() 
{
}

Asset::~Asset()
{
}

std::string Asset::getUrl()
{
    return std::string();
}

void Asset::getDependencies(std::string url, std::vector<std::shared_ptr<Asset>> dependencies)
{
}

}
