#pragma once

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



    typedef struct PipelineCreateInfo {
        PrimitiveType::Enum       primitiveType;
        void*               pData;
        uint32_t            vertexCount;
        bool                hostVisible;
        bool                dynamic;
    } PipelineCreateInfo;



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



    /**
     * Creates a render pipeline.
     *
     * @param primitiveTopology The primitive topology for how vertices are connected.
     * @param vertexLayout The layout of vertex attributes being input into the pipeline.
     * @param rasterizerState The state of how a primitive is converted (rasterized) to a two-dimensional image.
     * @param colorBlendState The blending state across all color attachments.
     * @param depthStencilState The depth stencil state.
     * @param renderPass The render pass to used by the pipeline.
     * @param descriptorSet The resource descriptor set to be used by the pipeline.
     * @param vertShader The vertex shader.
     * @param tescShader The tessellation control shader.
     * @param teseShader The tessellation evaluation shader.
     * @param geomShader The geometry shader.
     * @param fragShader The fragment shader.
     */
    virtual std::shared_ptr<RenderPipeline> createRenderPipeline(RenderPipeline::PrimitiveTopology primitiveTopology,
                                                                 VertexLayout vertexLayout,
                                                                 RasterizerState rasterizerState,
                                                                 ColorBlendState colorBlendState,
                                                                 DepthStencilState depthStencilState,
                                                                 std::shared_ptr<RenderPass> renderPass,
                                                                 std::shared_ptr<DescriptorSet> descriptorSet,
                                                                 std::shared_ptr<Shader> vertShader,
                                                                 std::shared_ptr<Shader> tescShader,
                                                                 std::shared_ptr<Shader> teseShader,
                                                                 std::shared_ptr<Shader> geomShader,
                                                                 std::shared_ptr<Shader> fragShader) = 0;




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
