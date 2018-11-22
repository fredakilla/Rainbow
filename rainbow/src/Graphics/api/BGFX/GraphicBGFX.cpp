#include "../../../Core/Base.h"
#include "../../../Core/Game.h"
#include "../../../Core/Platform.h"
#include "../../../Scene/SceneObject.h"
#include "../../../Core/FileSystem.h"

// bgfx impl
#include "GraphicsBGFX.h"
#include "GraphicsTypesBGFX.h"
#include "GraphicsBGFXUtils.h"

#include <bgfx/platform.h>
#include <bgfx/bgfx.h>
#include <brtshaderc/brtshaderc.h>

namespace rainbow
{

GraphicsBgfx::GraphicsBgfx() :
    Graphics()
{
    _api = Api::eBGFX;
}

GraphicsBgfx::~GraphicsBgfx()
{

}

bool isRendererTypeSupported(bgfx::RendererType::Enum requestRendererType)
{
    if(requestRendererType == bgfx::RendererType::Count)
        return true;

    bgfx::RendererType::Enum supportedTypes[bgfx::RendererType::Count];
    uint8_t count =  bgfx::getSupportedRenderers(bgfx::RendererType::Count, supportedTypes);

    for(int i=0; i<count; ++i)
    {
        if(supportedTypes[i] == requestRendererType)
            return true;
    }

    return false;
}

bool GraphicsBgfx::createDevice()
{
    bgfx::RendererType::Enum rendererType = bgfx::RendererType::OpenGL;

    if (!isRendererTypeSupported(rendererType))
    {
        // requested renderer is not supported.
        // fallback to default bgfx renderer for current platform.
        rendererType = bgfx::RendererType::Count;
    }

    bgfx::PlatformData pd;
    pd.ndt          = (void*)Platform::getPlatform()->getNativeConnection();
    pd.nwh          = (void*)(uintptr_t)Platform::getPlatform()->getNativeWindow();
    pd.context      = NULL;
    pd.backBuffer   = NULL;
    pd.backBufferDS = NULL;
    bgfx::setPlatformData(pd);

    bgfx::Init init;
    init.type = rendererType;
    if (!bgfx::init(init))
        return false;

    _resetFlags = BGFX_RESET_VSYNC;
    bgfx::reset(_width, _height, _resetFlags);

    _debugFlags = BGFX_DEBUG_TEXT | BGFX_DEBUG_STATS;
    bgfx::setDebug(_debugFlags);

    return true;
}

void GraphicsBgfx::initialize()
{
    Graphics::initialize();

    if (!createDevice())
        exit(-1);

    createCommandBuffers();
}

void GraphicsBgfx::finalize()
{
    bgfx::shutdown();
}

void GraphicsBgfx::resize(size_t width, size_t height)
{
    Graphics::resize(width, height);
    bgfx::reset(width, height, _resetFlags);
}

/*void GraphicsBgfx::render(float elapsedTime)
{
    Game* game = Game::getInstance();

    // TODO: Render scene...
}*/

void GraphicsBgfx::createInstance()
{

}

void GraphicsBgfx::createCommandBuffers()
{
    std::shared_ptr<CommandBufferBGFX> commandBuffer = std::make_shared<CommandBufferBGFX>();
    _commandBuffer = commandBuffer;
}

static void createVertexDeclFromlayout(const rainbow::VertexLayout* vertexLayout, bgfx::VertexDecl& vertexDecl)
{
    vertexDecl.begin();

    for (size_t i=0; i<vertexLayout->getAttributeCount(); ++i)
    {
        const rainbow::VertexLayout::Attribute attribute = vertexLayout->getAttribute(i);

        uint32_t semanticEnumIndex = static_cast<uint32_t>(attribute.semantic);
        bgfx::Attrib::Enum bgfxAttrib = LOOKUP_BGFX_VERTEX_ATTRIB[semanticEnumIndex];

        uint8_t index = static_cast<uint8_t>(attribute.format);
        BgfxAttributeFormat af = LOOKUP_BGFX_ATTRIB_TYPE[index];

        vertexDecl.add(bgfxAttrib, af.num, af.type, af.normalized, af.asInt);
    }

    vertexDecl.end();
}

std::shared_ptr<RenderPass> GraphicsBgfx::getRenderPass()
{
    return nullptr;
}

std::shared_ptr<RenderPass> GraphicsBgfx::acquireNextFrame()
{
    // This dummy draw call is here to make sure that view 0 is cleared
    // if no other draw calls are submitted to view 0.
    bgfx::touch(0);

    return nullptr;
}

void GraphicsBgfx::presentFrame(std::shared_ptr<Semaphore> /*waitSemaphore*/)
{
    bgfx::frame();
}

std::shared_ptr<CommandBuffer> GraphicsBgfx::beginCommands()
{
    std::shared_ptr<CommandBufferBGFX> commandBuffer = std::static_pointer_cast<CommandBufferBGFX>(_commandBuffer);
    commandBuffer->reset();
    return _commandBuffer;
}

void GraphicsBgfx::endCommands()
{

}

std::shared_ptr<Semaphore> GraphicsBgfx::createSemaphore()
{
    return nullptr;
}

void GraphicsBgfx::destroySemaphore(std::shared_ptr<Semaphore> semaphore)
{

}

std::shared_ptr<Semaphore> GraphicsBgfx::getSemaphorePresentComplete()
{
    return nullptr;
}

std::shared_ptr<Semaphore> GraphicsBgfx::getSemaphoreRenderComplete()
{
    return nullptr;
}

void GraphicsBgfx::submit(std::shared_ptr<CommandBuffer> commandBuffer,
            std::shared_ptr<Semaphore> /*waitSemaphore*/,
            std::shared_ptr<Semaphore> /*signalSemaphore*/)
{
    std::shared_ptr<CommandBufferBGFX> commandBufferBGFX = std::static_pointer_cast<CommandBufferBGFX>(commandBuffer);

    bgfx::setViewRect(0, 0, 0, _width, _height);
    bgfx::setViewClear(0
                    , commandBufferBGFX->_clearFlags
                    , commandBufferBGFX->_clearColor
                    , commandBufferBGFX->_depth
                    , commandBufferBGFX->_stencil
    ); 

    bgfx::setState(commandBufferBGFX->_state);
    bgfx::submit(0, commandBufferBGFX->_program);
}

void GraphicsBgfx::cmdBeginRenderPass(std::shared_ptr<CommandBuffer> commandBuffer,
                        std::shared_ptr<RenderPass> renderPass){}

void GraphicsBgfx::cmdEndRenderPass(std::shared_ptr<CommandBuffer> commandBuffer){}


void GraphicsBgfx::cmdSetViewport(std::shared_ptr<CommandBuffer> commandBuffer,
                    float x, float y, float width, float height,
                    float depthMin, float depthMax){}

void GraphicsBgfx::cmdSetScissor(std::shared_ptr<CommandBuffer> commandBuffer,
                   size_t x, size_t y, size_t width, size_t height){}


void GraphicsBgfx::cmdBindRenderPipeline(std::shared_ptr<CommandBuffer> commandBuffer,
                           std::shared_ptr<RenderPipeline> renderPipeline)
{
    std::shared_ptr<CommandBufferBGFX> commandBufferBGFX = std::static_pointer_cast<CommandBufferBGFX>(commandBuffer);
    std::shared_ptr<RenderPipelineBGFX> renderPipelineBGFX = std::static_pointer_cast<RenderPipelineBGFX>(renderPipeline);

    // bind pipeline stuff to command buffer

    commandBufferBGFX->_program = renderPipelineBGFX->_program;
    commandBufferBGFX->_state = renderPipelineBGFX->_state;
}


void GraphicsBgfx::_bindUniform(const DescriptorSet::Descriptor& descriptor)
{
    for (size_t i=0; i<descriptor.count; ++i)
    {
        std::shared_ptr<UniformBGFX> uniformBuffer = std::static_pointer_cast<UniformBGFX>(descriptor.uniforms[i]);

        GP_ASSERT(uniformBuffer->_usage == Buffer::Usage::eUniform);
        GP_ASSERT(bgfx::isValid(uniformBuffer->_handle));
        GP_ASSERT(uniformBuffer->_memBuffer.size() > 0);
        GP_ASSERT(uniformBuffer->_memBuffer.data() != nullptr);
        GP_ASSERT(uniformBuffer->_num > 0);

        const void* pdata = uniformBuffer->_memBuffer.data();
        bgfx::setUniform(uniformBuffer->_handle, pdata, uniformBuffer->_num);
    }
}

void GraphicsBgfx::cmdBindDescriptorSet(std::shared_ptr<CommandBuffer> commandBuffer,
                                        std::shared_ptr<RenderPipeline> renderPipeline,
                                        std::shared_ptr<DescriptorSet> descriptorSet)
{
    GP_ASSERT(descriptorSet);

    for (size_t i=0; i<descriptorSet->getDescriptorCount(); ++i)
    {
        DescriptorSet::Descriptor descriptor = descriptorSet->getDescriptor(i);

        switch (descriptor.type)
        {
        case DescriptorSet::Descriptor::Type::eUniform:
            _bindUniform(descriptor);
            break;

        case DescriptorSet::Descriptor::Type::eSampler:
        case DescriptorSet::Descriptor::Type::eTexture:
        case DescriptorSet::Descriptor::Type::eUndefined:
        default:
            break;
        }
    }
}

void GraphicsBgfx::cmdBindVertexBuffer(std::shared_ptr<CommandBuffer> commandBuffer,
                         std::shared_ptr<Buffer> vertexBuffer)
{
    GP_ASSERT(vertexBuffer != nullptr);

    std::shared_ptr<BufferBGFX> bufferBgfx = std::static_pointer_cast<BufferBGFX>(vertexBuffer);
    GP_ASSERT(bufferBgfx->_usage == Buffer::Usage::eVertex);
    GP_ASSERT(bgfx::isValid(bufferBgfx->_staticVertexBufferHandle));

    bgfx::setVertexBuffer(0, bufferBgfx->_staticVertexBufferHandle);
}

void GraphicsBgfx::cmdBindVertexBuffers(std::shared_ptr<CommandBuffer> commandBuffer,
                          std::vector<std::shared_ptr<Buffer>> vertexBuffers){}


void GraphicsBgfx::cmdBindIndexBuffer(std::shared_ptr<CommandBuffer> commandBuffer,
                        std::shared_ptr<Buffer> indexBuffer){}


void GraphicsBgfx::cmdClearColor(std::shared_ptr<CommandBuffer> commandBuffer,
                   float red, float green, float blue, float alpha,
                   size_t attachmentIndex)
{
    std::shared_ptr<CommandBufferBGFX> commandBufferBGFX = std::static_pointer_cast<CommandBufferBGFX>(commandBuffer);
    commandBufferBGFX->_clearColor = Vector4(red, green, blue, alpha).toColor();
    commandBufferBGFX->_clearFlags |= BGFX_CLEAR_COLOR;
}


void GraphicsBgfx::cmdClearDepthStencil(std::shared_ptr<CommandBuffer> commandBuffer,
                          float depth, uint32_t stencil)
{
    std::shared_ptr<CommandBufferBGFX> commandBufferBGFX = std::static_pointer_cast<CommandBufferBGFX>(commandBuffer);
    commandBufferBGFX->_depth = depth;
    commandBufferBGFX->_stencil = stencil;
    commandBufferBGFX->_clearFlags |= BGFX_CLEAR_DEPTH | BGFX_CLEAR_STENCIL;
}

void GraphicsBgfx::cmdDraw(std::shared_ptr<CommandBuffer> /*commandBuffer*/,
            size_t /*vertexCount*/,
            size_t /*vertexStart*/)
{
}

void GraphicsBgfx::cmdDrawIndexed(std::shared_ptr<CommandBuffer> commandBuffer,
                    size_t indexCount,
                    size_t indexStart){}

void GraphicsBgfx::cmdTransitionImage(std::shared_ptr<CommandBuffer> commandBuffer,
                        std::shared_ptr<Texture> texture,
                        Texture::Usage usageOld,
                        Texture::Usage usageNew){}

std::shared_ptr<Buffer> GraphicsBgfx::createVertexBuffer(uint32_t vertexCount,
                                                         const void* data,
                                                         const VertexLayout& layout,
                                                         bool hostVisible)
{

    bgfx::VertexDecl decl;
    createVertexDeclFromlayout(&layout, decl);

    //size_t dataSize = size;
    size_t dataSize = layout.getStride() * vertexCount;

    //const bgfx::Memory* mem = bgfx::makeRef(data, dataSize);
    const bgfx::Memory* mem = bgfx::copy(data, dataSize);

    std::shared_ptr<BufferBGFX> vertexBuffer = std::make_shared<BufferBGFX>();
    vertexBuffer->_usage = Buffer::Usage::eVertex;
    vertexBuffer->_hostVisible = hostVisible;
    vertexBuffer->_stride = layout.getStride();
    vertexBuffer->_size = dataSize;
    vertexBuffer->_mem = mem;

    //if (pCreateInfo->dynamic)
    {
        // todo: create dynamic vertex buffer
    }
    //else
    {
        uint16_t flags = BGFX_BUFFER_NONE;
        bgfx::VertexBufferHandle vbh = bgfx::createVertexBuffer(mem, decl, flags);
        vertexBuffer->_staticVertexBufferHandle = vbh;
    }

    return vertexBuffer;

}

std::shared_ptr<Buffer> GraphicsBgfx::createIndexBuffer(uint32_t indexCount,
                                          size_t indexStride,
                                          const void* data,
                                          bool hostVisible)
{
    return nullptr;
}

std::shared_ptr<Buffer> GraphicsBgfx::createUniformBuffer(size_t size,
                                            bool hostVisible,
                                            const void* data)
{

    bgfx::UniformHandle uniformHandle = bgfx::createUniform("u_color", bgfx::UniformType::Vec4, 1);

    std::shared_ptr<UniformBGFX> uniformBuffer = std::make_shared<UniformBGFX>();
    uniformBuffer->_usage = Buffer::Usage::eUniform;
    uniformBuffer->_handle = uniformHandle;
    uniformBuffer->_num = 1;
    uniformBuffer->_memBuffer.assign(data, size);
    uniformBuffer->_size = size;
    uniformBuffer->_hostVisible = hostVisible;
    return uniformBuffer;
}

void GraphicsBgfx::destroyBuffer(std::shared_ptr<Buffer> buffer)
{
    std::shared_ptr<BufferBGFX> bufferBgfx = std::static_pointer_cast<BufferBGFX>(buffer);

    switch (bufferBgfx->_usage)
    {
    case Buffer::Usage::eVertex:
        bgfx::destroy(bufferBgfx->_staticVertexBufferHandle);
        break;
    case Buffer::Usage::eIndex:
        bgfx::destroy(bufferBgfx->_staticIndexBufferHandle);
        break;
    case Buffer::Usage::eUniform:
        bgfx::destroy(bufferBgfx->_uiformHandle);
        break;
    }

    buffer.reset();
}

std::shared_ptr<Texture> GraphicsBgfx::createTexture1d(size_t width,
                                         PixelFormat pixelFormat,
                                         Texture::Usage usage,
                                         Texture::SampleCount sampleCount,
                                         bool hostVisible,
                                         const void* data)
{
    return nullptr;
}

std::shared_ptr<Texture> GraphicsBgfx::createTexture2d(size_t width, size_t height, size_t mipLevels,
                                         PixelFormat pixelFormat,
                                         Texture::Usage usage,
                                         Texture::SampleCount sampleCount,
                                         bool hostVisible,
                                         const void* data)
{
    return nullptr;
}

std::shared_ptr<Texture> GraphicsBgfx::createTexture3d(size_t width, size_t height, size_t depth,
                                         PixelFormat pixelFormat,
                                         Texture::Usage usage,
                                         Texture::SampleCount sampleCount,
                                         bool hostVisible,
                                         const void* data)
{
    return nullptr;
}

void GraphicsBgfx::destroyTexture(std::shared_ptr<Texture> texture)
{

}

std::shared_ptr<RenderPass> GraphicsBgfx::createRenderPass(size_t width, size_t height,
                                             size_t colorAttachmentCount,
                                             std::vector<PixelFormat> colorFormats,
                                             PixelFormat depthStencilFormat,
                                             Texture::SampleCount sampleCount)
{
    return nullptr;
}

void GraphicsBgfx::destroyRenderPass(std::shared_ptr<RenderPass> renderPass)
{

}

std::shared_ptr<Sampler> GraphicsBgfx::createSampler(Sampler::Filter filterMin,
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
                                       float lodMipBias)
{
    return nullptr;
}

void GraphicsBgfx::destroySampler(std::shared_ptr<Sampler> sampler)
{

}

std::shared_ptr<Shader> GraphicsBgfx::createShader(const std::string& url)
{
    std::string extension = FileSystem::getExtension(url);

    bgfx::ShaderHandle shaderHandle = BGFX_INVALID_HANDLE;
    if (extension == "VERT")
    {
        const bgfx::Memory* memVsh =  shaderc::compileShader(shaderc::ST_VERTEX, url.c_str());
        shaderHandle = bgfx::createShader(memVsh);
    }
    else if (extension == "FRAG")
    {
        const bgfx::Memory* memFsh =  shaderc::compileShader(shaderc::ST_FRAGMENT, url.c_str());
        shaderHandle = bgfx::createShader(memFsh);
    }

    GP_ASSERT(bgfx::isValid(shaderHandle));

    std::shared_ptr<ShaderBGFX> shader = std::make_shared<ShaderBGFX>();
    shader->_shaderHandle = shaderHandle;
    return shader;
}


void GraphicsBgfx::destroyShader(std::shared_ptr<Shader> shader)
{
    std::shared_ptr<ShaderBGFX> shaderBgfx = std::static_pointer_cast<ShaderBGFX>(shader);
    bgfx::destroy(shaderBgfx->_shaderHandle);
    shader.reset();
}

std::shared_ptr<DescriptorSet> GraphicsBgfx::createDescriptorSet(const DescriptorSet::Descriptor* descriptors,
                                                                 size_t descriptorCount)
{
    // create a new descriptor set
    std::shared_ptr<DescriptorSet> descriptorSet = std::make_shared<DescriptorSet>();
    descriptorSet->_descriptors.resize(descriptorCount);

    // copy all given descriptors into the new descriptor set
    for (size_t i = 0; i < descriptorCount; ++i)
    {
        memcpy(&descriptorSet->_descriptors[i], &descriptors[i], sizeof(DescriptorSet::Descriptor));
    }

    return descriptorSet;
}

void GraphicsBgfx::destroyDescriptorSet(std::shared_ptr<DescriptorSet> descriptorSet)
{

}

std::shared_ptr<RenderPipeline> GraphicsBgfx::createRenderPipeline(RenderPipeline::PrimitiveTopology primitiveTopology,
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
                                                     std::shared_ptr<Shader> fragShader)
{
    std::shared_ptr<RenderPipelineBGFX> renderPipelineBGFX = std::make_shared<RenderPipelineBGFX>();



    uint64_t state = 0L;

    // Topology
    switch (primitiveTopology)
    {
    case RenderPipeline::PrimitiveTopology::ePointList:
        state |= BGFX_STATE_PT_POINTS;
        break;
    case RenderPipeline::PrimitiveTopology::eLineList:
        state |= BGFX_STATE_PT_LINES;
        break;
    case RenderPipeline::PrimitiveTopology::eLineStrip:
        state |= BGFX_STATE_PT_LINESTRIP;
        break;
    case RenderPipeline::PrimitiveTopology::eTriangleStrip:
        state |= BGFX_STATE_PT_TRISTRIP;
        break;

    case RenderPipeline::PrimitiveTopology::eTriangleList:
    default:
        // default bgfx topology
        break;
    }

    // RasterizerState

    switch (rasterizerState.cullMode)
    {
    case RasterizerState::CullMode::eBack:
        state |= BGFX_STATE_CULL_CW;
        break;
    case RasterizerState::CullMode::eFront:
        state |= BGFX_STATE_CULL_CCW;
        break;
    case RasterizerState::CullMode::eNone:
    default:
        break;
    }


    // ColorBlendState

    if (colorBlendState.blendEnabled)
    {
        uint32_t srcColorIndex = static_cast<uint32_t>(colorBlendState.colorBlendSrc);
        uint32_t dstColorIndex = static_cast<uint32_t>(colorBlendState.colorBlendDst);
        uint32_t srcAlphaIndex = static_cast<uint32_t>(colorBlendState.alphaBlendSrc);
        uint32_t dstAlphaIndex = static_cast<uint32_t>(colorBlendState.alphaBlendDst);
        uint64_t srcColor = LOOKUP_BGFX_BLEND_FACTOR[srcColorIndex];
        uint64_t dstColor = LOOKUP_BGFX_BLEND_FACTOR[dstColorIndex];
        uint64_t srcAlpha = LOOKUP_BGFX_BLEND_FACTOR[srcAlphaIndex];
        uint64_t dstAlpha = LOOKUP_BGFX_BLEND_FACTOR[dstAlphaIndex];
        state |= BGFX_STATE_BLEND_FUNC_SEPARATE(srcColor, dstColor, srcAlpha, dstAlpha);

        uint32_t colorBlendOpIndex = static_cast<uint32_t>(colorBlendState.colorBlendOp);
        uint32_t alphaBlendOpIndex = static_cast<uint32_t>(colorBlendState.alphaBlendOp);
        uint64_t equaColor = LOOKUP_BGFX_BLEND_OP[colorBlendOpIndex];
        uint64_t equaAlpha = LOOKUP_BGFX_BLEND_OP[alphaBlendOpIndex];
        state |= BGFX_STATE_BLEND_EQUATION_SEPARATE(equaColor, equaAlpha);
    }

    ColorBlendState::WriteMask writeMask = static_cast<ColorBlendState::WriteMask>(colorBlendState.colorWriteMask);
    if (bool(writeMask & ColorBlendState::WriteMask::eRed))
        state |= BGFX_STATE_WRITE_R;
    if (bool(writeMask & ColorBlendState::WriteMask::eGreen))
        state |= BGFX_STATE_WRITE_G;
    if (bool(writeMask & ColorBlendState::WriteMask::eBlue))
        state |= BGFX_STATE_WRITE_B;
    if (bool(writeMask & ColorBlendState::WriteMask::eAlpha))
        state |= BGFX_STATE_WRITE_A;



    // DepthStencilState

    if(depthStencilState.depthWrite)
    {
        state |= BGFX_STATE_WRITE_Z;
    }

    if(depthStencilState.depthEnabled)
    {
        uint32_t depthTestIndex = static_cast<uint32_t>(depthStencilState.depthFunc);
        state |= LOOKUP_BGFX_DEPTH_TEST[depthTestIndex];
    }



    // compile shaders and create program

    /*const bgfx::Memory* memVsh =  shaderc::compileShader(shaderc::ST_VERTEX, "color_bgfx.vert");
    bgfx::ShaderHandle vsh = bgfx::createShader(memVsh);

    const bgfx::Memory* memFsh =  shaderc::compileShader(shaderc::ST_FRAGMENT, "color_bgfx.frag");
    bgfx::ShaderHandle fsh = bgfx::createShader(memFsh);*/

    bgfx::ProgramHandle program = BGFX_INVALID_HANDLE;

    if (vertShader != nullptr && fragShader != nullptr)
    {
        bgfx::ShaderHandle vsh = std::static_pointer_cast<ShaderBGFX>(vertShader)->_shaderHandle;
        bgfx::ShaderHandle fsh = std::static_pointer_cast<ShaderBGFX>(fragShader)->_shaderHandle;

        program = bgfx::createProgram(vsh, fsh, false);
    }

    GP_ASSERT(bgfx::isValid(program));

    //renderPipelineBGFX->_descriptorSet = descriptorSet;
    renderPipelineBGFX->_program = program;
    renderPipelineBGFX->_state = state;
    return renderPipelineBGFX;
}

void GraphicsBgfx::destroyRenderPipeline(std::shared_ptr<RenderPipeline> renderPipeline)
{
    std::shared_ptr<RenderPipelineBGFX> renderPipelineBgfx = std::static_pointer_cast<RenderPipelineBGFX>(renderPipeline);
    bgfx::destroy(renderPipelineBgfx->_program);
    renderPipeline.reset();
}

}
