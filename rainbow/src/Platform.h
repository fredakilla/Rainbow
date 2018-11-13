#pragma once

#include "Input.h"

#define SDL_MAIN_HANDLED
#include <sdl/SDL.h>
#include <sdl/SDL_syswm.h>
#if GP_PLATFORM_LINUX
#undef Bool
#undef CursorShape
#undef Expose
#undef FocusIn
#undef FocusOut
#undef FontChange
#undef KeyPress
#undef KeyRelease
#undef None
#undef Status
#undef Unsorted
#endif

namespace rainbow
{

/**
 * Defines a class for platform windowing and events.
 *
 * This class is only run from the main entry point.
 * It delegates useful event needed by the game engine 
 * classes such as Game, Input and UI.
 */
class Platform
{
public:

    /**
     * Gets the platform.
     */
    static Platform* getPlatform();

    /**
     * Runs the platform and polls for events.
     *
     * @return The returned exit code.
     */
    int run();

    /**
     * Gets the width of the platform window.
     *
     * @return The width of the platform window.
     */
    size_t getWidth() const;

    /**
     * Gets the height of the platform window.
     *
     * @return The height of the platform window.
     */
    size_t getHeight() const;

    /**
     * Gets the height of the platform window.
     *
     * @return The height of the platform window.
     */
    bool isFullscreen() const;

    /**
     * The SDL window handle
     *
     * @return The SDL window handle.
     */
    SDL_Window* getSDLWindow() const;

    /**
     * Gets the handle to the native window or view.
     *
     * @return The handle to the native window or view.
     */
    uint64_t getNativeWindow() const;

    /**
     * Gets the handle to the native connection, display or instance.
     *
     * @return The handle to the native connection, display or instance.
     */
    uint64_t getNativeConnection() const;

    /**
     * Loads a plugin/library by url.
     *
     * Example:
     *
     * Platform::getPlatform()->loadPlugin("./rainbow-user");
     *
     * Dynamically loads:
     *  rainbow-user.dll       (windows)
     *  rainbow-user.so        (linux)
     *  rainbow-user.dynlib    (macos)
     *
     * Required plugin symbols:
     *
     * extern "C"
     * {
     *     GP_EXPORT_API void onInitialize();
     *     GP_EXPORT_API void onFinalize();
     * }
     *
     * @param url The url of the plugin library
     */
    void loadPlugin(const std::string& url);

    /**
     * Unloads a plugin/library by url.
     *
     * @param url
     */
    void unloadPlugin(const std::string& url);

private:

    Platform();
    ~Platform();
    void initTranslateKey(uint16_t sdl, Input::Key key);
    Input::Key translateKey(SDL_Scancode sdl);
    Input::KeyModifiers translateKeyModifiers(uint16_t sdl);
    Input::KeyModifiers translateKeyModifiersPress(uint16_t key);
    void initTranslateGamepad(uint8_t sdl, Input::Key button);
    Input::Key translateGamepad(uint8_t sdl);
    void initTranslateGamepadAxis(uint8_t sdl, Input::GamepadAxis axis);
    Input::GamepadAxis translateGamepadAxis(uint8_t sdl);

    SDL_Window* _window;
    size_t _width;
    size_t _height;
    bool _fullscreen;
    uint64_t _nativeWindow;
    uint64_t _nativeConnection;
    bool _running;
    uint8_t _translateKey[256];
    uint8_t _translateGamepad[256];
    uint8_t _translateGamepadAxis[256];
};

}
