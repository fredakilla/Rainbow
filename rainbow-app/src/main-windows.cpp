#pragma comment(linker, "/subsystem:windows")
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "rainbow.h"

using namespace rainbow;


extern "C" int APIENTRY WinMain(HINSTANCE hInstance, 
                                HINSTANCE hPrevInstance,
                                LPSTR cmdLine,
                                int cmdShow)
{
    Platform* platform = Platform::getPlatform();
    return platform->run();
}
