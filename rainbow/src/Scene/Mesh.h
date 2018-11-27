#pragma once

#include "../Core/Base.h"
#include "../Scene/Component.h"
#include "../Graphics/GraphicsTypes.h"

namespace rainbow
{

class Mesh : public Component
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
     * Constructs a new mesh with the specified vertex layout.
     *
     * @param vertexLayout The vertex layout.
     * @param vertexCount The number of vertices.
     * @param dynamic true if the mesh is dynamic; false otherwise.
     *
     * @return The created mesh.
     */
    static std::shared_ptr<Mesh> createMesh(const VertexLayout& vertexLayout, uint32_t vertexCount, const void* vertexData, bool dynamic = false);


protected:

    /**
     * @see Activator::createObject
     */
    static std::shared_ptr<Serializable> createObject();

    /**
     * @see Serializable::getClassName
     */
    std::string getClassName() override;

    /**
     * @see Serializable::onSerialize
     */
    void onSerialize(Serializer* serializer) override;

    /**
     * @see Serializable::onDeserialize
     */
    void onDeserialize(Serializer* serializer) override;

private:

    void set(const VertexLayout& vertexLayout, uint32_t vertexCount, const void* vertexData, bool dynamic);

    VertexLayout _vertexLayout;
    uint32_t _vertexCount;
    bool _dynamic;
    PrimitiveTopology _primitiveType;
    std::shared_ptr<Buffer> _vertexBuffer;

};

}
