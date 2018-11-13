#include "App.h"

namespace rainbow
{

App app;

App::App() :
    _vertShader(nullptr),
    _fragShader(nullptr),
    _uniformBuffer(nullptr),
    _vertexBuffer(nullptr),
    _indexBuffer(nullptr),
    _texture(nullptr),
    _sampler(nullptr),
    _descriptorSet(nullptr),
    _renderPipeline(nullptr),
    _renderPass(nullptr)
{
}

App::~App()
{
}

void App::onInitialize()
{
    Game::onInitialize();
    
    // Testing immediate mode rendering
    std::shared_ptr<Graphics> graphics = getGraphics();
    
    // Create the vertex and fragment shaders
    _vertShader = graphics->createShader("color.vert");
    _fragShader = graphics->createShader("color.frag");

    // Create the vertex layout // todo: improve stride calculation with special offset value of -1
    std::vector<VertexLayout::Attribute> attributes(2); 
    attributes[0] = VertexLayout::Attribute(VertexLayout::Semantic::ePosition,
                                            Format::eR32G32B32Float, 0, 0, 0);
    attributes[1] = VertexLayout::Attribute(VertexLayout::Semantic::eColor,
                                            Format::eR32G32B32Float, 0, 1, VertexLayout::toStride(attributes[0].format));
    VertexLayout vertexLayout(attributes.data(), attributes.size());

    // Create the vertex buffer
    std::vector<float> vertices = 
    {
        0.25f,  0.25f,  0.0f,    1.0f, 0.0f, 0.0f,
       -0.25f,  0.25f,  0.0f,    0.0f, 1.0f, 0.0f,
        0.0f,  -0.25f,  0.0f,    0.0f, 0.0f, 1.0f
    };
    size_t vertexDataSize = sizeof(float) * vertices.size();
    size_t vertexStride = sizeof(float) * 7;
    _vertexBuffer = graphics->createVertexBuffer(vertexDataSize, vertexStride, false, vertices.data());

    // Gets the initial render pass.
    _renderPass = graphics->getRenderPass();

    // Create the render pipeline
    RasterizerState rasterizerState;
    ColorBlendState colorBlendState;
    DepthStencilState depthStencilState;
    _renderPipeline = graphics->createRenderPipeline(RenderPipeline::PrimitiveTopology::eTriangleList, vertexLayout,
                                                     rasterizerState, colorBlendState, depthStencilState, 
                                                     _renderPass, _descriptorSet, 
                                                    _vertShader, nullptr, nullptr, nullptr, _fragShader);
}

void App::onFinalize()
{
    std::shared_ptr<Graphics> graphics = getGraphics();
    graphics->destroyRenderPipeline(_renderPipeline);
    graphics->destroyBuffer(_vertexBuffer); 
    graphics->destroyShader(_vertShader);
    graphics->destroyShader(_fragShader);

    Game::onFinalize();
}

void App::onResize(size_t width, size_t height)
{
    Game::onResize(width, height);

    // TODO:
}

void App::onUpdate(float elapsedTime)
{
    Game::onUpdate(elapsedTime);

    std::shared_ptr<Graphics> graphics = getGraphics();

    _renderPass = graphics->acquireNextFrame();
    
    std::shared_ptr<CommandBuffer> commandBuffer = graphics->beginCommands();
    graphics->cmdSetViewport(commandBuffer, 0, 0, getWidth(), getHeight(), 0.0f, 1.0f);
    graphics->cmdSetScissor(commandBuffer, 0, 0, getWidth(), getHeight());
    graphics->cmdBeginRenderPass(commandBuffer, _renderPass);
    graphics->cmdClearColor(commandBuffer, 0.0f, 0.0f, 0.0f, 1.0f, 0);
    graphics->cmdClearDepthStencil(commandBuffer, 1.0, 0);
    graphics->cmdBindRenderPipeline(commandBuffer, _renderPipeline);
    graphics->cmdBindVertexBuffer(commandBuffer, _vertexBuffer);
    graphics->cmdDraw(commandBuffer, 3, 0);
    graphics->cmdEndRenderPass(commandBuffer);
    graphics->endCommands();
    
    graphics->submit(commandBuffer, graphics->getSemaphorePresentComplete(), graphics->getSemaphoreRenderComplete());

    graphics->presentFrame(graphics->getSemaphoreRenderComplete());
}

}
