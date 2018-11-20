#include "GraphicsTypesBGFX.h"

namespace rainbow
{

//------------------------------------------------------------------------------
// BufferBGFX
//------------------------------------------------------------------------------

BufferBGFX::BufferBGFX() :
    Buffer(),
    _hostMemory(nullptr),
    _mem(nullptr)
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
// UniformBufferBGFX
//-----------------------------------------------------------------------------
UniformBGFX::UniformBGFX() :
    Buffer(),
    _num(0)
{
    _usage = Usage::eUniform;
}

UniformBGFX::~UniformBGFX()
{
}

void* UniformBGFX::getHostMemory() const
{
    return _hostMemory;
}

//------------------------------------------------------------------------------
// RenderPipelineBGFX
//------------------------------------------------------------------------------

RenderPipelineBGFX::RenderPipelineBGFX() :
    RenderPipeline(),
    _program(BGFX_INVALID_HANDLE),
    _state(0L)
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
    _state = 0L;
}


//------------------------------------------------------------------------------
// ShaderBGFX
//-----------------------------------------------------------------------------

ShaderBGFX::ShaderBGFX() :
    Shader(),
    _shaderHandle(BGFX_INVALID_HANDLE)
{

}

ShaderBGFX::~ShaderBGFX()
{

}

//------------------------------------------------------------------------------
// DescriptorSetBGFX
//-----------------------------------------------------------------------------

DescriptorSetBGFX::DescriptorSetBGFX() :
    DescriptorSet()
{
}

DescriptorSetBGFX::~DescriptorSetBGFX()
{
}

}
