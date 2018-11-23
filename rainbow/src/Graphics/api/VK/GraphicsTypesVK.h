#pragma once

#include "../../GraphicsTypes.h"
#include <vulkan/vulkan.h>

namespace rainbow
{

//------------------------------------------------------------------------------
// BufferVK
//------------------------------------------------------------------------------
/**
 * Vulkan impl of a Buffer.
 */
class BufferVK : public Buffer
{
    friend class GraphicsVK;

public:
    //! Constructor
    BufferVK();
    //! Destructor
    ~BufferVK();
    //! @see Buffer::getHostMemory
    void* getHostMemory() const override;
    //! @see Buffer::map
    void* map(uint64_t size = VK_WHOLE_SIZE, uint64_t offset = 0) override;
    ///! @see Buffer::unmap
    void unmap();

protected:
    void* _hostMemory;
    VkDeviceMemory _deviceMemory;
    VkBuffer _buffer;
    VkDescriptorBufferInfo _bufferView;
    VkDevice _device;

};

//------------------------------------------------------------------------------
// RenderPipelineVK
//------------------------------------------------------------------------------
/**
 * Vulkan impl of a RenderPipeline.
 */
class RenderPipelineVK : public RenderPipeline
{
    friend class GraphicsVK;

public:
    //! Constructor
    RenderPipelineVK();

private:
    VkPipelineLayout _pipelineLayout;
    VkPipeline _pipeline;
};

//------------------------------------------------------------------------------
// CommandBufferVK
//------------------------------------------------------------------------------
/**
 * Vulkan impl of a RenderPipeline.
 */
class CommandBufferVK : public CommandBuffer
{
    friend class GraphicsVK;

public:
    CommandBufferVK();

private:
    VkCommandBuffer _commandBuffer;
};

//------------------------------------------------------------------------------
// SemaphoreVK
//-----------------------------------------------------------------------------
class SemaphoreVK : public Semaphore
{
    friend class GraphicsVK;

public:
    SemaphoreVK();
    ~SemaphoreVK();

private:
    VkSemaphore _semaphore;
};

//------------------------------------------------------------------------------
// RenderPassVK
//-----------------------------------------------------------------------------
/**
 * Vulkan impl of a RenderPass.
 */
class RenderPassVK : public RenderPass
{
    friend class GraphicsVK;

public:

    /**
     * Constructor.
     */
    RenderPassVK();
    /**
     * Destructor.
     */
    ~RenderPassVK();

private:
    VkDevice _device;
    VkRenderPass _renderPass;
    VkFramebuffer _framebuffer;
};

//------------------------------------------------------------------------------
// DescriptorSetVK
//-----------------------------------------------------------------------------
/**
 * Vulkan impl of a DesctiptorSet.
 */
class DescriptorSetVK : public DescriptorSet
{
    friend class GraphicsVK;

public:

    /**
     * Constructor.
     */
    DescriptorSetVK();

    /**
     * Destructor.
     */
    ~DescriptorSetVK();

private:
    VkDevice _device;
    VkDescriptorPool _descriptorPool;
    VkDescriptorSetLayout _descriptorSetLayout;
    VkDescriptorSet _descriptorSet;
};

//------------------------------------------------------------------------------
// TextureVK
//-----------------------------------------------------------------------------
/**
 * Vulkan impl of a Texture.
 */
class TextureVK : public Texture
{
    friend class GraphicsVK;

public:

    /**
     * Constructor.
     */
    TextureVK();    

    /**
     * Destructor.
     */
    ~TextureVK();

    /**
     * @see Buffer::getHostMemory
     */
    void* getHostMemory() const;

private:
    VkDevice _device;
    VkImage _image;
    VkImageView  _imageView;
    VkDeviceMemory _deviceMemory;
    void* _hostMemory;
    VkImageAspectFlags  _imageAspectFlags;
    VkDescriptorImageInfo _imageViewInfo;

    //! Optional sampler to use with this texture
    VkSampler _sampler;
};

//------------------------------------------------------------------------------
// SamplerVK
//-----------------------------------------------------------------------------

/**
 * Vulkan impl of a Sampler.
 */
class SamplerVK : public Sampler
{
    friend class GraphicsVK;

public:

    /**
     * Constructor.
     */
    SamplerVK();

    /**
     * Destructor.
     */
    ~SamplerVK();

private:
    VkDevice _device;
    VkSampler _sampler;
};

//------------------------------------------------------------------------------
// ShaderVK
//-----------------------------------------------------------------------------
/**
 * Vulkan impl of a Shader.
 */
class ShaderVK : public Shader
{
    friend class GraphicsVK;

public:

    /**
     * Constructor.
     */
    ShaderVK();

    /**
     * Constructor.
     */
    ShaderVK(VkDevice device, VkShaderModule shaderModule);

    /**
     * Destructor.
     */
    ~ShaderVK();

private:
    VkDevice _device;
    VkShaderModule _shaderModule;
};

} // end namespace rainbow

