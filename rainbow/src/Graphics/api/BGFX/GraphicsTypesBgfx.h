#pragma once

#include "../../GraphicsTypes.h"
#include <bgfx/bgfx.h>

namespace rainbow
{

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
};


//------------------------------------------------------------------------------
// CommandBufferBGFX
//------------------------------------------------------------------------------
/**
 * BGFX impl of a CommandBuffer.
 *
 * BGFX manage internally its own command buffer.
 * The CommandBuffer here is not a real command buffer and is
 * used instead as a wrapper for bgfx object to bound for the render pass.
 *
 */
class CommandBufferBGFX : public CommandBuffer
{
    friend class GraphicsBgfx;

public:

    CommandBufferBGFX();

private:

    bgfx::ProgramHandle _program;

};

} // end namespace rainbow

