#include <rainbow.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <gli/gli.hpp>
#include <imgui/imgui.h>


class StaticMeshSample : public Game
{
private:

    std::unique_ptr<Scene> _scene;

public:

    //! Constructor
    StaticMeshSample() :
        Game(),
        _scene(nullptr)
    {
    }

    //! Destructor
    ~StaticMeshSample()
    {
    }

    //! @see Game::onInitialize
    void onInitialize()
    {
        Platform::getPlatform()->setWindowTitle("StaticMesh Sample");
        Game::onInitialize();

        // vertex layout
        VertexLayout::Attribute attributes[] =
        {
            VertexLayout::Attribute(VertexLayout::Semantic::ePosition, VertexLayout::VertexFormat::eFloat3, 0, 0, 0),
            VertexLayout::Attribute(VertexLayout::Semantic::eColor, VertexLayout::VertexFormat::eFloat3, 0, 1, VertexLayout::toStride(attributes[0].format))
        };

        struct Vertex
        {
            float position[3];
            float color[3];
        };

        // vertices
        std::vector<Vertex> vertices =
        {
            { {  1.0f,  1.0f, 0.0f },   { 1.0f, 0.0f, 0.0f } },
            { { -1.0f,  1.0f, 0.0f },   { 0.0f, 1.0f, 0.0f } },
            { {  0.0f, -1.0f, 0.0f },   { 0.0f, 0.0f, 1.0f } }
        };

        // create mesh
        std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>();
        mesh->set(VertexLayout(attributes, 2), vertices.size(), vertices.data(), false);

        // create model
        std::shared_ptr<Model> model = std::make_shared<Model>();
        model->setMesh(mesh);

        // create node
        std::shared_ptr<SceneObject> node = std::make_shared<SceneObject>();
        node->addComponent(model);

        // create scene
        _scene = std::make_unique<Scene>();
        _scene->addChild(node);
    }


    //! @see Game::onFinalize
    void onFinalize()
    {
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
    }
};

// declare our game instance
static StaticMeshSample sample;

