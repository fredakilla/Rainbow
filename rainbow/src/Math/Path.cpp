#include "../Core/Base.h"
#include "../Math/Path.h"

namespace rainbow
{

Path::Path()
{
}

Path::~Path()
{
}

std::shared_ptr<Serializable> Path::createObject()
{
    return std::shared_ptr<Path>();
}

std::string Path::getClassName()
{
    return "rainbow::Path";
}

void Path::onSerialize(Serializer* serializer)
{

}

void Path::onDeserialize(Serializer* serializer)
{

}

}
