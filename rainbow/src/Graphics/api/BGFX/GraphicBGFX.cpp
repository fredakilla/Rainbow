#include "../../../Core/Base.h"
#include "../../../Core/Game.h"
#include "../../../Core/Platform.h"
#include "../../../Scene/SceneObject.h"
#include "../../../Core/FileSystem.h"

// bgfx impl
#include "GraphicsBGFX.h"
#include "GraphicsTypesBGFX.h"

#include <bgfx/platform.h>
#include <bgfx/bgfx.h>
#include <brtshaderc/brtshaderc.h>

namespace rainbow
{

GraphicsBgfx::GraphicsBgfx() :
    Graphics()
{
    print("Create GraphicsBgfx\n");
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


    // set default view
    //bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0xaa3030ff, 1.0f, 0);
    //bgfx::setViewRect(0, 0, 0, uint16_t(_width), uint16_t(_height));
}

void GraphicsBgfx::finalize()
{
    bgfx::shutdown();
}

void GraphicsBgfx::resize(size_t width, size_t height)
{
    Graphics::initialize();

    bgfx::reset(width, height, _resetFlags);
}

void GraphicsBgfx::render(float elapsedTime)
{
    Game* game = Game::getInstance();

    // TODO: Render scene...
}

void GraphicsBgfx::createInstance()
{

}

void GraphicsBgfx::createCommandBuffers()
{
    std::shared_ptr<CommandBufferBGFX> commandBuffer = std::make_shared<CommandBufferBGFX>();
    _commandBuffer = commandBuffer;
}













/*
void GraphicsBgfx::beginFrame()
{
    // This dummy draw call is here to make sure that view 0 is cleared
    // if no other draw calls are submitted to view 0.
    bgfx::touch(0);
}

void GraphicsBgfx::endFrame()
{
    bgfx::frame();
}
*/








/// map rainbow::VertexLayout::Semantic <=> bgfx::Attrib::Enum
bgfx::Attrib::Enum LOOKUP_BGFX_VERTEX_ATTRIB[] =
{
    bgfx::Attrib::Enum::Position,   //  ePosition,
    bgfx::Attrib::Enum::Normal,     //  eNormal,
    bgfx::Attrib::Enum::Color0,     //  eColor,
    bgfx::Attrib::Enum::Color0,     //  eColor0,
    bgfx::Attrib::Enum::Color1,     //  eColor1,
    bgfx::Attrib::Enum::Color2,     //  eColor2,
    bgfx::Attrib::Enum::Color3,     //  eColor3,
    bgfx::Attrib::Enum::Tangent,    //  eTangent,
    bgfx::Attrib::Enum::Bitangent,  //  eBitangent,
    bgfx::Attrib::Enum::TexCoord0,  //  eTexCoord,
    bgfx::Attrib::Enum::TexCoord0,  //  eTexCoord0,
    bgfx::Attrib::Enum::TexCoord1,  //  eTexCoord1,
    bgfx::Attrib::Enum::TexCoord2,  //  eTexCoord2,
    bgfx::Attrib::Enum::TexCoord3,  //  eTexCoord3,
    bgfx::Attrib::Enum::TexCoord4,  //  eTexCoord4,
    bgfx::Attrib::Enum::TexCoord5,  //  eTexCoord5,
    bgfx::Attrib::Enum::TexCoord6,  //  eTexCoord6,
    bgfx::Attrib::Enum::TexCoord7,  //  eTexCoord7
};


struct BgfxAttributeFormat
{
    bgfx::AttribType::Enum type;
    bool normalized;
    uint8_t num;
    bool asInt;
};

/// map rainbow::VertexLayout::VertexFormat <=> BgfxAttributeFormat
static BgfxAttributeFormat LOOKUP_BGFX_ATTRIB_TYPE[] =
{
    //        AttribType        normalized  num  asInt

    { bgfx::AttribType::Float,      false,   1,  false  },  // Float,    - single component float, expanded to (x, 0, 0, 1)
    { bgfx::AttribType::Float,      false,   2,  false  },  // Float2,   - 2-component float, expanded to (x, y, 0, 1)
    { bgfx::AttribType::Float,      false,   3,  false  },  // Float3,   - 3-component float, expanded to (x, y, z, 1)
    { bgfx::AttribType::Float,      false,   4,  false  },  // Float4,   - 4-component float
    { bgfx::AttribType::Uint8,      false,   4,  true   },  // Byte4,    - 4-component float (-128.0f..+127.0f) mapped to byte (-128..+127)
    { bgfx::AttribType::Uint8,      true ,   4,  true   },  // Byte4N,   - 4-component float (-1.0f..+1.0f) mapped to byte (-128..+127)
    { bgfx::AttribType::Uint8,      false,   4,  true   },  // UByte4,   - 4-component float (0.0f..255.0f) mapped to byte (0..255)
    { bgfx::AttribType::Uint8,      true ,   4,  true   },  // UByte4N,  - 4-component float (0.0f..+1.0) mapped to byte (-128..+127)
    { bgfx::AttribType::Int16,      false,   2,  true   },  // Short2,   - 2-component float (-32768.0f..+32767.0f) mapped to short (-32768..+32767)
    { bgfx::AttribType::Int16,      true ,   2,  true   },  // Short2N,  - 2-component float (-1.0f..+1.0f) mapped to short (-32768..+32767)
    { bgfx::AttribType::Int16,      false,   4,  true   },  // Short4,   - 4-component float (-32768.0f..+32767.0f) mapped to short (-32768..+32767)
    { bgfx::AttribType::Int16,      true ,   4,  true   },  // Short4N,  - 4-component float (-1.0f..+1.0f) mapped to short (-32768..+32767)
};

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


bool GraphicsBgfx::createVertexBuffer(const VertexBufferCreateInfo* pCreateInfo,
                                      std::shared_ptr<Buffer>& pVertexBuffer)
{
    GP_ASSERT(pCreateInfo);
    GP_ASSERT(pCreateInfo->pVertexLayout);

    bgfx::VertexDecl decl;
    createVertexDeclFromlayout(pCreateInfo->pVertexLayout, decl);

    size_t dataSize = pCreateInfo->pVertexLayout->getStride() * pCreateInfo->vertexCount;
    const bgfx::Memory* mem = bgfx::makeRef(pCreateInfo->pData, dataSize);

    std::shared_ptr<BufferBGFX> vertexBuffer = std::make_shared<BufferBGFX>();
    vertexBuffer->_usage = Buffer::Usage::eVertex;
    vertexBuffer->_hostVisible = pCreateInfo->hostVisible;
    vertexBuffer->_stride = pCreateInfo->pVertexLayout->getStride();
    vertexBuffer->_size = dataSize;

    if (pCreateInfo->dynamic)
    {
        // todo: create dynamic vertex buffer
    }
    else
    {
        uint16_t flags = BGFX_BUFFER_NONE;
        bgfx::VertexBufferHandle vbh = bgfx::createVertexBuffer(mem, decl, flags);
        vertexBuffer->_staticVertexBufferHandle = vbh;
    }

    pVertexBuffer = vertexBuffer;
    return true;
}


/*
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

    uint64_t bgfxState = 0;

    // Topology
    switch (primitiveTopology)
    {
    case RenderPipeline::PrimitiveTopology::ePointList:
        bgfxState |= BGFX_STATE_PT_POINTS;
        break;
    case RenderPipeline::PrimitiveTopology::eLineList:
        bgfxState |= BGFX_STATE_PT_LINES;
        break;
    case RenderPipeline::PrimitiveTopology::eLineStrip:
        bgfxState |= BGFX_STATE_PT_LINESTRIP;
        break;
    case RenderPipeline::PrimitiveTopology::eTriangleStrip:
        bgfxState |= BGFX_STATE_PT_TRISTRIP;
        break;

    case RenderPipeline::PrimitiveTopology::eTriangleList:
    default:
        // default bgfx topology
        break;
    }


    // RasterizerState
    switch(rasterizerState.cullMode)
    {
    case RasterizerState::CullMode::eBack:
        bgfxState |= BGFX_STATE_CULL_CW;
        break;
    case RasterizerState::CullMode::eFront:
        bgfxState |= BGFX_STATE_CULL_CCW;
        break;
    case RasterizerState::CullMode::eNone:
    default:
        break;
    }


}

*/

/*
void GraphicsBgfx::cmdBindVertexBuffer(std::shared_ptr<Buffer> vertexBuffer)
{
    GP_ASSERT(vertexBuffer != nullptr);

    std::shared_ptr<BufferBGFX> bufferBGFX = std::static_pointer_cast<BufferBGFX>(vertexBuffer);

    GP_ASSERT(bufferBGFX->_usage == Buffer::Usage::eVertex);
    GP_ASSERT(bgfx::isValid(bufferBGFX->_staticVertexBufferHandle));

    bgfx::setVertexBuffer(0, bufferBGFX->_staticVertexBufferHandle);
}
*/


/*
void GraphicsBgfx::cmdBindVertexBuffer(std::shared_ptr<CommandBuffer> commandBuffer, std::shared_ptr<Buffer> vertexBuffer)
{
    GP_UNUSED(commandBuffer);
    GP_ASSERT(vertexBuffer != nullptr);

    std::shared_ptr<BufferBGFX> bufferBGFX = std::static_pointer_cast<BufferBGFX>(vertexBuffer);

    GP_ASSERT(bufferBGFX->_usage == Buffer::Usage::eVertex);
    GP_ASSERT(bgfx::isValid(bufferBGFX->_staticVertexBufferHandle));

    bgfx::setVertexBuffer(0, bufferBGFX->_staticVertexBufferHandle);
}
*/


//--------------------------------------------




std::shared_ptr<RenderPass> GraphicsBgfx::getRenderPass()
{

}

std::shared_ptr<RenderPass> GraphicsBgfx::acquireNextFrame()
{
    // This dummy draw call is here to make sure that view 0 is cleared
    // if no other draw calls are submitted to view 0.
    bgfx::touch(0);

    return nullptr;
}

void GraphicsBgfx::presentFrame(std::shared_ptr<Semaphore> waitSemaphore)
{
    GP_UNUSED(waitSemaphore);

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
            std::shared_ptr<Semaphore> waitSemaphore,
            std::shared_ptr<Semaphore> signalSemaphore)
{
    GP_UNUSED(waitSemaphore, signalSemaphore);

    std::shared_ptr<CommandBufferBGFX> commandBufferBGFX = std::static_pointer_cast<CommandBufferBGFX>(commandBuffer);



    bgfx::setViewRect(0, 0, 0, _width, _height);

    bgfx::setViewClear(0
                    , commandBufferBGFX->_clearFlags
                    , commandBufferBGFX->_clearColor
                    , commandBufferBGFX->_depth
                    , commandBufferBGFX->_stencil
    );

    /*uint64_t state = 0
            | BGFX_STATE_WRITE_RGB
            | BGFX_STATE_WRITE_A
            | BGFX_STATE_WRITE_Z
            | BGFX_STATE_DEPTH_TEST_LESS
            | BGFX_STATE_MSAA
            ;*/

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

void GraphicsBgfx::cmdBindDescriptorSet(std::shared_ptr<CommandBuffer> commandBuffer,
                          std::shared_ptr<RenderPipeline> renderPipeline,
                          std::shared_ptr<DescriptorSet> descriptorSet){}

void GraphicsBgfx::cmdBindVertexBuffer(std::shared_ptr<CommandBuffer> commandBuffer,
                         std::shared_ptr<Buffer> vertexBuffer)
{
    GP_UNUSED(commandBuffer);
    GP_ASSERT(vertexBuffer != nullptr);

    std::shared_ptr<BufferBGFX> bufferBGFX = std::static_pointer_cast<BufferBGFX>(vertexBuffer);

    GP_ASSERT(bufferBGFX->_usage == Buffer::Usage::eVertex);
    GP_ASSERT(bgfx::isValid(bufferBGFX->_staticVertexBufferHandle));

    bgfx::setVertexBuffer(0, bufferBGFX->_staticVertexBufferHandle);
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


void GraphicsBgfx::cmdDraw(std::shared_ptr<CommandBuffer> commandBuffer,
            size_t vertexCount,
            size_t vertexStart)
{
    // not used here
}


void GraphicsBgfx::cmdDrawIndexed(std::shared_ptr<CommandBuffer> commandBuffer,
                    size_t indexCount,
                    size_t indexStart){}

void GraphicsBgfx::cmdTransitionImage(std::shared_ptr<CommandBuffer> commandBuffer,
                        std::shared_ptr<Texture> texture,
                        Texture::Usage usageOld,
                        Texture::Usage usageNew){}


std::shared_ptr<Buffer> GraphicsBgfx::createVertexBuffer(size_t size,
                                           size_t vertexStride,
                                           bool hostVisible,
                                           const void* data)
{
}

std::shared_ptr<Buffer> GraphicsBgfx::createIndexBuffer(size_t size,
                                          size_t indexStride,
                                          bool hostVisible,
                                          const void* data){}

std::shared_ptr<Buffer> GraphicsBgfx::createUniformBuffer(size_t size,
                                            bool hostVisible,
                                            const void* data){}

void GraphicsBgfx::destroyBuffer(std::shared_ptr<Buffer> buffer){}


std::shared_ptr<Texture> GraphicsBgfx::createTexture1d(size_t width,
                                         PixelFormat pixelFormat,
                                         Texture::Usage usage,
                                         Texture::SampleCount sampleCount,
                                         bool hostVisible,
                                         const void* data){}

std::shared_ptr<Texture> GraphicsBgfx::createTexture2d(size_t width, size_t height, size_t mipLevels,
                                         PixelFormat pixelFormat,
                                         Texture::Usage usage,
                                         Texture::SampleCount sampleCount,
                                         bool hostVisible,
                                         const void* data){}

std::shared_ptr<Texture> GraphicsBgfx::createTexture3d(size_t width, size_t height, size_t depth,
                                         PixelFormat pixelFormat,
                                         Texture::Usage usage,
                                         Texture::SampleCount sampleCount,
                                         bool hostVisible,
                                         const void* data){}

void GraphicsBgfx::destroyTexture(std::shared_ptr<Texture> texture){}


std::shared_ptr<RenderPass> GraphicsBgfx::createRenderPass(size_t width, size_t height,
                                             size_t colorAttachmentCount,
                                             std::vector<PixelFormat> colorFormats,
                                             PixelFormat depthStencilFormat,
                                             Texture::SampleCount sampleCount){}

void GraphicsBgfx::destroyRenderPass(std::shared_ptr<RenderPass> renderPass){}


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
                                       float lodMipBias){}

void GraphicsBgfx::destroySampler(std::shared_ptr<Sampler> sampler){}


std::shared_ptr<Shader> GraphicsBgfx::createShader(const std::string& url){}


void GraphicsBgfx::destroyShader(std::shared_ptr<Shader> shader){}


std::shared_ptr<DescriptorSet> GraphicsBgfx::createDescriptorSet(const DescriptorSet::Descriptor* descriptors,
                                                   size_t descriptorCount){}


void GraphicsBgfx::destroyDescriptorSet(std::shared_ptr<DescriptorSet> descriptorSet){}


/// maps ColorBlendState::BlendFactor to BGFX_STATE_BLEND_*
static const uint64_t LOOKUP_BGFX_BLEND_FACTOR[] = {
    BGFX_STATE_BLEND_ZERO,              // eZero,
    BGFX_STATE_BLEND_ONE,               // eOne,
    BGFX_STATE_BLEND_SRC_COLOR,         // eSrcColor,
    BGFX_STATE_BLEND_INV_SRC_COLOR,     // eOneMinusSrcColor,
    BGFX_STATE_BLEND_DST_COLOR,         // eDstColor,
    BGFX_STATE_BLEND_INV_DST_COLOR,     // eOneMinusDstColor,
    BGFX_STATE_BLEND_SRC_ALPHA,         // eSrcAlpha,
    BGFX_STATE_BLEND_INV_SRC_ALPHA,     // eOneMinuseSrcAlpha,
    BGFX_STATE_BLEND_DST_ALPHA,         // eDstAlpha,
    BGFX_STATE_BLEND_INV_DST_ALPHA,     // eOneMinuseDstAlpha,
    BGFX_STATE_BLEND_SRC_ALPHA_SAT,     // eSrcAlphaSaturate,
    BGFX_STATE_BLEND_ZERO,              // eSrc1Color,              // not supported in bgfx
    BGFX_STATE_BLEND_ZERO,              // eOneMinusSrc1Color,      // not supported in bgfx
    BGFX_STATE_BLEND_ZERO,              // eSrc1Alpha,              // not supported in bgfx
    BGFX_STATE_BLEND_ZERO,              // eOneMinuseSrc1Alpha      // not supported in bgfx
    BGFX_STATE_BLEND_FACTOR,            // eBlendFactor,
    BGFX_STATE_BLEND_INV_FACTOR,        // eOneMinuseBlendFactor
};

/// maps ColorBlendState::BlendOp to BGFX_STATE_BLEND_EQUATION_*
static const uint64_t LOOKUP_BGFX_BLEND_OP[] = {
    BGFX_STATE_BLEND_EQUATION_ADD,      // eAdd,
    BGFX_STATE_BLEND_EQUATION_SUB,      // eSubtract,
    BGFX_STATE_BLEND_EQUATION_REVSUB,   // eReverseSubstact,
    BGFX_STATE_BLEND_EQUATION_MIN,      // eMin,
    BGFX_STATE_BLEND_EQUATION_MAX,      // eMax
};

/// maps Sampler::CompareFunc to BGFX_STATE_DEPTH_TEST_*
static const uint64_t LOOKUP_BGFX_DEPTH_TEST[] = {
    BGFX_STATE_DEPTH_TEST_NEVER,        // eNever,
    BGFX_STATE_DEPTH_TEST_LESS,         // eLess,
    BGFX_STATE_DEPTH_TEST_EQUAL,        // eEqual,
    BGFX_STATE_DEPTH_TEST_LEQUAL,       // eLessOrEqual,
    BGFX_STATE_DEPTH_TEST_GREATER,      // eGreater,
    BGFX_STATE_DEPTH_TEST_NOTEQUAL,     // eNotEqual,
    BGFX_STATE_DEPTH_TEST_GEQUAL,       // eGreaterOrEqual,
    BGFX_STATE_DEPTH_TEST_ALWAYS        // eAlways
};

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

    const bgfx::Memory* memVsh =  shaderc::compileShader(shaderc::ST_VERTEX, "color_bgfx.vert");
    bgfx::ShaderHandle vsh = bgfx::createShader(memVsh);

    const bgfx::Memory* memFsh =  shaderc::compileShader(shaderc::ST_FRAGMENT, "color_bgfx.frag");
    bgfx::ShaderHandle fsh = bgfx::createShader(memFsh);

    bgfx::ProgramHandle program = bgfx::createProgram(vsh, fsh, true);
    GP_ASSERT(bgfx::isValid(program));



    renderPipelineBGFX->_program = program;
    renderPipelineBGFX->_state = state;
    return renderPipelineBGFX;
}

void GraphicsBgfx::destroyRenderPipeline(std::shared_ptr<RenderPipeline> renderPipeline){}








}
