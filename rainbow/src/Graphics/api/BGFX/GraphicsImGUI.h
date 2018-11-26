#ifndef GRAPHICSIMGUI_H
#define GRAPHICSIMGUI_H

#include "../../../Core/Base.h"
#include "../../../Core/Input.h"
#include <imgui/imgui.h>
#include <bgfx/bgfx.h>

namespace rainbow
{

class GraphicsImGUI
{
public:
    static GraphicsImGUI& getInstance();
    void imguiRender(ImDrawData* drawData);
    void imguiShutdown();
    void imguiInit();
    void imguiReset(uint16_t width, uint16_t height);

private:
    bgfx::VertexDecl imguiVertexDecl;
    bgfx::TextureHandle imguiFontTexture;
    bgfx::UniformHandle imguiFontUniform;
    bgfx::ProgramHandle imguiProgram;
};

}

#endif // GRAPHICSIMGUI_H
