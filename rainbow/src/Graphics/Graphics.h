#pragma once

namespace rainbow
{

/**
 * Defines direct hardware access to gpu functionality
 * for graphics, compute and transfer.
 */
class Graphics
{
    friend class Game;

public:

    /**
     * Constructor.
     */
    Graphics();

    /**
     * Destructor.
     */
    ~Graphics();

    /**
     * Gets the width of the graphics sytem swap images.
     *
     * @return The width of the graphics sytem swap images.
     */
    uint32_t getWidth();

    /**
     * Gets the height of the graphics sytem swap images.
     *
     * @return The height of the graphics sytem swap images.
     */
    uint32_t getHeight();


    virtual void initialize();
    virtual void resize(size_t width, size_t height);
    virtual void render(float elapsedTime);
    virtual void createInstance();


private:
    bool _initialized;
    bool _resized;
    uint32_t _width;
    uint32_t _height;
    bool _fullscreen;
    bool _vsync;
    bool _multisampling;
    bool _validation;

};

}
