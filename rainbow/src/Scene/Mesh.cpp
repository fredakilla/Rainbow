#include "../Scene/Mesh.h"
#include "../Graphics/Graphics.h"
#include "../Core/Game.h"

namespace rainbow
{

Mesh::Mesh() :
    Component(),
    _vertexCount(0),
    _dynamic(false),
    _primitiveType(PrimitiveTopology::eTriangleList),
    _vertexBuffer(nullptr)
{
}

Mesh::~Mesh()
{
    std::shared_ptr<Graphics> graphics = Game::getInstance()->getGraphics();
    if (_vertexBuffer)
        graphics->destroyBuffer(_vertexBuffer);
}

void Mesh::set(const VertexLayout& vertexLayout, uint32_t vertexCount, const void* vertexData, bool dynamic)
{
    _vertexLayout = vertexLayout;
    _vertexCount = vertexCount;
    _dynamic = dynamic;

    std::shared_ptr<Graphics> graphics = Game::getInstance()->getGraphics();

    _vertexBuffer = graphics->createVertexBuffer(vertexCount, vertexData, vertexLayout, false);
}

std::shared_ptr<Mesh> Mesh::createMesh(const VertexLayout& vertexLayout, uint32_t vertexCount, const void* vertexData, bool dynamic)
{
    GP_ASSERT(vertexCount > 0);

    std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>();
    mesh->set(vertexLayout, vertexCount, vertexData, dynamic);
    return mesh;
}

std::shared_ptr<Serializable> Mesh::createObject()
{
    return std::static_pointer_cast<Serializable>(std::make_shared<Mesh>());
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
