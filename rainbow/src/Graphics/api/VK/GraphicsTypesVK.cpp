#include "GraphicsTypesVK.h"

namespace rainbow
{

//------------------------------------------------------------------------------
// BufferVK
//------------------------------------------------------------------------------

BufferVK::BufferVK() :
    Buffer(),
    _hostMemory(nullptr),
    _deviceMemory(VK_NULL_HANDLE),
    _buffer(VK_NULL_HANDLE)
{
}

BufferVK::~BufferVK()
{
}

void* BufferVK::getHostMemory() const
{
    return _hostMemory;
}



//------------------------------------------------------------------------------
// RenderPipelineVK
//------------------------------------------------------------------------------

RenderPipelineVK::RenderPipelineVK() :
    RenderPipeline(),
    _pipelineLayout(VK_NULL_HANDLE),
    _pipeline(VK_NULL_HANDLE)
{
}

//------------------------------------------------------------------------------
// CommandBufferVK
//-----------------------------------------------------------------------------

CommandBufferVK::CommandBufferVK() :
    CommandBuffer(),
    _commandBuffer(VK_NULL_HANDLE)
{
}


//------------------------------------------------------------------------------
// SemaphoreVK
//-----------------------------------------------------------------------------

SemaphoreVK::SemaphoreVK() :
    Semaphore(),
    _semaphore(VK_NULL_HANDLE)
{
}

SemaphoreVK::~SemaphoreVK()
{
}



//------------------------------------------------------------------------------
// RenderPassVK
//-----------------------------------------------------------------------------

RenderPassVK::RenderPassVK() :
    RenderPass(),
    _device(nullptr),
    _renderPass(nullptr),
    _framebuffer(nullptr)
{
}

/*RenderPassVK::RenderPassVK(size_t width, size_t height,
                           size_t colorAttachmentCount,
                           PixelFormat colorFormat,
                           PixelFormat depthStencilFormat,
                           Texture::SampleCount sampleCount,
                           std::vector<std::shared_ptr<Texture>> colorAttachments,
                           std::vector<std::shared_ptr<Texture>> colorMultisampleAttachments,
                           std::shared_ptr<Texture> depthStencilAttachment,
                           VkDevice device,
                           VkRenderPass renderPass,
                           VkFramebuffer framebuffer) :
    RenderPass(width, height,
               colorAttachmentCount,
               colorFormat,
               depthStencilFormat,
               sampleCount,
               colorAttachments,
               colorMultisampleAttachments,
               depthStencilAttachment),
    _device(device),
    _renderPass(renderPass),
    _framebuffer(framebuffer)
{
}*/

RenderPassVK::~RenderPassVK()
{
}


//------------------------------------------------------------------------------
// DescriptorSetVK
//-----------------------------------------------------------------------------

DescriptorSetVK::DescriptorSetVK() :
    DescriptorSet(),
    _device(nullptr),
    _descriptorPool(nullptr),
    _descriptorSet(nullptr),
    _descriptorSetLayout(nullptr)
{
}

/*DescriptorSetVK::DescriptorSetVK(const Descriptor* descriptors,
                                 size_t descriptorCount,
                                 VkDevice device,
                                 VkDescriptorPool descriptorPool,
                                 VkDescriptorSetLayout descriptorSetLayout,
                                 VkDescriptorSet descriptorSet) :
    DescriptorSet(descriptors, descriptorCount),
    _device(device),
    _descriptorPool(descriptorPool),
    _descriptorSetLayout(descriptorSetLayout),
    _descriptorSet(descriptorSet)

{
}*/

DescriptorSetVK::~DescriptorSetVK()
{
}



//------------------------------------------------------------------------------
// TextureVK
//-----------------------------------------------------------------------------

TextureVK::TextureVK() :
    Texture(),
    _device(nullptr),
    _image(nullptr),
    _deviceMemory(nullptr),
    _hostMemory(nullptr)
{
}

/*TextureVK::TextureVK(Type type, size_t width, size_t height, size_t depth, size_t mipLevels,
                     PixelFormat pixelFormat,
                     Usage usage,
                     SampleCount sampleCount,
                     bool hostVisible,
                     bool hostOwned,
                     VkDevice device,
                     VkImage image,
                     VkDeviceMemory deviceMemory) :
    Texture(type, width, height, depth, mipLevels, pixelFormat, usage, sampleCount, hostOwned, hostVisible),
    _device(device),
    _image(image),
    _imageView(nullptr),
    _deviceMemory(deviceMemory),
    _hostMemory(nullptr)
{
}*/

TextureVK::~TextureVK()
{
}

void* TextureVK::getHostMemory() const
{
    return _hostMemory;
}


//------------------------------------------------------------------------------
// SamplerVK
//-----------------------------------------------------------------------------


SamplerVK::SamplerVK() :
    Sampler(),
    _device(nullptr),
    _sampler(nullptr)
{
}

/*SamplerVK::SamplerVK(Filter filterMin,
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
                     VkSampler sampler) :
    Sampler(filterMin,
            filterMag,
            filterMip,
            addressModeU,
            addressModeV,
            addressModeW,
            borderColor,
            compareEnabled,
            compareFunc,
            anisotropyEnabled,
            anisotropyMax,
            lodMin,
            lodMax,
            lodMipBias),
    _device(device),
    _sampler(sampler)
{
}*/

SamplerVK::~SamplerVK()
{
}


//------------------------------------------------------------------------------
// ShaderVK
//-----------------------------------------------------------------------------


ShaderVK::ShaderVK() :
    Shader(),
    _device(nullptr),
    _shaderModule(nullptr)
{
}

ShaderVK::ShaderVK(VkDevice device, VkShaderModule shaderModule) :
    Shader(),
    _device(device),
    _shaderModule(shaderModule)
{
}

ShaderVK::~ShaderVK()
{
}









}
