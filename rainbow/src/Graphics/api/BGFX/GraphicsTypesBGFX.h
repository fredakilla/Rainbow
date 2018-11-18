#pragma once

#include "../../GraphicsTypes.h"
#include <bgfx/bgfx.h>

namespace rainbow
{

class Vector4;

//------------------------------------------------------------------------------
// BufferBGFX
//------------------------------------------------------------------------------
/**
 * BGFX impl of a Buffer.
 */
class BufferBGFX : public Buffer
{
    friend class GraphicsBgfx;

public:
    //! Constructor
    BufferBGFX();
    //! Destructor
    ~BufferBGFX();
    //! @see Buffer::getHostMemory
    void* getHostMemory() const override;

protected:
    void* _hostMemory;
    union
    {
        bgfx::VertexBufferHandle            _staticVertexBufferHandle;
        bgfx::IndexBufferHandle             _staticIndexBufferHandle;
        bgfx::DynamicVertexBufferHandle     _dynamicVertexBufferHandle;
        bgfx::DynamicIndexBufferHandle      _dynamicIndexBufferHandle;
        bgfx::UniformHandle                 _uiformHandle;
    };

    const bgfx::Memory* _mem;
};


//------------------------------------------------------------------------------
// RenderPipelineBGFX
//------------------------------------------------------------------------------
/**
 * BGFX impl of a RenderPipeline.
 */
class RenderPipelineBGFX : public RenderPipeline
{
    friend class GraphicsBgfx;

public:
    //! Constructor
    RenderPipelineBGFX();

private:

    bgfx::ProgramHandle _program;
    uint64_t _state;
};


//------------------------------------------------------------------------------
// CommandBufferBGFX
//------------------------------------------------------------------------------
/**
 * BGFX impl of a CommandBuffer.
 *
 * BGFX manage internally its own command buffer.
 * The CommandBuffer here is not a real command buffer.
 * Use it as a wrapper for bgfx objects to bound for current the render pass.
 *
 */
class CommandBufferBGFX : public CommandBuffer
{
    friend class GraphicsBgfx;

public:

    CommandBufferBGFX();

    void reset();

private:

    // clear stuff
    uint32_t _clearColor;
    uint16_t _clearFlags;
    float _depth;
    uint32_t _stencil;

    // program
    bgfx::ProgramHandle _program;

    // state
    uint64_t _state;

};

} // end namespace rainbow

