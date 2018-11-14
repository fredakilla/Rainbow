#include "../Core/Base.h"
#include "../Graphics/Graphics.h"
#include "../Core/Game.h"
#include "../Core/Platform.h"
#include "../Scene/SceneObject.h"
#include "../Core/FileSystem.h"

namespace rainbow
{

const std::vector<const char*> __validationLayers = 
{
    "VK_LAYER_LUNARG_standard_validation"
};

Graphics::Graphics() :
    _initialized(false),
    _resized(false),
    _width(0),
    _height(0),
    _fullscreen(false),
    _vsync(false),
    _multisampling(false),
    _validation(false)
{

}

Graphics::~Graphics()
{

}

uint32_t Graphics::getWidth()
{
    return _width;
}

uint32_t Graphics::getHeight()
{
    return _height;
}

void Graphics::initialize()
{
    if (_initialized)
        return;

    std::shared_ptr<Game::Config> config = Game::getInstance()->getConfig();
    _width = config->width;
    _height = config->height;
    _fullscreen = config->fullscreen;
    _vsync = config->vsync;
    _multisampling = config->multisampling;
    _validation = config->validation;
    createInstance();
    /*createDevice();
    createSurface();
    createSwapchain();
    createRenderPasses();
    createCommandBuffers();
    createSynchronizationObjects();*/

    _initialized = true;
    _resized = true;
}

void Graphics::resize(size_t width, size_t height)
{
    if (!_resized)
        return;
    _resized = false;

    _width = width;
    _height = height;

    _resized = true;
}

void Graphics::render(float elapsedTime)
{
    Game* game = Game::getInstance();

    // TODO: Render scene...
}

void Graphics::createInstance()
{
}


}
