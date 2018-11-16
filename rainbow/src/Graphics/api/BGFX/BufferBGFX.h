#pragma once

#include "../../../Graphics/Buffer.h"
#include <bgfx/bgfx.h>

namespace rainbow
{

/**
 * BGFX impl of a Buffer.
 */
class BufferBGFX : public Buffer
{
    friend class GraphicsBgfx;

public:

    /**
     * Constructor.
     */
    BufferBGFX();

    /**
     * Destructor.
     */
    ~BufferBGFX();

    /**
     * @see Buffer::getHostMemory
     */
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

}

