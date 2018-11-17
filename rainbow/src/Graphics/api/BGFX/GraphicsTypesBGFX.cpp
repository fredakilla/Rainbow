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
    CommandBuffer(),
    _program(BGFX_INVALID_HANDLE),
    _clearFlags(BGFX_CLEAR_NONE),
    _clearColor(0x00000000)
{

}

void CommandBufferBGFX::reset()
{
    _program = BGFX_INVALID_HANDLE;
    _clearFlags = BGFX_CLEAR_NONE;
    _clearColor = 0x00000000;
}


}
