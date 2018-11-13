#include "Base.h"
#include "Mesh.h"
#include "FileSystem.h"

namespace rainbow
{

Mesh::Mesh()
{
}

Mesh::~Mesh()
{
}

std::shared_ptr<Serializable> Mesh::createObject()
{
    return std::shared_ptr<Mesh>();
}

std::string Mesh::getClassName()
{
    return "rainbow::Mesh";
}

void Mesh::onSerialize(Serializer* serializer)
{
}

void Mesh::onDeserialize(Serializer* serializer)
{
}

}
