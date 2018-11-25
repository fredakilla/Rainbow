#include <rainbow.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

/**
 * In this example :
 * - Creating and rendering a simple triangle mesh using a vertex buffer
 * - A descriptor with 2 ubo for matrices and color update
 * - setting up a render pipeline with descriptor set, shaders and states
 * - Updating ubo data.
 */
class TriangleSample : public Game
{
private:

    // vertex layout used in this example
    struct Vertex
    {
        float position[3];
        float color[3];
    };

    // uniform block layout as in vertex shader
    struct
    {
        glm::mat4 projectionMatrix;
        glm::mat4 modelMatrix;
        glm::mat4 viewMatrix;
    } _uboVS;

    // uniform block layout as in fragment shader
    struct
    {
        glm::vec4 color;
    } _uboFS;

    std::shared_ptr<Shader> _vertShader;
    std::shared_ptr<Shader> _fragShader;
    std::shared_ptr<Buffer> _uniformBufferFs;
    std::shared_ptr<Buffer> _uniformBufferVs;
    std::shared_ptr<Buffer> _vertexBuffer;
    std::shared_ptr<Buffer> _indexBuffer;
    std::shared_ptr<Texture> _texture;
    std::shared_ptr<Sampler> _sampler;
    std::shared_ptr<DescriptorSet> _descriptorSet;
    std::shared_ptr<RenderPipeline> _renderPipeline;
    std::shared_ptr<RenderPass> _renderPass;

public:

    //! Constructor
    TriangleSample() :
        Game(),
        _vertShader(nullptr),
        _fragShader(nullptr),
        _uniformBufferFs(nullptr),
        _uniformBufferVs(nullptr),
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
    ~TriangleSample()
    {
    }

    //! @see Game::onInitialize
    void onInitialize()
    {
        Platform::getPlatform()->setWindowTitle("Triangle");
        Game::onInitialize();

        // Testing immediate mode rendering
        std::shared_ptr<Graphics> graphics = getGraphics();

        // Create the vertex and fragment shaders
        if(graphics->getApi() == Graphics::eVULKAN)
        {
            _vertShader = graphics->createShader("color.vert");
            _fragShader = graphics->createShader("color.frag");
        }
        else
        {
            _vertShader = graphics->createShader("assets/shaders/bgfx/color2.vert");
            _fragShader = graphics->createShader("assets/shaders/bgfx/color2.frag");
        }

        // Create the vertex layout
        std::vector<VertexLayout::Attribute> attributes(2);
        attributes[0] = VertexLayout::Attribute(VertexLayout::Semantic::ePosition,
                                                VertexLayout::VertexFormat::eFloat3, 0, 0, 0);
        attributes[1] = VertexLayout::Attribute(VertexLayout::Semantic::eColor,
                                                VertexLayout::VertexFormat::eFloat3, 0, 1, VertexLayout::toStride(attributes[0].format));
        VertexLayout vertexLayout(attributes.data(), attributes.size());

        // Create the vertex buffer
        std::vector<Vertex> vertices =
        {
            { {  1.0f,  1.0f, 0.0f },   { 1.0f, 0.0f, 0.0f } },
            { { -1.0f,  1.0f, 0.0f },   { 0.0f, 1.0f, 0.0f } },
            { {  0.0f, -1.0f, 0.0f },   { 0.0f, 0.0f, 1.0f } }
        };

        uint32_t vertexCount = 3;
        _vertexBuffer = graphics->createVertexBuffer(vertexCount, vertices.data(), vertexLayout, false);

        // Gets the initial render pass.
        _renderPass = graphics->getRenderPass();


        // Create the uniform buffer for the fragment shader
        glm::vec4 color = glm::vec4(0.9f, 0.5f, 0.3f, 1.0f);
        _uniformBufferFs = graphics->createUniformBuffer(sizeof(_uboFS), true, &color);

        // Create the uniform buffer for the vertex shader (with no data, it will be updated in update method)
        _uniformBufferVs =  graphics->createUniformBuffer(sizeof(_uboVS), true, nullptr);


        // Create 2 descriptors for uniforms
        std::vector<DescriptorSet::Descriptor> descriptors(2);

        // uniform buffer for vertex shader - binding 0
        descriptors[0] = {};
        descriptors[0].type = DescriptorSet::Descriptor::Type::eUniform;
        descriptors[0].binding = 0;
        descriptors[0].count = 1;
        descriptors[0].shaderStages = DescriptorSet::Descriptor::ShaderStages::eVert;
        descriptors[0].uniforms[0] = _uniformBufferVs;

        // uniform buffer for fragment shader - binding 1
        descriptors[1] = {};
        descriptors[1].type = DescriptorSet::Descriptor::Type::eUniform;
        descriptors[1].binding = 1;
        descriptors[1].count = 1;
        descriptors[1].shaderStages = DescriptorSet::Descriptor::ShaderStages::eFrag;
        descriptors[1].uniforms[0] = _uniformBufferFs;

        // Create descriptor set
        _descriptorSet = graphics->createDescriptorSet(descriptors.data(), descriptors.size());


        // Create default states
        RasterizerState rasterizerState;
        ColorBlendState colorBlendState;
        DepthStencilState depthStencilState;

        // Create the render pipeline
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
        graphics->destroyDescriptorSet(_descriptorSet);
        graphics->destroyBuffer(_vertexBuffer);
        graphics->destroyBuffer(_uniformBufferVs);
        graphics->destroyBuffer(_uniformBufferFs);
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


        // update vertex shader uniforms
        {
            float time = getAbsoluteTime();

            // Update matrices
            _uboVS.projectionMatrix = glm::perspective(glm::radians(60.0f), (float)getWidth() / (float)getHeight(), 0.1f, 256.0f);

            _uboVS.viewMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -4.0f));

            _uboVS.modelMatrix = glm::mat4(1.0f);
            _uboVS.modelMatrix = glm::rotate(_uboVS.modelMatrix, glm::radians(time * 100), glm::vec3(0.0f, 1.0f, 0.0f));

            // Map uniform buffer and update it
            void* pdata = _uniformBufferVs->map(sizeof(_uboVS));
            memcpy(pdata, &_uboVS, sizeof(_uboVS));
            _uniformBufferVs->unmap();
        }

        // update fragment shader uniforms
        {
            float time = (sin(getAbsoluteTime() * 8.0f) + 1.0f) * 0.5f;
            _uboFS.color = glm::vec4(time, 0.5f, 0.3f, 1.0f);

            void* pData = _uniformBufferFs->map(sizeof(_uboFS));
            memcpy(pData, &_uboFS, sizeof(_uboFS));
            _uniformBufferFs->unmap();
        }


        // update graphics

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
};

// declare our game instance
static TriangleSample sample;

