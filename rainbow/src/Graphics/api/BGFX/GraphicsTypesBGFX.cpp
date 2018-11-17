#include "GraphicsTypesBGFX.h"

namespace rainbow
{

//------------------------------------------------------------------------------
// BufferBGFX
//------------------------------------------------------------------------------

BufferBGFX::BufferBGFX() :
    Buffer(),
    _hostMemory(nullptr)
{
}

BufferBGFX::~BufferBGFX()
{
}

void* BufferBGFX::getHostMemory() const
{
    return _hostMemory;
}



//------------------------------------------------------------------------------
// RenderPipelineBGFX
//------------------------------------------------------------------------------

RenderPipelineBGFX::RenderPipelineBGFX() :
    RenderPipeline(),
    _program(BGFX_INVALID_HANDLE)
{

}

//------------------------------------------------------------------------------
// CommandBufferBGFX
//-----------------------------------------------------------------------------

CommandBufferBGFX::CommandBufferBGFX() :
    CommandBuffer()
{
    reset();
}

void CommandBufferBGFX::reset()
{
    _clearColor = 0x00000000;
    _clearFlags = BGFX_CLEAR_NONE;
    _depth = 0.0f;
    _stencil = 0;
    _program = BGFX_INVALID_HANDLE;
}


}
