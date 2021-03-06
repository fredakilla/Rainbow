#include "../Core/Base.h"
#include "../Scene/SceneLoader.h"

namespace rainbow
{

SceneLoader::SceneLoader() 
{
}

SceneLoader::~SceneLoader()
{
}

SceneLoader* SceneLoader::getSceneLoader()
{
    static SceneLoader loader;
    return &loader;
}

void SceneLoader::getDependencies(std::string url, std::vector<std::shared_ptr<Asset>> dependencies)
{
}

std::shared_ptr<Asset> SceneLoader::load(std::string url, std::shared_ptr<AssetLoader::Parameters> parameters)
{
    return nullptr;
}

}
