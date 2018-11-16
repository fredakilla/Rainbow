#include "../../../Core/Base.h"
#include "../../../Core/Game.h"
#include "../../../Core/Platform.h"
#include "../../../Scene/SceneObject.h"
#include "../../../Core/FileSystem.h"

// bgfx impl
#include "GraphicsBGFX.h"
#include "BufferBGFX.h"

#include <bgfx/platform.h>
#include <bgfx/bgfx.h>

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

    // set default view
    bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0xaa3030ff, 1.0f, 0);
    bgfx::setViewRect(0, 0, 0, uint16_t(_width), uint16_t(_height));
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


#if 0
static void getBgfxAttribute(VertexLayout::Semantic sementic, bgfx::Attrib::Enum& attrib)
{
    switch(sementic)
    {
        case VertexLayout::Semantic::ePosition:     attrib = bgfx::Attrib::Position;    break;
        case VertexLayout::Semantic::eNormal:       attrib = bgfx::Attrib::Normal;      break;
        case VertexLayout::Semantic::eColor:        attrib = bgfx::Attrib::Color0;      break;
        case VertexLayout::Semantic::eColor0:       attrib = bgfx::Attrib::Color0;      break;
        case VertexLayout::Semantic::eColor1:       attrib = bgfx::Attrib::Color1;      break;
        case VertexLayout::Semantic::eColor2:       attrib = bgfx::Attrib::Color2;      break;
        case VertexLayout::Semantic::eColor3:       attrib = bgfx::Attrib::Color3;      break;
        case VertexLayout::Semantic::eTangent:      attrib = bgfx::Attrib::Tangent;     break;
        case VertexLayout::Semantic::eBitangent:    attrib = bgfx::Attrib::Bitangent;   break;
        case VertexLayout::Semantic::eWeight:       attrib = bgfx::Attrib::Weight;      break;
        case VertexLayout::Semantic::eIndices:      attrib = bgfx::Attrib::Indices;     break;
        case VertexLayout::Semantic::eTexCoord:     attrib = bgfx::Attrib::TexCoord0;   break;
        case VertexLayout::Semantic::eTexCoord0:    attrib = bgfx::Attrib::TexCoord0;   break;
        case VertexLayout::Semantic::eTexCoord1:    attrib = bgfx::Attrib::TexCoord1;   break;
        case VertexLayout::Semantic::eTexCoord2:    attrib = bgfx::Attrib::TexCoord2;   break;
        case VertexLayout::Semantic::eTexCoord3:    attrib = bgfx::Attrib::TexCoord3;   break;
        case VertexLayout::Semantic::eTexCoord4:    attrib = bgfx::Attrib::TexCoord4;   break;
        case VertexLayout::Semantic::eTexCoord5:    attrib = bgfx::Attrib::TexCoord5;   break;
        case VertexLayout::Semantic::eTexCoord6:    attrib = bgfx::Attrib::TexCoord6;   break;
        case VertexLayout::Semantic::eTexCoord7:    attrib = bgfx::Attrib::TexCoord7;   break;

        case VertexLayout::Semantic::eColor4:
            GP_ERROR("Attribute not supported by BGFX.");
            break;

        default:
            attrib = bgfx::Attrib::Count;
    }
}

/// vertex attribute mapping from rainbow::Format to BgfxAttributeFormat
/// if type not supported by bgfx, num is set to 0 and type is set to bgfx::AttribType::Count
struct BgfxAttributeFormat
{
    bgfx::AttribType::Enum type;
    bool normalized;
    uint8_t num;
    bool asInt;
};
static BgfxAttributeFormat BGFX_ATTRIBUTE_FORMAT_MAP[] =
{
    { bgfx::AttribType::Count,  false,   0,  false   },  // eUndefined,           // undefined
    { bgfx::AttribType::Uint8,  true,    1,  true    },  // eR8Unorm,
    { bgfx::AttribType::Int16,  true,    1,  true    },  // eR16Unorm,
    { bgfx::AttribType::Half,   false,   1,  false   },  // eR16Float,            // availability depends on: `BGFX_CAPS_VERTEX_ATTRIB_HALF`.
    { bgfx::AttribType::Count,  true,    0,  false   },  // eR32Uint,             // int32 unsuported by bgfx
    { bgfx::AttribType::Float,  false,   1,  false   },  // eR32Float,
    { bgfx::AttribType::Uint8,  true,    2,  true    },  // eR8G8Unorm,
    { bgfx::AttribType::Int16,  true,    2,  true    },  // eR16G16Unorm,
    { bgfx::AttribType::Half,   false,   2,  false   },  // eR16G16Float,         // availability depends on: `BGFX_CAPS_VERTEX_ATTRIB_HALF`.
    { bgfx::AttribType::Count,  true,    0,  false   },  // eR32G32Uint,          // int32 unsuported by bgfx
    { bgfx::AttribType::Float,  false,   2,  true    },  // eR32G32Float,
    { bgfx::AttribType::Count,  true,    0,  false   },  // eR32G32B32Uint,       // int32 unsuported by bgfx
    { bgfx::AttribType::Float,  false,   3,  false   },  // eR32G32B32Float,
    { bgfx::AttribType::Uint8,  true,    4,  true    },  // eB8G8R8A8Unorm,
    { bgfx::AttribType::Uint8,  true,    4,  true    },  // eR8G8B8A8Unorm,
    { bgfx::AttribType::Int16,  true,    4,  true    },  // eR16G16B16A16Unorm,
    { bgfx::AttribType::Half,   false,   4,  false   },  // eR16G16B16A16Float,   // availability depends on: `BGFX_CAPS_VERTEX_ATTRIB_HALF`.
    { bgfx::AttribType::Count,  true,    0,  false   },  // eR32G32B32A32Uint,    // unsuported by bgfx
    { bgfx::AttribType::Float,  false,   4,  false   },  // eR32G32B32A32Float,

    // not used by vertex attributes
    { bgfx::AttribType::Count,  0, 0, 0 }, // eD16Unorm,
    { bgfx::AttribType::Count,  0, 0, 0 }, // eX8D24UnormPack32,
    { bgfx::AttribType::Count,  0, 0, 0 }, // eD32Float,
    { bgfx::AttribType::Count,  0, 0, 0 }, // eS8Uint,
    { bgfx::AttribType::Count,  0, 0, 0 }, // eD24UnormS8Uint,
    { bgfx::AttribType::Count,  0, 0, 0 }, // eD32FloatS8Uint
};

void createVertexDeclFromlayout(const VertexLayout* vertexLayout, bgfx::VertexDecl& vertexDecl)
{
    vertexDecl.begin();

    for (size_t i=0; i<vertexLayout->getAttributeCount(); ++i)
    {
        const VertexLayout::Attribute attribute = vertexLayout->getAttribute(i);

        bgfx::Attrib::Enum bgfxAttrib;
        getBgfxAttribute(attribute.semantic, bgfxAttrib);

        uint8_t index = static_cast<uint8_t>(attribute.format);
        BgfxAttributeFormat af = BGFX_ATTRIBUTE_FORMAT_MAP[index];

        vertexDecl.add(bgfxAttrib, af.num, af.type, af.normalized, af.asInt);
    }

    vertexDecl.end();
}

/*
bool GraphicsBgfx::createVertexBuffer(const VertexBufferCreateInfo* pCreateInfo,
                                      std::shared_ptr<Buffer>& pVertexBuffer)
{
    GP_ASSERT(pCreateInfo);
    GP_ASSERT(pCreateInfo->pVertexLayout);

    bgfx::VertexDecl decl;
    createVertexDeclFromlayout(pCreateInfo->pVertexLayout, decl);

    const bgfx::Memory* mem = bgfx::makeRef(pCreateInfo->pData, pCreateInfo->dataSize);

    std::shared_ptr<BufferBGFX> vertexBuffer = std::make_shared<BufferBGFX>();
    vertexBuffer->_usage = Buffer::Usage::eVertex;
    vertexBuffer->_hostVisible = pCreateInfo->hostVisible;
    vertexBuffer->_stride = pCreateInfo->pVertexLayout->getStride();
    vertexBuffer->_size = pCreateInfo->dataSize;

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
*/



#endif




struct BgfxAttributeFormat
{
    bgfx::AttribType::Enum type;
    bool normalized;
    uint8_t num;
    bool asInt;
};


/// map rainbow::VertexAttrib <=> bgfx::Attrib::Enum
bgfx::Attrib::Enum VERTEX_ATTRIB_MAP[] =
{
    // bgfx::Attrib::Enum           rainbow::VertexAttrib

    bgfx::Attrib::Enum::Position,   // Position,
    bgfx::Attrib::Enum::Normal,     // Normal,
    bgfx::Attrib::Enum::Color0,     // Color0,
    bgfx::Attrib::Enum::Color1,     // Color1,
    bgfx::Attrib::Enum::Color2,     // Color2,
    bgfx::Attrib::Enum::Color3,     // Color3,
    bgfx::Attrib::Enum::Tangent,    // Tangent,
    bgfx::Attrib::Enum::Bitangent,  // Bitangent,
    bgfx::Attrib::Enum::Indices,    // Indices,
    bgfx::Attrib::Enum::Weight,     // Weight,
    bgfx::Attrib::Enum::TexCoord0,  // TexCoord0,
    bgfx::Attrib::Enum::TexCoord1,  // TexCoord1,
    bgfx::Attrib::Enum::TexCoord2,  // TexCoord2,
    bgfx::Attrib::Enum::TexCoord3,  // TexCoord3,
    bgfx::Attrib::Enum::TexCoord4,  // TexCoord4,
    bgfx::Attrib::Enum::TexCoord5,  // TexCoord5,
    bgfx::Attrib::Enum::TexCoord6,  // TexCoord6,
    bgfx::Attrib::Enum::TexCoord7,  // TexCoord7,
};

/// map rainbow::VertexFormat <=> BgfxAttributeFormat
static BgfxAttributeFormat VERTEX_FORMAT_MAP[] =
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
    { bgfx::AttribType::Uint10,     true ,   4,  true   },  // UInt10_2N,- 4-component packed, normalized 10-bit XYZ, 2-bit W (0.0 .. 1.0)
};


void createVertexDeclFromlayout2(const rainbow::VertexLayout* vertexLayout, bgfx::VertexDecl& vertexDecl)
{
    vertexDecl.begin();

    for (size_t i=0; i<vertexLayout->getComponentCount(); ++i)
    {
        const rainbow::VertexLayout::Component component = vertexLayout->getComponent(i);

        bgfx::Attrib::Enum bgfxAttrib = VERTEX_ATTRIB_MAP[component.attr];

        uint8_t index = static_cast<uint8_t>(component.format);
        BgfxAttributeFormat af = VERTEX_FORMAT_MAP[index];

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
    createVertexDeclFromlayout2(pCreateInfo->pVertexLayout, decl);

    size_t dataSize = pCreateInfo->pVertexLayout->byteSize() * pCreateInfo->vertexCount; //pCreateInfo->dataSize;
    //size_t dataSize2 = pCreateInfo->dataSize;

    const bgfx::Memory* mem = bgfx::makeRef(pCreateInfo->pData, dataSize);

    std::shared_ptr<BufferBGFX> vertexBuffer = std::make_shared<BufferBGFX>();
    vertexBuffer->_usage = Buffer::Usage::eVertex;
    vertexBuffer->_hostVisible = pCreateInfo->hostVisible;
    vertexBuffer->_stride = pCreateInfo->pVertexLayout->byteSize();
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






















void GraphicsBgfx::cmdBindVertexBuffer(std::shared_ptr<Buffer> vertexBuffer)
{
    GP_ASSERT(vertexBuffer != nullptr);

    std::shared_ptr<BufferBGFX> bufferBGFX = std::static_pointer_cast<BufferBGFX>(vertexBuffer);

    GP_ASSERT(bufferBGFX->_usage == Buffer::Usage::eVertex);
    GP_ASSERT(bgfx::isValid(bufferBGFX->_staticVertexBufferHandle));

    bgfx::setVertexBuffer(0, bufferBGFX->_staticVertexBufferHandle);
}



}
