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
    //void beginFrame() override;
    //void endFrame() override;

    bool createVertexBuffer(const VertexBufferCreateInfo* pCreateInfo, std::shared_ptr<Buffer>& pVertexBuffer) override;

/*
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


    //void cmdBindVertexBuffer(std::shared_ptr<Buffer> vertexBuffer) override;

    void cmdBindVertexBuffer(std::shared_ptr<CommandBuffer>, std::shared_ptr<Buffer> vertexBuffer) override;
*/





    std::shared_ptr<RenderPass> getRenderPass() override;

    std::shared_ptr<RenderPass> acquireNextFrame() override;

    void presentFrame(std::shared_ptr<Semaphore> waitSemaphore = nullptr) override;

    std::shared_ptr<CommandBuffer> beginCommands() override;

    void endCommands() override;

    std::shared_ptr<Semaphore> createSemaphore() override;

    void destroySemaphore(std::shared_ptr<Semaphore> semaphore) override;

    std::shared_ptr<Semaphore> getSemaphorePresentComplete() override;

    std::shared_ptr<Semaphore> getSemaphoreRenderComplete() override;

    void submit(std::shared_ptr<CommandBuffer> commandBuffer,
                std::shared_ptr<Semaphore> waitSemaphore,
                std::shared_ptr<Semaphore> signalSemaphore) override;

    void cmdBeginRenderPass(std::shared_ptr<CommandBuffer> commandBuffer,
                            std::shared_ptr<RenderPass> renderPass) override;

    void cmdEndRenderPass(std::shared_ptr<CommandBuffer> commandBuffer) override;


    void cmdSetViewport(std::shared_ptr<CommandBuffer> commandBuffer,
                        float x, float y, float width, float height,
                        float depthMin, float depthMax) override;

    void cmdSetScissor(std::shared_ptr<CommandBuffer> commandBuffer,
                       size_t x, size_t y, size_t width, size_t height) override;


    void cmdBindRenderPipeline(std::shared_ptr<CommandBuffer> commandBuffer,
                               std::shared_ptr<RenderPipeline> renderPipeline) override;

    void cmdBindDescriptorSet(std::shared_ptr<CommandBuffer> commandBuffer,
                              std::shared_ptr<RenderPipeline> renderPipeline,
                              std::shared_ptr<DescriptorSet> descriptorSet) override;

    void cmdBindVertexBuffer(std::shared_ptr<CommandBuffer> commandBuffer,
                             std::shared_ptr<Buffer> vertexBuffer) override;

    void cmdBindVertexBuffers(std::shared_ptr<CommandBuffer> commandBuffer,
                              std::vector<std::shared_ptr<Buffer>> vertexBuffers) override;


    void cmdBindIndexBuffer(std::shared_ptr<CommandBuffer> commandBuffer,
                            std::shared_ptr<Buffer> indexBuffer) override;


    void cmdClearColor(std::shared_ptr<CommandBuffer> commandBuffer,
                       float red, float green, float blue, float alpha,
                       size_t attachmentIndex) override;


    void cmdClearDepthStencil(std::shared_ptr<CommandBuffer> commandBuffer,
                              float depth, uint32_t stencil) override;


    void cmdDraw(std::shared_ptr<CommandBuffer> commandBuffer,
                size_t vertexCount,
                size_t vertexStart) override;


    void cmdDrawIndexed(std::shared_ptr<CommandBuffer> commandBuffer,
                        size_t indexCount,
                        size_t indexStart) override;

    void cmdTransitionImage(std::shared_ptr<CommandBuffer> commandBuffer,
                            std::shared_ptr<Texture> texture,
                            Texture::Usage usageOld,
                            Texture::Usage usageNew) override;


    std::shared_ptr<Buffer> createVertexBuffer(size_t size,
                                               size_t vertexStride,
                                               bool hostVisible,
                                               const void* data,
                                               const VertexLayout& layout) override;

    std::shared_ptr<Buffer> createIndexBuffer(size_t size,
                                              size_t indexStride,
                                              bool hostVisible,
                                              const void* data) override;

    std::shared_ptr<Buffer> createUniformBuffer(size_t size,
                                                bool hostVisible,
                                                const void* data) override;

    void destroyBuffer(std::shared_ptr<Buffer> buffer) override;


    std::shared_ptr<Texture> createTexture1d(size_t width,
                                             PixelFormat pixelFormat,
                                             Texture::Usage usage,
                                             Texture::SampleCount sampleCount,
                                             bool hostVisible,
                                             const void* data) override;

    std::shared_ptr<Texture> createTexture2d(size_t width, size_t height, size_t mipLevels,
                                             PixelFormat pixelFormat,
                                             Texture::Usage usage,
                                             Texture::SampleCount sampleCount,
                                             bool hostVisible,
                                             const void* data) override;

    std::shared_ptr<Texture> createTexture3d(size_t width, size_t height, size_t depth,
                                             PixelFormat pixelFormat,
                                             Texture::Usage usage,
                                             Texture::SampleCount sampleCount,
                                             bool hostVisible,
                                             const void* data) override;

    void destroyTexture(std::shared_ptr<Texture> texture) override;


    std::shared_ptr<RenderPass> createRenderPass(size_t width, size_t height,
                                                 size_t colorAttachmentCount,
                                                 std::vector<PixelFormat> colorFormats,
                                                 PixelFormat depthStencilFormat,
                                                 Texture::SampleCount sampleCount) override;

    void destroyRenderPass(std::shared_ptr<RenderPass> renderPass) override;


    std::shared_ptr<Sampler> createSampler(Sampler::Filter filterMin,
                                           Sampler::Filter filterMag,
                                           Sampler::Filter filterMip,
                                           Sampler::AddressMode addressModeU,
                                           Sampler::AddressMode addressModeV,
                                           Sampler::AddressMode addressModeW,
                                           Sampler::BorderColor borderColor,
                                           bool compareEnabled,
                                           Sampler::CompareFunc compareFunc,
                                           bool anisotropyEnabled,
                                           float anisotropyMax,
                                           float lodMin,
                                           float lodMax,
                                           float lodMipBias) override;

    void destroySampler(std::shared_ptr<Sampler> sampler) override;


    std::shared_ptr<Shader> createShader(const std::string& url) override;


    void destroyShader(std::shared_ptr<Shader> shader) override;


    std::shared_ptr<DescriptorSet> createDescriptorSet(const DescriptorSet::Descriptor* descriptors,
                                                       size_t descriptorCount) override;


    void destroyDescriptorSet(std::shared_ptr<DescriptorSet> descriptorSet) override;


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

    void destroyRenderPipeline(std::shared_ptr<RenderPipeline> renderPipeline) override;














private:
    bool createDevice();
    void createCommandBuffers();
    //std::shared_ptr<Buffer> createBuffer(Buffer::Usage usage, size_t size, size_t stride, bool hostVisible, bool is32bit);

private:    
    uint32_t _resetFlags;   // bgfx reset flags
    uint32_t _debugFlags;   // bgfx debug flags

    std::shared_ptr<CommandBuffer> _commandBuffer;
};

}

