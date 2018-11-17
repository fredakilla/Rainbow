#ifdef RB_DEF_API_BGFX
#pragma once

#include "../../../Core/Base.h"
#include "../../../Graphics/Graphics.h"

namespace rainbow
{

/**
 * Defines direct hardware access to gpu functionality
 * for graphics, compute and transfer.
 */
class GraphicsBgfx : public Graphics
{

public:

    //! Constructor.
    GraphicsBgfx();

    //! Destructor.
    ~GraphicsBgfx();

    void initialize() override;
    void finalize() override;
    void resize(size_t width, size_t height) override;
    void render(float elapsedTime) override;
    void createInstance() override;
    void beginFrame() override;
    void endFrame() override;

    bool createVertexBuffer(const VertexBufferCreateInfo* pCreateInfo, std::shared_ptr<Buffer>& pVertexBuffer) override;


    std::shared_ptr<RenderPipeline> createRenderPipeline(RenderPipeline::PrimitiveTopology primitiveTopology,
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
                                                                 std::shared_ptr<Shader> fragShader) override;


    void cmdBindVertexBuffer(std::shared_ptr<Buffer> vertexBuffer) override;

private:
    bool createDevice();
    std::shared_ptr<Buffer> createBuffer(Buffer::Usage usage, size_t size, size_t stride, bool hostVisible, bool is32bit);

private:    
    uint32_t _resetFlags;   // bgfx reset flags
    uint32_t _debugFlags;   // bgfx debug flags
};

}

#endif // RB_DEF_API_BGFX
