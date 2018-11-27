#include "../Scene/Mesh.h"
#include "../Graphics/Graphics.h"
#include "../Core/Game.h"

namespace rainbow
{

Mesh::Mesh() :
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
    GP_ASSERT(vertexLayout.getAttributeCount() > 0);
    GP_ASSERT(vertexCount > 0);
    GP_ASSERT(vertexData);

    _vertexLayout = vertexLayout;
    _vertexCount = vertexCount;
    _dynamic = dynamic;

    std::shared_ptr<Graphics> graphics = Game::getInstance()->getGraphics();

    _vertexBuffer = graphics->createVertexBuffer(vertexCount, vertexData, vertexLayout, false);
}

}
