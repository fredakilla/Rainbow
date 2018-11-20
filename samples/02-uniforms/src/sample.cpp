#include <rainbow.h>


class Sample : public Game
{
public:

    //! Constructor
    Sample() :
        Game(),
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

    //! Destructor
    ~Sample()
    {
    }

    //! @see Game::onInitialize
    void onInitialize()
    {
        Game::onInitialize();

        // Testing immediate mode rendering
        std::shared_ptr<Graphics> graphics = getGraphics();

        // Create the vertex and fragment shaders
        if(graphics->getApi() == Graphics::eVULKAN)
        {
            _vertShader = graphics->createShader("color2.vert");
            _fragShader = graphics->createShader("color2.frag");
        }
        else
        {
            _vertShader = graphics->createShader("assets/shaders/bgfx/color2.vert");
            _fragShader = graphics->createShader("assets/shaders/bgfx/color2.frag");
        }

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

        uint32_t vertexCount = 3;
        _vertexBuffer = graphics->createVertexBuffer(vertexCount, vertices.data(), vertexLayout, false);

        // Gets the initial render pass.
        _renderPass = graphics->getRenderPass();







        /*fsParams p;
        p.color = Vector4(1,1,1,1);

        ShaderParameters params;
        params.add("color", Uniform::eVec4, &p.color);
        params.add("test", Uniform::eVec4, &p.test);
        params.add("mat", Uniform::eMat4, &p.matrix);

        _uniformBuffer = graphics->createUniforms(params);*/






        // create an uniform buffer
        Vector4 fragUniformDataColor = Vector4(0.5f, 0.5f, 0.3f, 1.0f);

        size_t uniformBufferSize = sizeof(fragUniformDataColor);
        _uniformBuffer = graphics->createUniformBuffer(uniformBufferSize, false, &fragUniformDataColor);

        // supported uniform version for bgfx
        //_uniformBuffer = graphics->createUniform("u_color", UniformType::Vec4, &fragUniformDataColor, 1);


        // add a descriptor to bind uniform
        DescriptorSet::Descriptor descriptor1;
        descriptor1.type = DescriptorSet::Descriptor::Type::eUniform;
        descriptor1.count = 1;
        descriptor1.shaderStages = DescriptorSet::Descriptor::ShaderStages::eFrag;
        descriptor1.binding = 0;
        descriptor1.uniforms[0] = _uniformBuffer;

        // add descriptors list
        std::vector<DescriptorSet::Descriptor> descriptors;
        descriptors.push_back(descriptor1);
        _descriptorSet = graphics->createDescriptorSet(descriptors.data(), descriptors.size());




        // Create the render pipelineuniformBuffer
        RasterizerState rasterizerState;
        ColorBlendState colorBlendState;
        DepthStencilState depthStencilState;
        _renderPipeline = graphics->createRenderPipeline(RenderPipeline::PrimitiveTopology::eTriangleList, vertexLayout,
                                                         rasterizerState, colorBlendState, depthStencilState,
                                                         _renderPass, _descriptorSet,
                                                        _vertShader, nullptr, nullptr, nullptr, _fragShader);

    }


    //! @see Game::onFinalize
    void onFinalize()
    {
        std::shared_ptr<Graphics> graphics = getGraphics();
        graphics->destroyRenderPipeline(_renderPipeline);
        graphics->destroyBuffer(_vertexBuffer);
        graphics->destroyShader(_vertShader);
        graphics->destroyShader(_fragShader);

        Game::onFinalize();
    }

    //! @see Game::onResize
    void onResize(size_t width, size_t height)
    {
        Game::onResize(width, height);
    }

    //! @see Game::onUpdate
    void onUpdate(float elapsedTime)
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
        graphics->cmdBindDescriptorSet(commandBuffer, _renderPipeline, _descriptorSet);
        graphics->cmdBindRenderPipeline(commandBuffer, _renderPipeline);
        graphics->cmdBindVertexBuffer(commandBuffer, _vertexBuffer);
        graphics->cmdDraw(commandBuffer, 3, 0);
        graphics->cmdEndRenderPass(commandBuffer);
        graphics->endCommands();

        graphics->submit(commandBuffer, graphics->getSemaphorePresentComplete(), graphics->getSemaphoreRenderComplete());

        graphics->presentFrame(graphics->getSemaphoreRenderComplete());
    }

private:

    std::shared_ptr<Shader> _vertShader;
    std::shared_ptr<Shader> _fragShader;
    std::shared_ptr<Buffer> _uniformBuffer;
    std::shared_ptr<Buffer> _vertexBuffer;
    std::shared_ptr<Buffer> _indexBuffer;
    std::shared_ptr<Texture> _texture;
    std::shared_ptr<Sampler> _sampler;
    std::shared_ptr<DescriptorSet> _descriptorSet;
    std::shared_ptr<RenderPipeline> _renderPipeline;
    std::shared_ptr<RenderPass> _renderPass;
};

// declare game instance
static Sample sample;


/*
struct Uniform
{
    enum UniformType
    {
        eVec4,
        eMat3,
        eMat4
    };

    std::string uniformName;
    UniformType type;
    unsigned char* buffer;
};

class ShaderParameters
{
public:

    ShaderParameters()
    {}

    void add(const char* name, Uniform::UniformType type, void* data)
    {}


    std::vector<Uniform> _uniforms;
};


struct fsParams
{
    Vector4 color;
    Vector4 test;
    Matrix matrix;
};
*/
