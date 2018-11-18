#if 0

#include "sample.h"

using namespace rainbow;







#include <bgfx/bgfx.h>
#include <brtshaderc/brtshaderc.h>
bgfx::ProgramHandle m_program;
bgfx::VertexBufferHandle m_vbh;
bgfx::IndexBufferHandle m_ibh;
struct PosColorVertex
{
    float m_x;
    float m_y;
    float m_z;
    float m_r;
    float m_g;
    float m_b;

    static void init()
    {
        ms_decl
            .begin()
            .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
            .add(bgfx::Attrib::Color0,   3, bgfx::AttribType::Float, true)
            .end();
    };

    static bgfx::VertexDecl ms_decl;
};
bgfx::VertexDecl PosColorVertex::ms_decl;
static PosColorVertex s_cubeVertices[] =
{
    {   0.0f,  0.5f, 0.5f,     1.0f, 0.0f, 0.0f,   },
    {  0.5f, -0.5f, 0.5f,      0.0f, 1.0f, 0.0f ,   },
    {  -0.5f, -0.5f, 0.5f,     0.0f, 0.0f, 1.0f,   }
};
static const uint16_t s_cubeTriList[] =
{
0, 1, 2
};








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
    Platform::getPlatform()->setWindowTitle("01-Triangle");
    Game::onInitialize();
    
    // Testing immediate mode rendering
    std::shared_ptr<Graphics> graphics = getGraphics();
    
    // Create the vertex and fragment shaders
    ///_vertShader = graphics->createShader("color.vert");
    ///_fragShader = graphics->createShader("color.frag");

    // Create the vertex layout // todo: improve stride calculation with special offset value of -1
    std::vector<VertexLayout::Attribute> attributes(2);
    attributes[0] = VertexLayout::Attribute(VertexLayout::Semantic::ePosition, VertexLayout::VertexFormat::eFloat3, 0, 0, 0);
    attributes[1] = VertexLayout::Attribute(VertexLayout::Semantic::eColor, VertexLayout::VertexFormat::eFloat3, 0, 1, VertexLayout::toStride(attributes[0].format));
    VertexLayout vertexLayout(attributes.data(), attributes.size());

    // Create the vertex buffer
    static std::vector<float> vertices =
    {
        // position            // color

        0.0f,  0.5f, 0.0f,     1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.0f,     0.0f, 1.0f, 0.0f ,
       -0.5f, -0.5f, 0.0f,     0.0f, 0.0f, 1.0f,
    };
    ///size_t vertexDataSize = sizeof(float) * vertices.size();
    ///size_t vertexStride = sizeof(float) * 6;
    ///_vertexBuffer = graphics->createVertexBuffer(vertexDataSize, vertexLayout, false, vertices.data());




    //rainbow::VertexLayout vertexLayout =
    //{
    //    { rainbow::VertexAttr::Position, rainbow::VertexFormat::Float3 },
    //    { rainbow::VertexAttr::Color0, rainbow::VertexFormat::Float3 }
    //};


    Graphics::VertexBufferCreateInfo vbInfo = {};
    vbInfo.pVertexLayout = &vertexLayout;
    vbInfo.pData = vertices.data();
    vbInfo.vertexCount = 3;
    vbInfo.dynamic = false;

    graphics->createVertexBuffer(&vbInfo, _vertexBuffer);


    //

    // Create the render pipeline
    RasterizerState rasterizerState;
    ColorBlendState colorBlendState;
    DepthStencilState depthStencilState;

    _renderPipeline = graphics->createRenderPipeline(RenderPipeline::PrimitiveTopology::eTriangleList, vertexLayout,
                                                     rasterizerState, colorBlendState, depthStencilState,
                                                     _renderPass, _descriptorSet,
                                                     _vertShader, nullptr, nullptr, nullptr, _fragShader);




    /*




    Graphics::PipelineCreateInfo pipelineInfo = {};
    pipelineInfo.primitiveType = PrimitiveType::Triangles;
    pipelineInfo.vertexLayout = &vertexLayout;
    pipelineInfo.rasterizerState = rasterizerState;
    pipelineInfo.colorBlendState = colorBlendState;
    pipelineInfo.depthStencilState = depthStencilState;
    pipelineInfo.renderPass = _renderPass;
    pipelineInfo.descriptorSet = _descriptorSet;
    pipelineInfo.vertexShader = _vertShader;
    pipelineInfo.fragmentShader = _fragShader;
    graphics->createRenderPipeline(&pipelineInfo, _renderPipeline);

    Graphics::PipelineCreateInfo

*/




   /* // Gets the initial render pass.
    _renderPass = graphics->getRenderPass();

    // Create the render pipeline
    RasterizerState rasterizerState;
    ColorBlendState colorBlendState;
    DepthStencilState depthStencilState;
    _renderPipeline = graphics->createRenderPipeline(RenderPipeline::PrimitiveTopology::eTriangleList, vertexLayout,
                                                     rasterizerState, colorBlendState, depthStencilState, 
                                                     _renderPass, _descriptorSet, 
                                                    _vertShader, nullptr, nullptr, nullptr, _fragShader);*/






    ///------------

/*
    // compile vertex shader, with default arguments.
    const bgfx::Memory* memVsh =  shaderc::compileShader(shaderc::ST_VERTEX, "color.vert");
    bgfx::ShaderHandle vsh = bgfx::createShader(memVsh);

    // compile fragment shader, with specific arguments for defines, varying def file, shader profile.
    const bgfx::Memory* memFsh =  shaderc::compileShader(shaderc::ST_FRAGMENT, "color.frag");
    bgfx::ShaderHandle fsh = bgfx::createShader(memFsh);

    // build program using shaders
    m_program = bgfx::createProgram(vsh, fsh, true);



    PosColorVertex::init();
    m_vbh = bgfx::createVertexBuffer(
                // Static data can be passed with bgfx::makeRef
                  bgfx::makeRef(s_cubeVertices, sizeof(s_cubeVertices) )
                , PosColorVertex::ms_decl
    );




    m_ibh = bgfx::createIndexBuffer(
                // Static data can be passed with bgfx::makeRef
                bgfx::makeRef(s_cubeTriList, sizeof(s_cubeTriList) )
    );

*/
}

void Sample::onFinalize()
{
    std::shared_ptr<Graphics> graphics = getGraphics();
    /*graphics->destroyRenderPipeline(_renderPipeline);
    graphics->destroyBuffer(_vertexBuffer); 
    graphics->destroyShader(_vertShader);
    graphics->destroyShader(_fragShader);*/

    Game::onFinalize();
}

void Sample::onResize(size_t width, size_t height)
{
    Game::onResize(width, height);

    // TODO:
}

void Sample::onUpdate(float elapsedTime)
{
    Game::onUpdate(elapsedTime);

    std::shared_ptr<Graphics> graphics = getGraphics();



    //graphics->beginFrame();
    //graphics->cmdBindVertexBuffer(_vertexBuffer);


  /* {



        bgfx::setViewRect(0, 0, 0, graphics->getWidth(), graphics->getHeight() );
        bgfx::setViewClear(0
                        , BGFX_CLEAR_COLOR|BGFX_CLEAR_DEPTH
                        , 0x303030ff
                        , 1.0f
                        , 0
        );



        uint64_t state = 0
                | BGFX_STATE_WRITE_RGB
                | BGFX_STATE_WRITE_A
                | BGFX_STATE_WRITE_Z
                | BGFX_STATE_DEPTH_TEST_LESS
                | BGFX_STATE_MSAA
                ;

        //bgfx::setVertexBuffer(0, m_vbh);
        //bgfx::setIndexBuffer(m_ibh);
        bgfx::setState(state);
        //bgfx::submit(0, m_program);
    }

     //graphics->endFrame();
*/



    _renderPass = graphics->acquireNextFrame();

    std::shared_ptr<CommandBuffer> commandBuffer = graphics->beginCommands();
    // graphics->cmdSetViewport(commandBuffer, 0, 0, getWidth(), getHeight(), 0.0f, 1.0f);
    // graphics->cmdSetScissor(commandBuffer, 0, 0, getWidth(), getHeight());
    // graphics->cmdBeginRenderPass(commandBuffer, _renderPass);
    graphics->cmdClearColor(commandBuffer, 0.1f, 0.1f, 0.1f, 1.0f, 0);
    graphics->cmdClearDepthStencil(commandBuffer, 1.0, 0);
    graphics->cmdBindRenderPipeline(commandBuffer, _renderPipeline);
    graphics->cmdBindVertexBuffer(commandBuffer, _vertexBuffer);
    graphics->cmdDraw(commandBuffer, 3, 0);
    //graphics->cmdEndRenderPass(commandBuffer);
    graphics->endCommands();

    graphics->submit(commandBuffer, graphics->getSemaphorePresentComplete(), graphics->getSemaphoreRenderComplete());

    graphics->presentFrame(graphics->getSemaphoreRenderComplete());


}


#else



#include "sample.h"


Sample sample;

Sample::Sample() :
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

Sample::~Sample()
{
}


struct UniformBufferObject {
    Vector4 colors;
};

UniformBufferObject myBuffer;
std::shared_ptr<Buffer> uniformBuffer;



void Sample::onInitialize()
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
                                            VertexLayout::VertexFormat::eFloat3, 0, 0, 0);
    attributes[1] = VertexLayout::Attribute(VertexLayout::Semantic::eColor,
                                            VertexLayout::VertexFormat::eFloat3, 0, 1, VertexLayout::toStride(attributes[0].format));
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
    /*Graphics::VertexBufferCreateInfo vbInfo = {};
    vbInfo.pVertexLayout = &vertexLayout;
    vbInfo.pData = vertices.data();
    vbInfo.vertexCount = 3;
    vbInfo.dynamic = false;
    graphics->createVertexBuffer(&vbInfo, _vertexBuffer);*/



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

void Sample::onFinalize()
{
    std::shared_ptr<Graphics> graphics = getGraphics();
    graphics->destroyRenderPipeline(_renderPipeline);
    graphics->destroyBuffer(_vertexBuffer);
    graphics->destroyShader(_vertShader);
    graphics->destroyShader(_fragShader);

    Game::onFinalize();
}

void Sample::onResize(size_t width, size_t height)
{
    Game::onResize(width, height);

    // TODO:
}

void Sample::onUpdate(float elapsedTime)
{
    Game::onUpdate(elapsedTime);

    std::shared_ptr<Graphics> graphics = getGraphics();

    _renderPass = graphics->acquireNextFrame();

    std::shared_ptr<CommandBuffer> commandBuffer = graphics->beginCommands();
    graphics->cmdSetViewport(commandBuffer, 0, 0, getWidth(), getHeight(), 0.0f, 1.0f);
    graphics->cmdSetScissor(commandBuffer, 0, 0, getWidth(), getHeight());
    graphics->cmdBeginRenderPass(commandBuffer, _renderPass);
    graphics->cmdClearColor(commandBuffer, 0.1f, 0.1f, 0.1f, 1.0f, 0);
    graphics->cmdClearDepthStencil(commandBuffer, 1.0, 0);
    graphics->cmdBindRenderPipeline(commandBuffer, _renderPipeline);
    graphics->cmdBindVertexBuffer(commandBuffer, _vertexBuffer);
    graphics->cmdDraw(commandBuffer, 3, 0);
    graphics->cmdEndRenderPass(commandBuffer);
    graphics->endCommands();

    graphics->submit(commandBuffer, graphics->getSemaphorePresentComplete(), graphics->getSemaphoreRenderComplete());

    graphics->presentFrame(graphics->getSemaphoreRenderComplete());
}







#endif
