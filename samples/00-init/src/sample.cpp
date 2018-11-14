#include "sample.h"

using namespace rainbow;

// sample instance
Sample sample;

Sample::Sample()
{
}

Sample::~Sample()
{
}

void Sample::onInitialize()
{
    Game::onInitialize();
}

void Sample::onFinalize()
{
    Game::onFinalize();
}

void Sample::onResize(size_t width, size_t height)
{
    Game::onResize(width, height);
}

void Sample::onUpdate(float elapsedTime)
{
    Game::onUpdate(elapsedTime);

    std::shared_ptr<Graphics> graphics = getGraphics();

    graphics->beginFrame();
    graphics->endFrame();
}
