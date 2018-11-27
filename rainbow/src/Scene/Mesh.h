#pragma once

#include "../Core/Base.h"
#include "../Scene/Component.h"
#include "../Graphics/GraphicsTypes.h"

namespace rainbow
{

class Mesh
{

public:

    /**
     * Defines supported primitive types.
     */
    enum class PrimitiveTopology
    {
        ePointList,
        eLineList,
        eLineStrip,
        eTriangleList,
        eTriangleStrip,
    };


    /**
     * Constructor.
     */
    Mesh();

    /**
     * Destructor.
     */
    ~Mesh();


    /**
     * Set a mesh with the specified vertex layout and vertices.
     *
     * @param vertexLayout The vertex layout.
     * @param vertexCount The number of vertices.
     * @param vertexData The vertices data.
     * @param dynamic true if the mesh is dynamic; false otherwise.
     *
     * @return The created mesh.
     */
    void set(const VertexLayout& vertexLayout, uint32_t vertexCount, const void* vertexData, bool dynamic);

private:

    VertexLayout _vertexLayout;
    uint32_t _vertexCount;
    bool _dynamic;
    PrimitiveTopology _primitiveType;
    std::shared_ptr<Buffer> _vertexBuffer;

};

}
