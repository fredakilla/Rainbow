#include "../Core/Base.h"
#include "../Core/Platform.h"
#include "../Core/Game.h"

#include <imgui/imgui.h>
#include "../Graphics/Graphics.h"

#define PLUGIN_FN_ON_LOAD       "onLoad"
#define PLUGIN_FN_ON_UNLOAD     "onUnload"

namespace rainbow
{


//-------------------------------------------------------------------------------------------------------------
// Gamepad
//-----------------------------------------------------------------------------------------------------------

struct GamepadAxisDpadRemap
{
    Input::Key first;
    Input::Key second;
};

GamepadAxisDpadRemap __gamepadAxisDpad[] =
{
    { Input::Key::eGamepadLeft, Input::Key::eGamepadRight   },
    { Input::Key::eGamepadUp,   Input::Key::eGamepadDown    },
    { Input::Key::eNone,         Input::Key::eNone          },
    { Input::Key::eGamepadLeft, Input::Key::eGamepadRight   },
    { Input::Key::eGamepadUp,   Input::Key::eGamepadDown    },
    { Input::Key::eNone,         Input::Key::eNone          }
};

class Gamepad
{
public:
    Gamepad() : 
        joystick(nullptr),
        controller(nullptr), 
        jid(INT32_MAX)
    {
        std::memset(value, 0, sizeof(value));
        deadzone[static_cast<uint32_t>(Input::GamepadAxis::eLeftX)] =
        deadzone[static_cast<uint32_t>(Input::GamepadAxis::eLeftY)] = 7849;
        deadzone[static_cast<uint32_t>(Input::GamepadAxis::eRightX)] =
        deadzone[static_cast<uint32_t>(Input::GamepadAxis::eRightY)] = 8689;
        deadzone[static_cast<uint32_t>(Input::GamepadAxis::eLeftZ)] =
        deadzone[static_cast<uint32_t>(Input::GamepadAxis::eRightZ)] = 30;
    }

    void create(const SDL_JoyDeviceEvent& evt, Game* game)
    {
        if (jid == INT32_MAX)
        {
            Input::getInput()->postGamepadConnectionEvent(evt.which, true);
            game->onGamepadConnection(evt.which, true);
        }
        this->joystick = SDL_JoystickOpen(evt.which);
        SDL_Joystick* joystick = this->joystick;
        jid = SDL_JoystickInstanceID(joystick);
    }

    void create(const SDL_ControllerDeviceEvent& evt, Game* game)
    {
        if (jid == INT32_MAX)
        {
            Input::getInput()->postGamepadConnectionEvent(evt.which, true);
            game->onGamepadConnection(evt.which, true);
        }
        controller = SDL_GameControllerOpen(evt.which);
        SDL_Joystick* joystick = SDL_GameControllerGetJoystick(controller);
        jid = SDL_JoystickInstanceID(joystick);
    }

    void destroy(Game* game, uint32_t controllerIndex)
    {
        if (controller || joystick)
        {
            Input::getInput()->postGamepadConnectionEvent(controllerIndex, false);
            game->onGamepadConnection(controllerIndex, false);
        }
        if (controller != nullptr)
        {
            SDL_GameControllerClose(controller);
            controller = nullptr;
        }
        if (joystick != nullptr)
        {
            SDL_JoystickClose(joystick);
            joystick = nullptr;
        }
        jid = INT32_MAX;
    }

    bool ignore(Input::GamepadAxis axis, int* value)
    {
        const int32_t oldValue = this->value[static_cast<uint32_t>(axis)];
        const int32_t deadzone = this->deadzone[static_cast<uint32_t>(axis)];
        int32_t newValue = *value;
        newValue = ((newValue > deadzone) || (newValue < -deadzone)) ? newValue : 0;
        this->value[static_cast<uint32_t>(axis)] = newValue;
        *value = newValue;
        return oldValue != newValue;
    }

    bool ignore(bool isJoystick) const
    {
        return isJoystick && joystick && controller;
    }

    void updateAxis(uint32_t controllerIndex, Input::GamepadAxis axis, int32_t value, bool isJoystick)
    {
        if (ignore(isJoystick))
        {
            return;
        }
        Input* input = Input::getInput();
        if (ignore(axis, &value))
        {
            input->postGamepadAxisEvent(controllerIndex, axis, value);

            if (__gamepadAxisDpad[static_cast<uint32_t>(axis)].first != Input::Key::eNone)
            {
                if (value == 0)
                {
                    input->postKeyPressEvent(__gamepadAxisDpad[static_cast<uint32_t>(axis)].first, Input::KeyModifiers::eNone, false, controllerIndex);
                    input->postKeyPressEvent(__gamepadAxisDpad[static_cast<uint32_t>(axis)].second, Input::KeyModifiers::eNone, false, controllerIndex);
                }
                else
                {
                    input->postKeyPressEvent(value < 0 ?
                                                 __gamepadAxisDpad[static_cast<uint32_t>(axis)].first :
                                                 __gamepadAxisDpad[static_cast<uint32_t>(axis)].second, Input::KeyModifiers::eNone, true, controllerIndex);
                }
            }
        }
    }

    void updateButton(Game* game, bool isDown, bool isJoystick, Input::Key key, uint32_t controllerIndex)
    {
        if (key == Input::Key::eCount || ignore(isJoystick))
        {
            return;
        }
        Input::getInput()->postKeyPressEvent(key, Input::KeyModifiers::eNone, isDown, controllerIndex);
        game->onKeyPress(key, Input::KeyModifiers::eNone, isDown);
    }

    SDL_Joystick* joystick;
    SDL_GameController* controller;
    SDL_JoystickID jid;
    int32_t value[static_cast<uint32_t>(Input::GamepadAxis::eCount)];
    int32_t deadzone[static_cast<uint32_t>(Input::GamepadAxis::eCount)];
};



//-------------------------------------------------------------------------------------------------------------
// ImGui-SDL2 binding
//-------------------------------------------------------------------------------------------------------------

static bool g_MousePressed[3] = { false, false, false };
static SDL_Cursor* g_MouseCursors[ImGuiMouseCursor_COUNT] = { 0 };
static Uint64 g_Time = 0;

static const char* ImGui_SDL_GetClipboardText(void*)
{
    return SDL_GetClipboardText();
}

static void ImGui_SDL_SetClipboardText(void*, const char* text)
{
    SDL_SetClipboardText(text);
}

static void ImGui_SDL_Shutdown()
{
    // Destroy SDL mouse cursors
    for (ImGuiMouseCursor cursor_n = 0; cursor_n < ImGuiMouseCursor_COUNT; cursor_n++)
        SDL_FreeCursor(g_MouseCursors[cursor_n]);
    memset(g_MouseCursors, 0, sizeof(g_MouseCursors));

    // Destroy imgui graphics
    //Game::getInstance()->getGraphics()->imguiDestroy();
}

static bool ImGui_SDL_Init(SDL_Window* window)
{
    // Create and init imgui graphics
    //Game::getInstance()->getGraphics()->imguiCreate();

    ImGui::StyleColorsClassic();

    // Setup back-end capabilities flags
    ImGuiIO& io = ImGui::GetIO();
    io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;       // We can honor GetMouseCursor() values (optional)
    io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;        // We can honor io.WantSetMousePos requests (optional, rarely used)

    // Keyboard mapping. ImGui will use those indices to peek into the io.KeysDown[] array.
    io.KeyMap[ImGuiKey_Tab] = SDL_SCANCODE_TAB;
    io.KeyMap[ImGuiKey_LeftArrow] = SDL_SCANCODE_LEFT;
    io.KeyMap[ImGuiKey_RightArrow] = SDL_SCANCODE_RIGHT;
    io.KeyMap[ImGuiKey_UpArrow] = SDL_SCANCODE_UP;
    io.KeyMap[ImGuiKey_DownArrow] = SDL_SCANCODE_DOWN;
    io.KeyMap[ImGuiKey_PageUp] = SDL_SCANCODE_PAGEUP;
    io.KeyMap[ImGuiKey_PageDown] = SDL_SCANCODE_PAGEDOWN;
    io.KeyMap[ImGuiKey_Home] = SDL_SCANCODE_HOME;
    io.KeyMap[ImGuiKey_End] = SDL_SCANCODE_END;
    io.KeyMap[ImGuiKey_Insert] = SDL_SCANCODE_INSERT;
    io.KeyMap[ImGuiKey_Delete] = SDL_SCANCODE_DELETE;
    io.KeyMap[ImGuiKey_Backspace] = SDL_SCANCODE_BACKSPACE;
    io.KeyMap[ImGuiKey_Space] = SDL_SCANCODE_SPACE;
    io.KeyMap[ImGuiKey_Enter] = SDL_SCANCODE_RETURN;
    io.KeyMap[ImGuiKey_Escape] = SDL_SCANCODE_ESCAPE;
    io.KeyMap[ImGuiKey_A] = SDL_SCANCODE_A;
    io.KeyMap[ImGuiKey_C] = SDL_SCANCODE_C;
    io.KeyMap[ImGuiKey_V] = SDL_SCANCODE_V;
    io.KeyMap[ImGuiKey_X] = SDL_SCANCODE_X;
    io.KeyMap[ImGuiKey_Y] = SDL_SCANCODE_Y;
    io.KeyMap[ImGuiKey_Z] = SDL_SCANCODE_Z;

    io.SetClipboardTextFn = ImGui_SDL_SetClipboardText;
    io.GetClipboardTextFn = ImGui_SDL_GetClipboardText;
    io.ClipboardUserData = NULL;

    g_MouseCursors[ImGuiMouseCursor_Arrow] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW);
    g_MouseCursors[ImGuiMouseCursor_TextInput] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_IBEAM);
    g_MouseCursors[ImGuiMouseCursor_ResizeAll] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZEALL);
    g_MouseCursors[ImGuiMouseCursor_ResizeNS] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZENS);
    g_MouseCursors[ImGuiMouseCursor_ResizeEW] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZEWE);
    g_MouseCursors[ImGuiMouseCursor_ResizeNESW] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZENESW);
    g_MouseCursors[ImGuiMouseCursor_ResizeNWSE] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZENWSE);
    g_MouseCursors[ImGuiMouseCursor_Hand] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_HAND);

#ifdef _WIN32
    SDL_SysWMinfo wmInfo;
    SDL_VERSION(&wmInfo.version);
    SDL_GetWindowWMInfo(window, &wmInfo);
    io.ImeWindowHandle = wmInfo.info.win.window;
#else
    (void)window;
#endif

    return true;
}

static void ImGui_SDL_UpdateMousePosAndButtons()
{
    ImGuiIO& io = ImGui::GetIO();

    SDL_Window* sdlWindow = Platform::getPlatform()->getSDLWindow();

    // Set OS mouse position if requested (rarely used, only when ImGuiConfigFlags_NavEnableSetMousePos is enabled by user)
    if (io.WantSetMousePos)
        SDL_WarpMouseInWindow(sdlWindow, (int)io.MousePos.x, (int)io.MousePos.y);
    else
        io.MousePos = ImVec2(-FLT_MAX, -FLT_MAX);

    int mx, my;
    Uint32 mouse_buttons = SDL_GetMouseState(&mx, &my);
    io.MouseDown[0] = g_MousePressed[0] || (mouse_buttons & SDL_BUTTON(SDL_BUTTON_LEFT)) != 0;  // If a mouse press event came, always pass it as "mouse held this frame", so we don't miss click-release events that are shorter than 1 frame.
    io.MouseDown[1] = g_MousePressed[1] || (mouse_buttons & SDL_BUTTON(SDL_BUTTON_RIGHT)) != 0;
    io.MouseDown[2] = g_MousePressed[2] || (mouse_buttons & SDL_BUTTON(SDL_BUTTON_MIDDLE)) != 0;
    g_MousePressed[0] = g_MousePressed[1] = g_MousePressed[2] = false;

#if SDL_HAS_CAPTURE_MOUSE && !defined(__EMSCRIPTEN__)
    SDL_Window* focused_window = SDL_GetKeyboardFocus();
    if (g_Window == focused_window)
    {
        // SDL_GetMouseState() gives mouse position seemingly based on the last window entered/focused(?)
        // The creation of a new windows at runtime and SDL_CaptureMouse both seems to severely mess up with that, so we retrieve that position globally.
        int wx, wy;
        SDL_GetWindowPosition(focused_window, &wx, &wy);
        SDL_GetGlobalMouseState(&mx, &my);
        mx -= wx;
        my -= wy;
        io.MousePos = ImVec2((float)mx, (float)my);
    }

    // SDL_CaptureMouse() let the OS know e.g. that our imgui drag outside the SDL window boundaries shouldn't e.g. trigger the OS window resize cursor.
    // The function is only supported from SDL 2.0.4 (released Jan 2016)
    bool any_mouse_button_down = ImGui::IsAnyMouseDown();
    SDL_CaptureMouse(any_mouse_button_down ? SDL_TRUE : SDL_FALSE);
#else
    if (SDL_GetWindowFlags(sdlWindow) & SDL_WINDOW_INPUT_FOCUS)
        io.MousePos = ImVec2((float)mx, (float)my);
#endif
}

static void ImGui_SDL_UpdateMouseCursor()
{
    ImGuiIO& io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_NoMouseCursorChange)
        return;

    ImGuiMouseCursor imgui_cursor = ImGui::GetMouseCursor();
    if (io.MouseDrawCursor || imgui_cursor == ImGuiMouseCursor_None)
    {
        // Hide OS mouse cursor if imgui is drawing it or if it wants no cursor
        SDL_ShowCursor(SDL_FALSE);
    }
    else
    {
        // Show OS mouse cursor
        SDL_SetCursor(g_MouseCursors[imgui_cursor] ? g_MouseCursors[imgui_cursor] : g_MouseCursors[ImGuiMouseCursor_Arrow]);
        SDL_ShowCursor(SDL_TRUE);
    }
}

static void ImGui_SDL_NewFrame(SDL_Window* window)
{
    ImGuiIO& io = ImGui::GetIO();
    IM_ASSERT(io.Fonts->IsBuilt());     // Font atlas needs to be built, call renderer _NewFrame() function e.g. ImGui_ImplOpenGL3_NewFrame()

    // Setup display size (every frame to accommodate for window resizing)
    int w, h;
    int display_w, display_h;
    SDL_GetWindowSize(window, &w, &h);
    SDL_GL_GetDrawableSize(window, &display_w, &display_h);
    io.DisplaySize = ImVec2((float)w, (float)h);
    io.DisplayFramebufferScale = ImVec2(w > 0 ? ((float)display_w / w) : 0, h > 0 ? ((float)display_h / h) : 0);

    // Setup time step (we don't use SDL_GetTicks() because it is using millisecond resolution)
    static Uint64 frequency = SDL_GetPerformanceFrequency();
    Uint64 current_time = SDL_GetPerformanceCounter();
    io.DeltaTime = g_Time > 0 ? (float)((double)(current_time - g_Time) / frequency) : (float)(1.0f / 60.0f);
    g_Time = current_time;

    ImGui_SDL_UpdateMousePosAndButtons();
    ImGui_SDL_UpdateMouseCursor();


    // Start the frame. This call will update the io.WantCaptureMouse, io.WantCaptureKeyboard flag that you can use to dispatch inputs (or not) to your application.
    ImGui::NewFrame();

    // fix issue when debugging (mouse was still captured by sdl and we cannot use it in debugger)
    //SDL_CaptureMouse(SDL_FALSE);
}

// You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
// - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application.
// - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application.
// Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
static bool ImGui_SDL_ProcessEvent(SDL_Event* event)
{
    ImGuiIO& io = ImGui::GetIO();
    switch (event->type)
    {
    case SDL_MOUSEWHEEL:
        {
            if (event->wheel.x > 0) io.MouseWheelH += 1;
            if (event->wheel.x < 0) io.MouseWheelH -= 1;
            if (event->wheel.y > 0) io.MouseWheel += 1;
            if (event->wheel.y < 0) io.MouseWheel -= 1;
            return true;
        }
    case SDL_MOUSEBUTTONDOWN:
        {
            if (event->button.button == SDL_BUTTON_LEFT) g_MousePressed[0] = true;
            if (event->button.button == SDL_BUTTON_RIGHT) g_MousePressed[1] = true;
            if (event->button.button == SDL_BUTTON_MIDDLE) g_MousePressed[2] = true;
            return true;
        }
    case SDL_TEXTINPUT:
        {
            io.AddInputCharactersUTF8(event->text.text);
            return true;
        }
    case SDL_KEYDOWN:
    case SDL_KEYUP:
        {
            int key = event->key.keysym.scancode;
            IM_ASSERT(key >= 0 && key < IM_ARRAYSIZE(io.KeysDown));
            io.KeysDown[key] = (event->type == SDL_KEYDOWN);
            io.KeyShift = ((SDL_GetModState() & KMOD_SHIFT) != 0);
            io.KeyCtrl = ((SDL_GetModState() & KMOD_CTRL) != 0);
            io.KeyAlt = ((SDL_GetModState() & KMOD_ALT) != 0);
            io.KeySuper = ((SDL_GetModState() & KMOD_GUI) != 0);
            return true;
        }
    }
    return false;
}


//-------------------------------------------------------------------------------------------------------------
// Platform SDL2 impl
//-------------------------------------------------------------------------------------------------------------

Platform* Platform::getPlatform()
{
    static Platform platform;
    return &platform;
}

Platform::Platform() :
    _window(nullptr),
    _width(0),
    _height(0),
    _fullscreen(false),
    _nativeWindow(0),
    _nativeConnection(0),
    _running(false)
{
    std::memset(_translateKey, 0, sizeof(_translateKey));
    initTranslateKey(SDL_SCANCODE_ESCAPE,       Input::Key::eEsc);
    initTranslateKey(SDL_SCANCODE_RETURN,       Input::Key::eReturn);
    initTranslateKey(SDL_SCANCODE_TAB,          Input::Key::eTab);
    initTranslateKey(SDL_SCANCODE_BACKSPACE,    Input::Key::eBackspace);
    initTranslateKey(SDL_SCANCODE_SPACE,        Input::Key::eSpace);
    initTranslateKey(SDL_SCANCODE_UP,           Input::Key::eUp);
    initTranslateKey(SDL_SCANCODE_DOWN,         Input::Key::eDown);
    initTranslateKey(SDL_SCANCODE_LEFT,         Input::Key::eLeft);
    initTranslateKey(SDL_SCANCODE_RIGHT,        Input::Key::eRight);
    initTranslateKey(SDL_SCANCODE_PAGEUP,       Input::Key::ePageUp);
    initTranslateKey(SDL_SCANCODE_PAGEDOWN,     Input::Key::ePageDown);
    initTranslateKey(SDL_SCANCODE_HOME,         Input::Key::eHome);
    initTranslateKey(SDL_SCANCODE_END,          Input::Key::eEnd);
    initTranslateKey(SDL_SCANCODE_PRINTSCREEN,  Input::Key::ePrint);
    initTranslateKey(SDL_SCANCODE_KP_PLUS,      Input::Key::ePlus);
    initTranslateKey(SDL_SCANCODE_EQUALS,       Input::Key::ePlus);
    initTranslateKey(SDL_SCANCODE_KP_MINUS,     Input::Key::eMinus);
    initTranslateKey(SDL_SCANCODE_MINUS,        Input::Key::eMinus);
    initTranslateKey(SDL_SCANCODE_GRAVE,        Input::Key::eTilde);
    initTranslateKey(SDL_SCANCODE_KP_COMMA,     Input::Key::eComma);
    initTranslateKey(SDL_SCANCODE_COMMA,        Input::Key::eComma);
    initTranslateKey(SDL_SCANCODE_KP_PERIOD,    Input::Key::ePeriod);
    initTranslateKey(SDL_SCANCODE_PERIOD,       Input::Key::ePeriod);
    initTranslateKey(SDL_SCANCODE_SLASH,        Input::Key::eSlash);
    initTranslateKey(SDL_SCANCODE_F1,           Input::Key::eF1);
    initTranslateKey(SDL_SCANCODE_F2,           Input::Key::eF2);
    initTranslateKey(SDL_SCANCODE_F3,           Input::Key::eF3);
    initTranslateKey(SDL_SCANCODE_F4,           Input::Key::eF4);
    initTranslateKey(SDL_SCANCODE_F5,           Input::Key::eF5);
    initTranslateKey(SDL_SCANCODE_F6,           Input::Key::eF6);
    initTranslateKey(SDL_SCANCODE_F7,           Input::Key::eF7);
    initTranslateKey(SDL_SCANCODE_F8,           Input::Key::eF8);
    initTranslateKey(SDL_SCANCODE_F9,           Input::Key::eF9);
    initTranslateKey(SDL_SCANCODE_F10,          Input::Key::eF10);
    initTranslateKey(SDL_SCANCODE_F11,          Input::Key::eF11);
    initTranslateKey(SDL_SCANCODE_F12,          Input::Key::eF12);
    initTranslateKey(SDL_SCANCODE_KP_0,         Input::Key::eNumpad0);
    initTranslateKey(SDL_SCANCODE_KP_1,         Input::Key::eNumpad1);
    initTranslateKey(SDL_SCANCODE_KP_2,         Input::Key::eNumpad2);
    initTranslateKey(SDL_SCANCODE_KP_3,         Input::Key::eNumpad3);
    initTranslateKey(SDL_SCANCODE_KP_4,         Input::Key::eNumpad4);
    initTranslateKey(SDL_SCANCODE_KP_5,         Input::Key::eNumpad5);
    initTranslateKey(SDL_SCANCODE_KP_6,         Input::Key::eNumpad6);
    initTranslateKey(SDL_SCANCODE_KP_7,         Input::Key::eNumpad7);
    initTranslateKey(SDL_SCANCODE_KP_8,         Input::Key::eNumpad8);
    initTranslateKey(SDL_SCANCODE_KP_9,         Input::Key::eNumpad9);
    initTranslateKey(SDL_SCANCODE_0,            Input::Key::e0);
    initTranslateKey(SDL_SCANCODE_1,            Input::Key::e1);
    initTranslateKey(SDL_SCANCODE_2,            Input::Key::e2);
    initTranslateKey(SDL_SCANCODE_3,            Input::Key::e3);
    initTranslateKey(SDL_SCANCODE_4,            Input::Key::e4);
    initTranslateKey(SDL_SCANCODE_5,            Input::Key::e5);
    initTranslateKey(SDL_SCANCODE_6,            Input::Key::e6);
    initTranslateKey(SDL_SCANCODE_7,            Input::Key::e7);
    initTranslateKey(SDL_SCANCODE_8,            Input::Key::e8);
    initTranslateKey(SDL_SCANCODE_9,            Input::Key::e9);
    initTranslateKey(SDL_SCANCODE_A,            Input::Key::eA);
    initTranslateKey(SDL_SCANCODE_B,            Input::Key::eB);
    initTranslateKey(SDL_SCANCODE_C,            Input::Key::eC);
    initTranslateKey(SDL_SCANCODE_D,            Input::Key::eD);
    initTranslateKey(SDL_SCANCODE_E,            Input::Key::eE);
    initTranslateKey(SDL_SCANCODE_F,            Input::Key::eF);
    initTranslateKey(SDL_SCANCODE_G,            Input::Key::eG);
    initTranslateKey(SDL_SCANCODE_H,            Input::Key::eH);
    initTranslateKey(SDL_SCANCODE_I,            Input::Key::eI);
    initTranslateKey(SDL_SCANCODE_J,            Input::Key::eJ);
    initTranslateKey(SDL_SCANCODE_K,            Input::Key::eK);
    initTranslateKey(SDL_SCANCODE_L,            Input::Key::eL);
    initTranslateKey(SDL_SCANCODE_M,            Input::Key::eM);
    initTranslateKey(SDL_SCANCODE_N,            Input::Key::eN);
    initTranslateKey(SDL_SCANCODE_O,            Input::Key::eO);
    initTranslateKey(SDL_SCANCODE_P,            Input::Key::eP);
    initTranslateKey(SDL_SCANCODE_Q,            Input::Key::eQ);
    initTranslateKey(SDL_SCANCODE_R,            Input::Key::eR);
    initTranslateKey(SDL_SCANCODE_S,            Input::Key::eS);
    initTranslateKey(SDL_SCANCODE_T,            Input::Key::eT);
    initTranslateKey(SDL_SCANCODE_U,            Input::Key::eU);
    initTranslateKey(SDL_SCANCODE_V,            Input::Key::eV);
    initTranslateKey(SDL_SCANCODE_W,            Input::Key::eW);
    initTranslateKey(SDL_SCANCODE_X,            Input::Key::eX);
    initTranslateKey(SDL_SCANCODE_Y,            Input::Key::eY);
    initTranslateKey(SDL_SCANCODE_Z,            Input::Key::eZ);

    std::memset(_translateGamepad, uint8_t(Input::Key::eCount), sizeof(_translateGamepad) );
    initTranslateGamepad(SDL_CONTROLLER_BUTTON_A,             Input::Key::eGamepadA);
    initTranslateGamepad(SDL_CONTROLLER_BUTTON_B,             Input::Key::eGamepadB);
    initTranslateGamepad(SDL_CONTROLLER_BUTTON_X,             Input::Key::eGamepadX);
    initTranslateGamepad(SDL_CONTROLLER_BUTTON_Y,             Input::Key::eGamepadY);
    initTranslateGamepad(SDL_CONTROLLER_BUTTON_LEFTSTICK,     Input::Key::eGamepadThumbLeft);
    initTranslateGamepad(SDL_CONTROLLER_BUTTON_RIGHTSTICK,    Input::Key::eGamepadThumbRight);
    initTranslateGamepad(SDL_CONTROLLER_BUTTON_LEFTSHOULDER,  Input::Key::eGamepadShoulderLeft);
    initTranslateGamepad(SDL_CONTROLLER_BUTTON_RIGHTSHOULDER, Input::Key::eGamepadShoulderRight);
    initTranslateGamepad(SDL_CONTROLLER_BUTTON_DPAD_UP,       Input::Key::eGamepadUp);
    initTranslateGamepad(SDL_CONTROLLER_BUTTON_DPAD_DOWN,     Input::Key::eGamepadDown);
    initTranslateGamepad(SDL_CONTROLLER_BUTTON_DPAD_LEFT,     Input::Key::eGamepadLeft);
    initTranslateGamepad(SDL_CONTROLLER_BUTTON_DPAD_RIGHT,    Input::Key::eGamepadRight);
    initTranslateGamepad(SDL_CONTROLLER_BUTTON_BACK,          Input::Key::eGamepadBack);
    initTranslateGamepad(SDL_CONTROLLER_BUTTON_START,         Input::Key::eGamepadStart);
    initTranslateGamepad(SDL_CONTROLLER_BUTTON_GUIDE,         Input::Key::eGamepadGuide);

    std::memset(_translateGamepadAxis, uint8_t(Input::GamepadAxis::eCount), sizeof(_translateGamepadAxis) );
    initTranslateGamepadAxis(SDL_CONTROLLER_AXIS_LEFTX,        Input::GamepadAxis::eLeftX);
    initTranslateGamepadAxis(SDL_CONTROLLER_AXIS_LEFTY,        Input::GamepadAxis::eLeftY);
    initTranslateGamepadAxis(SDL_CONTROLLER_AXIS_TRIGGERLEFT,  Input::GamepadAxis::eLeftZ);
    initTranslateGamepadAxis(SDL_CONTROLLER_AXIS_RIGHTX,       Input::GamepadAxis::eRightX);
    initTranslateGamepadAxis(SDL_CONTROLLER_AXIS_RIGHTY,       Input::GamepadAxis::eRightY);
    initTranslateGamepadAxis(SDL_CONTROLLER_AXIS_TRIGGERRIGHT, Input::GamepadAxis::eRightZ);
}

Platform::~Platform()
{
}

int Platform::run()
{
    if (_running)
        return 0;

    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_GAMECONTROLLER) < 0)
        GP_ERROR("Failed to initialize. SDL error=%s", SDL_GetError());

    Game* game = Game::getInstance();
    std::shared_ptr<Game::Config> config = game->getConfig();

    _width = config->width;
    _height = config->height;
    _fullscreen = config->fullscreen;

    // Create the SDL window based on game config
    _window = SDL_CreateWindow(config->title.c_str(), 
                                SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
                                _width, _height, 
                                _fullscreen ? 
                                (SDL_WINDOW_SHOWN | SDL_WINDOW_FULLSCREEN) : 
                                (SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE));

    // Get the native display,instance and windows
    SDL_SysWMinfo wmi;
    SDL_VERSION(&wmi.version);
    if (SDL_GetWindowWMInfo(_window, &wmi) )
    {
#if GP_PLATFORM_WINDOWS
        _nativeWindow = (uint64_t)wmi.info.win.window;
        _nativeConnection = (uint64_t)wmi.info.win.hinstance;
#elif GP_PLATFORM_LINUX
        _nativeWindow = (uint64_t)wmi.info.x11.window;
        _nativeConnection = (uint64_t)wmi.info.x11.display;
#elif GP_PLATFORM_MACOS
        _nativeWindow = (uint64_t)wmi.info.cocoa.window;
        _nativeConnection = 0;
#else
        GP_ERROR("Failed to initialize. Unsupported platform: %s");
#endif  
    }
    else
    {
        GP_WARN("Failed to get SDL window info. SDL error: %s", SDL_GetError());
    }

    // Enable file drops
    SDL_EventState(SDL_DROPFILE, SDL_ENABLE);
    Input* input = Input::getInput();
    input->initialize();
    game->onInitialize();
    Gamepad gamepads[Input::kGamepadsMax];
    int mx, my = 0.0f;
    bool exit = false;
    _running = true;

    // init imgui
    ImGui_SDL_Init(_window);

    // Start the event loop
    SDL_Event evt;
    while (!exit)
    {
        while (SDL_PollEvent(&evt))
        {
            // Process ImGui events
            ImGui_SDL_ProcessEvent(&evt);

            // Process SDL events
            switch (evt.type)
            {
                case SDL_QUIT:
                    {
                        exit = true;
                    }
                    break;

                case SDL_MOUSEMOTION:
                    {
                        const SDL_MouseMotionEvent& motionEvt = evt.motion;
                        mx = motionEvt.x;
                        my = motionEvt.y;
                        input->postMouseMotionEvent(mx, my);
                        game->onMouseMotion(mx, my);
                    }
                    break;

                case SDL_MOUSEBUTTONDOWN:
                case SDL_MOUSEBUTTONUP:
                    {
                        const SDL_MouseButtonEvent& mouseEvent = evt.button;
                        Input::MouseButton button;
                        switch (mouseEvent.button)
                        {
                        default:
                        case SDL_BUTTON_LEFT:
                            button = Input::MouseButton::eLeft;
                            break;
                        case SDL_BUTTON_MIDDLE:
                            button = Input::MouseButton::eMiddle;
                            break;
                        case SDL_BUTTON_RIGHT:
                            button = Input::MouseButton::eRight;
                            break;
                        }
                        input->postMousePressEvent(mouseEvent.x, mouseEvent.y, button,
                                                   mouseEvent.type == SDL_MOUSEBUTTONDOWN);
                        game->onMousePress(mouseEvent.x, mouseEvent.y, 0, button, 
                                           mouseEvent.type == SDL_MOUSEBUTTONDOWN);
                    }
                    break;

                case SDL_MOUSEWHEEL:
                    {
                        const SDL_MouseWheelEvent& mouseEvent = evt.wheel;
                        input->postMouseWheelEvent(mouseEvent.y);
                        game->onMouseWheel(mouseEvent.y);
                    }
                    break;

                case SDL_TEXTINPUT:
                    {
                        const SDL_TextInputEvent& textEvent = evt.text;
                        input->postKeyCharEvent(textEvent.text[0]);
                        game->onKeyChar(textEvent.text[0]);
                    }
                    break;

                case SDL_KEYDOWN:
                    {
                        const SDL_KeyboardEvent& keyEvent = evt.key;

                        Input::KeyModifiers keyModifiers = translateKeyModifiers(keyEvent.keysym.mod);
                        Input::Key key = translateKey(keyEvent.keysym.scancode);
                        if (static_cast<uint32_t>(key) == 0 && keyModifiers == Input::KeyModifiers::eNone)
                        {
                            keyModifiers = translateKeyModifiersPress(keyEvent.keysym.scancode);
                        }

                        if (key == Input::Key::eEsc)
                        {
                            char pressedChar = 0x1b;
                            input->postKeyCharEvent(pressedChar);
                            game->onKeyChar(pressedChar);
                        }
                        else if (key == Input::Key::eReturn)
                        {
                            char pressedChar = 0x0d;
                            input->postKeyCharEvent(pressedChar);
                            game->onKeyChar(pressedChar);
                        }
                        else if (key == Input::Key::eBackspace)
                        {
                            uint8_t pressedChar = 0x08;
                            input->postKeyCharEvent(pressedChar);
                            game->onKeyChar(pressedChar);
                        }
                        input->postKeyPressEvent(key, keyModifiers, keyEvent.state == SDL_PRESSED);
                        game->onKeyPress(key, keyModifiers, keyEvent.state == SDL_PRESSED);
                    }
                    break;

                    case SDL_KEYUP:
                        {
                            const SDL_KeyboardEvent& keyEvent = evt.key;
                            Input::KeyModifiers keyModifiers = translateKeyModifiers(keyEvent.keysym.mod);
                            Input::Key key = translateKey(keyEvent.keysym.scancode);
                            input->postKeyPressEvent(key, keyModifiers, keyEvent.state == SDL_PRESSED);
                            game->onKeyPress(key, keyModifiers, keyEvent.state == SDL_PRESSED);
                        }
                        break;

                    case SDL_JOYDEVICEADDED:
                        {
                            const SDL_JoyDeviceEvent& joystickEvent = evt.jdevice;
                            gamepads[joystickEvent.which].create(joystickEvent, game);
                        }
                        break;

                    case SDL_JOYDEVICEREMOVED:
                        {
                            const SDL_JoyDeviceEvent& joystickEvent = evt.jdevice;
                            gamepads[joystickEvent.which].destroy(game, joystickEvent.which);
                        }
                        break;

                    case SDL_CONTROLLERDEVICEADDED:
                        {
                            const SDL_ControllerDeviceEvent& controllerEvent = evt.cdevice;
                            gamepads[controllerEvent.which].create(controllerEvent, game);
                        }
                        break;

                    case SDL_CONTROLLERDEVICEREMAPPED:
                        break;

                    case SDL_CONTROLLERDEVICEREMOVED:
                        {
                            const SDL_ControllerDeviceEvent& controllerEvent = evt.cdevice;
                            gamepads[controllerEvent.which].destroy(game, controllerEvent.which);
                        }
                        break;

                    case SDL_JOYAXISMOTION:
                        {
                            const SDL_JoyAxisEvent& joystickEvent = evt.jaxis;
                            const Input::GamepadAxis axis = translateGamepadAxis(joystickEvent.axis);
                            gamepads[joystickEvent.which].updateAxis(joystickEvent.which, axis, joystickEvent.value, true);
                        }
                        break;

                    case SDL_CONTROLLERAXISMOTION:
                        {
                            const SDL_ControllerAxisEvent& controllerEvent = evt.caxis;
                            const Input::GamepadAxis axis = translateGamepadAxis(controllerEvent.axis);
                            gamepads[controllerEvent.which].updateAxis(controllerEvent.which, axis, controllerEvent.value, false);
                        }
                        break;

                    case SDL_JOYBUTTONDOWN:
                    case SDL_JOYBUTTONUP:
                        {
                            const SDL_JoyButtonEvent& buttonEvent = evt.jbutton;
                            gamepads[buttonEvent.which].updateButton(game, evt.type == SDL_JOYBUTTONDOWN, true, translateGamepad(buttonEvent.button), buttonEvent.which);
                        }
                        break;

                    case SDL_CONTROLLERBUTTONDOWN:
                    case SDL_CONTROLLERBUTTONUP:
                        {
                            const SDL_ControllerButtonEvent& buttonEvent = evt.cbutton;
                            gamepads[buttonEvent.which].updateButton(game, evt.type == SDL_CONTROLLERBUTTONDOWN, false, translateGamepad(buttonEvent.button), buttonEvent.which);
                        }
                        break;

                    case SDL_WINDOWEVENT:
                        {
                            const SDL_WindowEvent& windowEvent = evt.window;
                            switch (windowEvent.event)
                            {
                            case SDL_WINDOWEVENT_SIZE_CHANGED:
                                game->onResize(windowEvent.data1, windowEvent.data2);
                                break;
                            case SDL_WINDOWEVENT_SHOWN:
                            case SDL_WINDOWEVENT_HIDDEN:
                            case SDL_WINDOWEVENT_EXPOSED:
                            case SDL_WINDOWEVENT_MOVED:
                            case SDL_WINDOWEVENT_MINIMIZED:
                            case SDL_WINDOWEVENT_MAXIMIZED:
                            case SDL_WINDOWEVENT_RESTORED:
                            case SDL_WINDOWEVENT_ENTER:
                            case SDL_WINDOWEVENT_LEAVE:
                            case SDL_WINDOWEVENT_FOCUS_GAINED:
                            case SDL_WINDOWEVENT_FOCUS_LOST:
                                break;
                            case SDL_WINDOWEVENT_CLOSE:
                                exit = true;
                                break;
                            }
                        }
                        break;


                    case SDL_DROPFILE:
                        {
                            const SDL_DropEvent& dropEvent = evt.drop;
                            game->onDropFile(std::string(dropEvent.file));
                            SDL_free(dropEvent.file);
                        }
                        break;
            }
        }

        ImGui_SDL_NewFrame(_window);

        game->frame();
        input->update();
    }

    game->exit();

    ImGui_SDL_Shutdown();
    SDL_DestroyWindow(_window);
    SDL_Quit();

    return 0;
}

uint64_t Platform::getNativeWindow() const
{
    return _nativeWindow;
}

uint64_t Platform::getNativeConnection() const
{
    return _nativeConnection;
}

void loadPlugin(const std::string& url)
{
    /* todo:

    std::string path = url;
    #if GP_PLATFORM_WINDOWS
        path.append(".dll");
    #elif GP_PLATFORM_LINUX
        path.append(".so");
    #elif GP_PLATFORM_MACOS
        path.append(".dynlib");
    #else
        GP_ERROR("Platform no supported");
    #endif

    // Dynamically load the library/object
    void* libraryHandle = SDL_LoadObject(url.c_str());
    char* onLoadFunc = PLUGIN_FN_ON_LOAD;
    void (*onLoad)(int anInt);
    onLoad = (void (*)())SDL_LoadFunction(libraryHandle, onLoadFunc);
    if (onLoad != NULL)
    {
        // Invoke the onLoad function in the library
        onLoad();
    }
    else
    {
        // todo: error handling
    }
    */
}

void unloadPlugin(const std::string& url)
{
    // todo:
}

size_t Platform::getWidth() const
{
    return _width;
}

size_t Platform::getHeight() const
{
    return _height;
}

bool Platform::isFullscreen() const
{
    return _fullscreen;
}

SDL_Window * Platform::getSDLWindow() const
{
    return _window;
}

void Platform::initTranslateKey(uint16_t sdl, Input::Key key)
{
    _translateKey[sdl & 0xff] = (uint8_t)key;
}

Input::Key Platform::translateKey(SDL_Scancode sdl)
{
    return (Input::Key)_translateKey[sdl & 0xff];
}

 Input::KeyModifiers Platform::translateKeyModifiers(uint16_t sdl)
{
    Input::KeyModifiers keyModifiers = Input::KeyModifiers::eNone;
    keyModifiers |= sdl & KMOD_LALT   ? Input::KeyModifiers::eLeftAlt   : Input::KeyModifiers::eNone;
    keyModifiers |= sdl & KMOD_RALT   ? Input::KeyModifiers::eRightAlt   : Input::KeyModifiers::eNone;
    keyModifiers |= sdl & KMOD_LCTRL  ? Input::KeyModifiers::eLeftCtrl   : Input::KeyModifiers::eNone;
    keyModifiers |= sdl & KMOD_RCTRL  ? Input::KeyModifiers::eRightCtrl  : Input::KeyModifiers::eNone;
    keyModifiers |= sdl & KMOD_LSHIFT ? Input::KeyModifiers::eLeftShift  : Input::KeyModifiers::eNone;
    keyModifiers |= sdl & KMOD_RSHIFT ? Input::KeyModifiers::eRightShift : Input::KeyModifiers::eNone;
    keyModifiers |= sdl & KMOD_LGUI   ? Input::KeyModifiers::eLeftMeta   : Input::KeyModifiers::eNone;
    keyModifiers |= sdl & KMOD_RGUI   ? Input::KeyModifiers::eRightMeta  : Input::KeyModifiers::eNone;
    return keyModifiers;
}

Input::KeyModifiers Platform::translateKeyModifiersPress(uint16_t key)
{
    Input::KeyModifiers keyModifiers = Input::KeyModifiers::eNone;
    switch (key)
    {
    case SDL_SCANCODE_LALT:
        keyModifiers = Input::KeyModifiers::eLeftAlt;
        break;
    case SDL_SCANCODE_RALT:
        keyModifiers = Input::KeyModifiers::eRightAlt;
        break;
    case SDL_SCANCODE_LCTRL:
        keyModifiers = Input::KeyModifiers::eLeftCtrl;
        break;
    case SDL_SCANCODE_RCTRL:
        keyModifiers = Input::KeyModifiers::eRightCtrl;
        break;
    case SDL_SCANCODE_LSHIFT:
        keyModifiers = Input::KeyModifiers::eLeftShift;
        break;
    case SDL_SCANCODE_RSHIFT:
        keyModifiers = Input::KeyModifiers::eRightShift;
        break;
    case SDL_SCANCODE_LGUI:
        keyModifiers = Input::KeyModifiers::eLeftMeta;
        break;
    case SDL_SCANCODE_RGUI:
        keyModifiers = Input::KeyModifiers::eRightMeta;
        break;
    default:
        keyModifiers = Input::KeyModifiers::eNone;
        break;
    }
    return keyModifiers;
}

void Platform::initTranslateGamepad(uint8_t sdl, Input::Key button)
{
    _translateGamepad[sdl] = static_cast<uint8_t>(button);
}

Input::Key Platform::translateGamepad(uint8_t sdl)
{
    return Input::Key(_translateGamepad[sdl]);
}

void Platform::initTranslateGamepadAxis(uint8_t sdl, Input::GamepadAxis axis)
{
    _translateGamepadAxis[sdl] = uint8_t(axis);
}

Input::GamepadAxis Platform::translateGamepadAxis(uint8_t sdl)
{
    return Input::GamepadAxis(_translateGamepadAxis[sdl]);
}

void Platform::setWindowTitle(const char* title)
{
    SDL_SetWindowTitle(_window, title);
}


extern void print(const char* format, ...)
{
#ifdef GP_PLATFORM_WINDOWS
    va_list argptr;
    va_start(argptr, format);
    int sz = vfprintf(stderr, format, argptr);
    if (sz > 0)
    {
        char* buf = new char[sz + 1];
        vsprintf(buf, format, argptr);
        buf[sz] = 0;
        OutputDebugStringA(buf);
        GP_SAFE_DELETE_ARRAY(buf);
    }
    va_end(argptr);
#elif GP_PLATFORM_LINUX || GP_PLATFORM_MACOS
    GP_ASSERT(format);
    va_list argptr;
    va_start(argptr, format);
    vfprintf(stderr, format, argptr);
    va_end(argptr);
#endif
}

}
