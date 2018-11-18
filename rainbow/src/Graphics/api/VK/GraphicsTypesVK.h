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

protected:
    void* _hostMemory;
    VkDeviceMemory _deviceMemory;
    VkBuffer _buffer;
    VkDescriptorBufferInfo _bufferView;
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
     * Constructor.
     */
    /*RenderPassVK(size_t width, size_t height,
                 size_t colorAttachmentCount,
                 PixelFormat colorFormat,
                 PixelFormat depthStencilFormat,
                 Texture::SampleCount sampleCount,
                 std::vector<std::shared_ptr<Texture>> colorAttachments,
                 std::vector<std::shared_ptr<Texture>> colorMultisampleAttachments,
                 std::shared_ptr<Texture> depthStencilAttachment,
                 VkDevice device,
                 VkRenderPass renderPass,
                 VkFramebuffer framebuffer);*/

    /**
     * Destructor.
     */
    ~RenderPassVK();

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
public:

    /**
     * Constructor.
     */
    DescriptorSetVK();

    /**
     * Constructor.
     */
   /* DescriptorSetVK(const Descriptor* descriptors,
                    size_t descriptorCount,
                    VkDevice device,
                    VkDescriptorPool descriptorPool,
                    VkDescriptorSetLayout descriptorSetLayout,
                    VkDescriptorSet descriptorSet);*/
    /**
     * Destructor.
     */
    ~DescriptorSetVK();

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
     * Constructor.
     */
    /*TextureVK(Type type, size_t width, size_t height, size_t depth, size_t mipLevels,
              PixelFormat pixelFormat,
              Usage usage,
              SampleCount sampleCount,
              bool hostVisible,
              bool hostOwned,
              VkDevice device,
              VkImage image,
              VkDeviceMemory deviceMemory);*/

    /**
     * Destructor.
     */
    ~TextureVK();

    /**
     * @see Buffer::getHostMemory
     */
    void* getHostMemory() const;

    VkDevice _device;
    VkImage _image;
    VkImageView  _imageView;
    VkDeviceMemory _deviceMemory;
    void* _hostMemory;
    VkImageAspectFlags  _imageAspectFlags;
    VkDescriptorImageInfo _imageViewInfo;
};



//------------------------------------------------------------------------------
// SamplerVK
//-----------------------------------------------------------------------------

/**
 * Vulkan impl of a Sampler.
 */
class SamplerVK : public Sampler
{
public:

    /**
     * Constructor.
     */
    SamplerVK();

    /**
     * Constructor.
     */
    /*SamplerVK(Filter filterMin,
              Filter filterMag,
              Filter filterMip,
              AddressMode addressModeU,
              AddressMode addressModeV,
              AddressMode addressModeW,
              BorderColor borderColor,
              bool compareEnabled,
              CompareFunc compareFunc,
              bool anisotropyEnabled,
              float anisotropyMax,
              float lodMin,
              float lodMax,
              float lodMipBias,
              VkDevice device,
              VkSampler sampler);*/

    /**
     * Destructor.
     */
    ~SamplerVK();

    VkDevice _device;
    VkSampler _sampler;
};



//------------------------------------------------------------------------------
// SamplerVK
//-----------------------------------------------------------------------------

/**
 * Vulkan impl of a Shader.
 */
class ShaderVK : public Shader
{
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

    VkDevice _device;
    VkShaderModule _shaderModule;
};



} // end namespace rainbow

