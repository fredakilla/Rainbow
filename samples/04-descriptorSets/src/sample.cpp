#include <rainbow.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <gli/gli.hpp>
#include <imgui/imgui.h>

/**
 * In this example :
 * - using 2 descriptor sets for rendering 2 differents quads mesh
 * - with different textures and differents shaders data (UBO)
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


    struct QuadParameters
    {
        struct Matrices
        {
            glm::mat4 projectionMatrix;
            glm::mat4 modelMatrix;
            glm::mat4 viewMatrix;
        } matrices;

        glm::vec4 color;
        glm::vec3 rotation;
        std::shared_ptr<Texture> texture;
        std::shared_ptr<DescriptorSet> descriptorSet;
        std::shared_ptr<Buffer> uniformBuffer;
    };
    std::array<QuadParameters, 2> _quads;

    std::shared_ptr<Buffer> _vertexBuffer;
    std::shared_ptr<Buffer> _indexBuffer;
    std::shared_ptr<RenderPass> _renderPass;
    std::shared_ptr<RenderPipeline> _pipelineTextured;
    std::shared_ptr<Shader> _fragShaderTextured;
    std::shared_ptr<Shader> _vertShaderTextured;

public:

    //! Constructor
    PipelinesSample() :
        Game(),
        _vertexBuffer(nullptr),
        _indexBuffer(nullptr),
        _renderPass(nullptr),
        _pipelineTextured(nullptr),
        _fragShaderTextured(nullptr),
        _vertShaderTextured(nullptr)
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
        {
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
            _quads[0].texture = graphics->createTexture2d(tex2D);
        }
        {
            std::string imageUrl = "crate02_color_height_rgba.ktx";
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
            _quads[1].texture = graphics->createTexture2d(tex2D);
        }

        // Create uniform buffer
        _quads[0].uniformBuffer = graphics->createUniformBuffer(sizeof(QuadParameters::Matrices), true, nullptr);
        _quads[1].uniformBuffer = graphics->createUniformBuffer(sizeof(QuadParameters::Matrices), true, nullptr);


        // Create descriptor sets for each quad
        for (int i=0; i<_quads.size(); i++)
        {
            // Create a shared descriptor set with 3 descriptors
            std::vector<DescriptorSet::Descriptor> descriptors(2);

            // uniform buffer for vertex shader - binding 0
            descriptors[0] = {};
            descriptors[0].type = DescriptorSet::Descriptor::Type::eUniform;
            descriptors[0].binding = 0;
            descriptors[0].count = 1;
            descriptors[0].shaderStages = DescriptorSet::Descriptor::ShaderStages::eVert;
            descriptors[0].uniforms[0] = _quads[i].uniformBuffer;

            // texture for fragment shader - binding 1
            descriptors[1].type = DescriptorSet::Descriptor::Type::eTexture;
            descriptors[1].binding = 1;
            descriptors[1].count = 1;
            descriptors[1].shaderStages = DescriptorSet::Descriptor::ShaderStages::eFrag;
            descriptors[1].textures[0] = _quads[i].texture;

            // Create descriptor set
            _quads[i].descriptorSet = graphics->createDescriptorSet(descriptors.data(), descriptors.size());
        }


        // Create shared states with default values
        RasterizerState rasterizerState;
        ColorBlendState colorBlendState;
        DepthStencilState depthStencilState;

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

        // Create a single render pipeline because descriptor sets use the same layout for each quads,  use same shaders, same states...
        // however shaders use differents uniform buffers and shaders data that wil be binded by Graphics::cmdBindDescriptorSet into onUpdate method
        _pipelineTextured = graphics->createRenderPipeline(RenderPipeline::PrimitiveTopology::eTriangleList, vertexLayout,
                                                         rasterizerState, colorBlendState, depthStencilState,
                                                         _renderPass, _quads[0].descriptorSet,
                                                         _vertShaderTextured, nullptr, nullptr, nullptr, _fragShaderTextured);

    }


    //! @see Game::onFinalize
    void onFinalize()
    {
        std::shared_ptr<Graphics> graphics = getGraphics();
        graphics->destroyBuffer(_vertexBuffer);
        graphics->destroyBuffer(_indexBuffer);
        graphics->destroyRenderPipeline(_pipelineTextured);
        graphics->destroyShader(_vertShaderTextured);
        graphics->destroyShader(_fragShaderTextured);
        for (auto quad : _quads)
        {
            graphics->destroyBuffer(quad.uniformBuffer);
            graphics->destroyTexture(quad.texture);
            graphics->destroyDescriptorSet(quad.descriptorSet);
        }
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

        float time = getAbsoluteTime();

        // Create some ImGui widgets
        static float zTranslation1 = 0.0f;
        static float zTranslation2 = 0.0f;
        ImGui::SetNextWindowPos(ImVec2(10, 10));
        ImGui::SetNextWindowSize(ImVec2(0, 0), ImGuiSetCond_FirstUseEver);
        ImGui::Begin("Example");
        ImGui::Text("%.2f ms/frame (%.1d fps)", (1000.0f / Game::getFrameRate()), Game::getFrameRate());
        ImGui::SliderFloat("Translate1 Z", &zTranslation1, 0.0f, -70.0f);
        ImGui::SliderFloat("Translate2 Z", &zTranslation2, 0.0f, -70.0f);
        ImGui::End();

        // update imgui
        Game::getInstance()->getGraphics()->imguiUpdate();


        // update uniform buffers
        _quads[0].matrices.modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(-2.0f, 0.0f, zTranslation1));
        _quads[1].matrices.modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3( 1.5f, 0.5f, zTranslation2));

        _quads[0].rotation = glm::vec3(3.3f * time, 18.0f * time, -10.4f * time);
        _quads[1].rotation = glm::vec3(-12.2f * time, 4.0f * time, 21.1f * time);

        for (int i=0; i<_quads.size(); i++)
        {
            // Update matrices
            _quads[i].matrices.projectionMatrix = glm::perspective(glm::radians(60.0f), (float)getWidth() / (float)getHeight(), 0.1f, 256.0f);
            _quads[i].matrices.viewMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -4.0f));
            _quads[i].matrices.modelMatrix = glm::rotate(_quads[i].matrices.modelMatrix, glm::radians(_quads[i].rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
            _quads[i].matrices.modelMatrix = glm::rotate(_quads[i].matrices.modelMatrix, glm::radians(_quads[i].rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
            _quads[i].matrices.modelMatrix = glm::rotate(_quads[i].matrices.modelMatrix, glm::radians(_quads[i].rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

            // Map uniform buffer and update it
            void* pdata = _quads[i].uniformBuffer->map(sizeof(QuadParameters::Matrices));
            memcpy(pdata, &_quads[i].matrices, sizeof(QuadParameters::Matrices));
            _quads[i].uniformBuffer->unmap();
        }


        // update graphics

        std::shared_ptr<Graphics> graphics = getGraphics();

        _renderPass = graphics->acquireNextFrame();

        std::shared_ptr<CommandBuffer> commandBuffer = graphics->beginCommands();
        graphics->cmdSetViewport(commandBuffer, 0, 0, getWidth(), getHeight(), 0.0f, 1.0f);
        graphics->cmdSetScissor(commandBuffer, 0, 0, getWidth(), getHeight());
        graphics->cmdBeginRenderPass(commandBuffer, _renderPass);
        graphics->cmdClearColor(commandBuffer, 0.1f, 0.1f, 0.1f, 1.0f, 0);
        graphics->cmdClearDepthStencil(commandBuffer, 1.0, 0);

        // 1st quad
        graphics->cmdBindDescriptorSet(commandBuffer, _pipelineTextured, _quads[0].descriptorSet);
        graphics->cmdBindRenderPipeline(commandBuffer, _pipelineTextured);
        graphics->cmdBindVertexBuffer(commandBuffer, _vertexBuffer);
        graphics->cmdBindIndexBuffer(commandBuffer, _indexBuffer);
        graphics->cmdDrawIndexed(commandBuffer, 6, 0);

        // 2nd quad
        graphics->cmdBindDescriptorSet(commandBuffer, _pipelineTextured, _quads[1].descriptorSet);
        graphics->cmdBindRenderPipeline(commandBuffer, _pipelineTextured);
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

