#include "../../../Core/Base.h"
#include "../../../Core/Game.h"
#include "../../../Core/Platform.h"
#include "../../../Scene/SceneObject.h"
#include "../../../Core/FileSystem.h"

#include "GraphicsBgfx.h"

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

bool GraphicsBgfx::_createDevice()
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

    if (!_createDevice())
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


}
