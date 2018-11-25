#include <rainbow.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <gli/gli.hpp>
#include <imgui/imgui.h>

/**
 * In this example :
 * - using 3 render pipelines to rendering same quad mesh into 3 viewports
 * - 1st pipeline use color shaders
 * - 2nd pipeline use texture shaders
 * - 3nd pipeline use color shaders but custom states
 * - sharing same descriptor set to render each pipeline
 */
class PipelinesSample : public Game
{
private:

    // vertex layout used in this example
    struct Vertex
    {
        float position[3];
        float color[3];
        float uv[2];
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

    std::shared_ptr<Buffer> _uniformBufferFs;
    std::shared_ptr<Buffer> _uniformBufferVs;
    std::shared_ptr<Buffer> _vertexBuffer;
    std::shared_ptr<Buffer> _indexBuffer;
    std::shared_ptr<Texture> _texture;
    std::shared_ptr<Sampler> _sampler;
    std::shared_ptr<DescriptorSet> _descriptorSet;
    std::shared_ptr<RenderPass> _renderPass;

    std::shared_ptr<RenderPipeline> _pipelineTextured;
    std::shared_ptr<RenderPipeline> _pipelineColored;
    std::shared_ptr<RenderPipeline> _pipelineWireframe;
    std::shared_ptr<Shader> _fragShaderTextured;
    std::shared_ptr<Shader> _vertShaderTextured;
    std::shared_ptr<Shader> _fragShaderColored;
    std::shared_ptr<Shader> _vertShaderColored;

public:

    //! Constructor
    PipelinesSample() :
        Game(),
        _uniformBufferFs(nullptr),
        _uniformBufferVs(nullptr),
        _vertexBuffer(nullptr),
        _indexBuffer(nullptr),
        _texture(nullptr),
        _sampler(nullptr),
        _descriptorSet(nullptr),
        _renderPass(nullptr),
        _pipelineTextured(nullptr),
        _pipelineColored(nullptr),
        _pipelineWireframe(nullptr),
        _fragShaderTextured(nullptr),
        _vertShaderTextured(nullptr),
        _fragShaderColored(nullptr),
        _vertShaderColored(nullptr)
    {
    }

    //! Destructor
    ~PipelinesSample()
    {
    }

    //! @see Game::onInitialize
    void onInitialize()
    {
        Platform::getPlatform()->setWindowTitle("Pipelines");
        Game::onInitialize();

        std::shared_ptr<Graphics> graphics = getGraphics();


        // Create the vertex layout
        std::vector<VertexLayout::Attribute> attributes(3);
        uint32_t offset = 0;
        attributes[0] = VertexLayout::Attribute(VertexLayout::Semantic::ePosition, VertexLayout::VertexFormat::eFloat3, 0, 0, offset);
        offset += VertexLayout::toStride(attributes[0].format);
        attributes[1] = VertexLayout::Attribute(VertexLayout::Semantic::eColor, VertexLayout::VertexFormat::eFloat3, 0, 1, offset);
        offset += VertexLayout::toStride(attributes[1].format);
        attributes[2] = VertexLayout::Attribute(VertexLayout::Semantic::eTexCoord, VertexLayout::VertexFormat::eFloat2, 0, 2, offset);
        offset += VertexLayout::toStride(attributes[2].format);

        VertexLayout vertexLayout(attributes.data(), attributes.size());

        // Create the vertex buffer
        std::vector<Vertex> vertices =
        {
            { {  1.0f,  1.0f, 0.0f },   { 1.0f, 0.0f, 0.0f },   { 1.0f, 1.0f }  },
            { { -1.0f,  1.0f, 0.0f },   { 0.0f, 1.0f, 0.0f },   { 0.0f, 1.0f }  },
            { {  1.0f, -1.0f, 0.0f },   { 0.0f, 0.0f, 1.0f },   { 1.0f, 0.0f }  },
            { { -1.0f, -1.0f, 0.0f },   { 1.0f, 0.0f, 1.0f },   { 0.0f, 0.0f }  }
        };
        _vertexBuffer = graphics->createVertexBuffer(vertices.size(), vertices.data(), vertexLayout, false);

        // Create the index buffer
        std::vector<uint16_t> indices =
        {
            0,1,2,
            1,3,2
        };
        _indexBuffer = graphics->createIndexBuffer(indices.size(), sizeof(uint16_t), indices.data(), false);

        // Gets the initial render pass.
        _renderPass = graphics->getRenderPass();



        // load texture using GLI - todo: impl and use the AssetLoader
        std::string imageUrl = "crate01_color_height_rgba.ktx";
        if(!FileSystem::fileExists(imageUrl))
        {
            GP_ERROR("File does not exists %s", imageUrl.c_str());
            return;
        }
        gli::texture2d tex2D(gli::load(imageUrl.c_str()));
        GP_ASSERT(!tex2D.empty());

        size_t width, height;
        size_t mipLevels;
        size_t layerCount;
        width = static_cast<size_t>(tex2D[0].extent().x);
        height = static_cast<size_t>(tex2D[0].extent().y);
        mipLevels = static_cast<size_t>(tex2D.levels());
        layerCount = static_cast<uint32_t>(tex2D.layers());

        // create vulkan texture using gli::texture2d
        _texture = graphics->createTexture2d(tex2D);




        // Create the uniform buffer for the fragment shader
        glm::vec4 color = glm::vec4(0.9f, 0.5f, 0.3f, 1.0f);
        _uniformBufferFs = graphics->createUniformBuffer(sizeof(_uboFS), true, &color);

        // Create the uniform buffer for the vertex shader (with no data, it will be updated in update method)
        _uniformBufferVs =  graphics->createUniformBuffer(sizeof(_uboVS), true, nullptr);



        // Create a shared descriptor set with 3 descriptors
        std::vector<DescriptorSet::Descriptor> descriptors(3);

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

        // texture for fragment shader - binding 2
        descriptors[2].type = DescriptorSet::Descriptor::Type::eTexture;
        descriptors[2].binding = 2;
        descriptors[2].count = 1;
        descriptors[2].shaderStages = DescriptorSet::Descriptor::ShaderStages::eFrag;
        descriptors[2].textures[0] = _texture;

        // Create descriptor set
        _descriptorSet = graphics->createDescriptorSet(descriptors.data(), descriptors.size());


        // Create shared states with default values
        RasterizerState rasterizerState;
        ColorBlendState colorBlendState;
        DepthStencilState depthStencilState;


        // create 1st pipeline
        {
            // Create the vertex and fragment shaders
            if(graphics->getApi() == Graphics::eVULKAN)
            {
                _vertShaderTextured = graphics->createShader("texture.vert");
                _fragShaderTextured = graphics->createShader("texture.frag");
            }
            else
            {
                _vertShaderTextured = graphics->createShader("assets/shaders/bgfx/texture.vert");
                _fragShaderTextured = graphics->createShader("assets/shaders/bgfx/texture.frag");
            }

            // Create the render pipeline
            _pipelineTextured = graphics->createRenderPipeline(RenderPipeline::PrimitiveTopology::eTriangleList, vertexLayout,
                                                             rasterizerState, colorBlendState, depthStencilState,
                                                             _renderPass, _descriptorSet,
                                                             _vertShaderTextured, nullptr, nullptr, nullptr, _fragShaderTextured);
        }

        // create 2nd pipeline
        {

            // Create the vertex and fragment shaders
            if(graphics->getApi() == Graphics::eVULKAN)
            {
                _vertShaderColored = graphics->createShader("color.vert");
                _fragShaderColored = graphics->createShader("color.frag");
            }
            else
            {
                _vertShaderColored = graphics->createShader("assets/shaders/bgfx/color.vert");
                _fragShaderColored = graphics->createShader("assets/shaders/bgfx/color.frag");
            }

            // Create default states
            RasterizerState rasterizerState;
            ColorBlendState colorBlendState;
            DepthStencilState depthStencilState;

            // Create the render pipeline
            _pipelineColored = graphics->createRenderPipeline(RenderPipeline::PrimitiveTopology::eTriangleList, vertexLayout,
                                                             rasterizerState, colorBlendState, depthStencilState,
                                                             _renderPass, _descriptorSet,
                                                             _vertShaderColored, nullptr, nullptr, nullptr, _fragShaderColored);

        }

        // create 3rd pipeline, using color shaders but with custom states
        {
            // cull back side and set wireframe
            //rasterizerState.cullMode = RasterizerState::CullMode::eBack;
            rasterizerState.fillMode = RasterizerState::FillMode::eWireframe;

            // Create the render pipeline
            _pipelineWireframe = graphics->createRenderPipeline(RenderPipeline::PrimitiveTopology::eTriangleList, vertexLayout,
                                                             rasterizerState, colorBlendState, depthStencilState,
                                                             _renderPass, _descriptorSet,
                                                             _vertShaderColored, nullptr, nullptr, nullptr, _fragShaderColored);
        }

    }


    //! @see Game::onFinalize
    void onFinalize()
    {
        std::shared_ptr<Graphics> graphics = getGraphics();
        graphics->destroyDescriptorSet(_descriptorSet);
        graphics->destroyBuffer(_vertexBuffer);
        graphics->destroyBuffer(_indexBuffer);
        graphics->destroyBuffer(_uniformBufferVs);
        graphics->destroyBuffer(_uniformBufferFs);
        graphics->destroyTexture(_texture);

        graphics->destroyRenderPipeline(_pipelineTextured);
        graphics->destroyRenderPipeline(_pipelineColored);
        graphics->destroyRenderPipeline(_pipelineWireframe);
        graphics->destroyShader(_vertShaderTextured);
        graphics->destroyShader(_fragShaderTextured);
        graphics->destroyShader(_vertShaderColored);
        graphics->destroyShader(_fragShaderColored);

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


        // Create some ImGui widgets
        static float zTranslation = 0.0f;
        ImGui::SetNextWindowPos(ImVec2(10, 10));
        ImGui::SetNextWindowSize(ImVec2(0, 0), ImGuiSetCond_FirstUseEver);
        ImGui::Begin("Example");
        ImGui::Text("%.2f ms/frame (%.1d fps)", (1000.0f / Game::getFrameRate()), Game::getFrameRate());
        ImGui::SliderFloat("Translate Z", &zTranslation, 0.0f, -70.0f);
        ImGui::End();

        // update imgui
        Game::getInstance()->getGraphics()->imguiUpdate();


        // update vertex shader uniforms
        {
            float time = getAbsoluteTime();

            // Update matrices
            _uboVS.projectionMatrix = glm::perspective(glm::radians(60.0f), (float)getWidth() / (float)getHeight(), 0.1f, 256.0f);

            _uboVS.viewMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -4.0f));

            _uboVS.modelMatrix = glm::mat4(1.0f);
            _uboVS.modelMatrix = glm::translate(_uboVS.modelMatrix, glm::vec3(0.0f, 0.0f, zTranslation));
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
        //graphics->cmdSetViewport(commandBuffer, 0, 0, getWidth(), getHeight(), 0.0f, 1.0f);
        graphics->cmdSetScissor(commandBuffer, 0, 0, getWidth(), getHeight());
        graphics->cmdBeginRenderPass(commandBuffer, _renderPass);
        graphics->cmdClearColor(commandBuffer, 0.0f, 0.0f, 0.0f, 1.0f, 0);
        graphics->cmdClearDepthStencil(commandBuffer, 1.0, 0);

        uint16_t viewWidth = getWidth()/3;
        uint16_t viewHeight = getHeight();

        // viewport 1 : left
        Rectangle view1 = Rectangle(0, 0, viewWidth, viewHeight);
        graphics->cmdSetViewport(commandBuffer, view1.x, view1.y, view1.width, view1.height, 0.0f, 1.0f);
        graphics->cmdBindDescriptorSet(commandBuffer, _pipelineColored, _descriptorSet);
        graphics->cmdBindRenderPipeline(commandBuffer, _pipelineColored);
        graphics->cmdBindVertexBuffer(commandBuffer, _vertexBuffer);
        graphics->cmdBindIndexBuffer(commandBuffer, _indexBuffer);
        graphics->cmdDrawIndexed(commandBuffer, 6, 0);

        // viewport 2 : center
        Rectangle view2 = Rectangle(viewWidth, 0, viewWidth, viewHeight);
        graphics->cmdSetViewport(commandBuffer, view2.x, view2.y, view2.width, view2.height, 0.0f, 1.0f);
        graphics->cmdBindDescriptorSet(commandBuffer, _pipelineTextured, _descriptorSet);
        graphics->cmdBindRenderPipeline(commandBuffer, _pipelineTextured);
        graphics->cmdBindVertexBuffer(commandBuffer, _vertexBuffer);
        graphics->cmdBindIndexBuffer(commandBuffer, _indexBuffer);
        graphics->cmdDrawIndexed(commandBuffer, 6, 0);

        // viewport 3 : right
        Rectangle view3 = Rectangle(viewWidth * 2, 0, viewWidth, viewHeight);
        graphics->cmdSetViewport(commandBuffer, view3.x, view3.y, view3.width, view3.height, 0.0f, 1.0f);
        graphics->cmdBindDescriptorSet(commandBuffer, _pipelineWireframe, _descriptorSet);
        graphics->cmdBindRenderPipeline(commandBuffer, _pipelineWireframe);
        graphics->cmdBindVertexBuffer(commandBuffer, _vertexBuffer);
        graphics->cmdBindIndexBuffer(commandBuffer, _indexBuffer);
        graphics->cmdDrawIndexed(commandBuffer, 6, 0);

        graphics->cmdDrawImgui(commandBuffer);
        graphics->cmdEndRenderPass(commandBuffer);
        graphics->endCommands();


        graphics->submit(commandBuffer, graphics->getSemaphorePresentComplete(), graphics->getSemaphoreRenderComplete());

        graphics->presentFrame(graphics->getSemaphoreRenderComplete());
    }
};

// declare our game instance
static PipelinesSample sample;

