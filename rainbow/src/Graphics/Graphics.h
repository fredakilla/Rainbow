#pragma once

#include "../Graphics/Buffer.h"
#include "GraphicsTypes.h"

namespace rainbow
{


/**
 * Defines direct hardware access to gpu functionality
 * for graphics, compute and transfer.
 */
class Graphics
{
    friend class Game;

public:


    /**
     * Vertex buffer create info struct.
     * @param pVertexLayout The vertex buffer layout see: VertexLayout.
     * @param pData Pointer to vertices data.
     * @param vertexCount Vertex count.
     * @param hostVisible Visible to host.
     * @param dynamic Dynamic or static vertex buffer.
     */
    typedef struct VertexBufferCreateInfo {
        VertexLayout*       pVertexLayout;
        void*               pData;
        uint32_t            vertexCount;
        bool                hostVisible;
        bool                dynamic;
    } VertexLayoutCreateInfo;





public:

    /**
     * Constructor.
     */
    Graphics();

    /**
     * Destructor.
     */
    virtual ~Graphics();

    /**
     * Gets the width of the graphics sytem swap images.
     *
     * @return The width of the graphics sytem swap images.
     */
    uint32_t getWidth();

    /**
     * Gets the height of the graphics sytem swap images.
     *
     * @return The height of the graphics sytem swap images.
     */
    uint32_t getHeight();




    virtual void initialize();
    virtual void finalize() {}
    virtual void resize(size_t width, size_t height);
    virtual void render(float elapsedTime);
    virtual void createInstance() {}
    virtual void beginFrame() {}
    virtual void endFrame() {}

    /**
      * Creates a vertex buffer.
      *
      * @param pCreateInfo pointer to the VertexBufferCreateInfo struct.
      * @param vertexBuffer reference to the vertex buffer pointer.
      * @return True if success and false if fails.
      */
    virtual bool createVertexBuffer(const VertexBufferCreateInfo* pCreateInfo,
                                    std::shared_ptr<Buffer>& vertexBuffer) = 0;


    virtual void cmdBindVertexBuffer(std::shared_ptr<Buffer> vertexBuffer) = 0;


protected:
    bool _initialized;
    bool _resized;
    uint32_t _width;
    uint32_t _height;
    bool _fullscreen;
    bool _vsync;
    bool _multisampling;
    bool _validation;
};

} // end namespace rainbow
