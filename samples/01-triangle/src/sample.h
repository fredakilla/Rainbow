#pragma once

#include <rainbow.h>


/**
 * Main application and game class.
 */
class Sample : public Game
{
public:

    /**
     * Constructor.
     */
    Sample();

    /**
     * Destructor.
     */
    ~Sample();

    /**
     * @see Game::onInitialize
     */
    void onInitialize();

    /**
     * @see Game::onFinalize
     */
    void onFinalize();

    /**
     * @see Game::onResize
     */
    void onResize(size_t width, size_t height);

    /**
     * @see Game::onUpdate
     */
    void onUpdate(float elapsedTime);

private:

    std::shared_ptr<Buffer> _vertexBuffer;

};
