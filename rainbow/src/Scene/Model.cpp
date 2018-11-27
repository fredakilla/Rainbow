#include "../Scene/Model.h"

namespace rainbow
{

Model::Model() :
    Component(),
    _mesh(nullptr)
{
}

Model::~Model()
{
}

void Model::setMesh(std::shared_ptr<Mesh> mesh)
{
    _mesh = mesh;
}

std::shared_ptr<Mesh> Model::getMesh() const
{
    return _mesh;
}

std::shared_ptr<Serializable> Model::createObject()
{
    return std::static_pointer_cast<Serializable>(std::make_shared<Model>());
}

std::string Model::getClassName()
{
    return "rainbow::Model";
}

void Model::onSerialize(Serializer* serializer)
{
}

void Model::onDeserialize(Serializer* serializer)
{
}

}
