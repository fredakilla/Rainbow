#include "../Core/Base.h"
#include "GraphicsTypes.h"

namespace rainbow
{

VertexLayout::VertexLayout()
{
}

VertexLayout::VertexLayout(const Attribute* attributes, size_t attributeCount) :
    _stride(0)
{
    GP_ASSERT(attributeCount);
    GP_ASSERT(attributeCount < GP_GRAPHICS_VERTEX_ATTRIBUTES_MAX);

    for (size_t i = 0; i < attributeCount; ++i)
    {
        Attribute attribute;
        memcpy(&attribute, &attributes[i], sizeof(Attribute));
        _attributes.push_back(attribute);
        _stride += toStride(_attributes[i].format);
    }
}

VertexLayout::~VertexLayout()
{
}

const VertexLayout::Attribute& VertexLayout::getAttribute(size_t index) const
{
    GP_ASSERT(index < _attributes.size());
    return _attributes[index];
}

size_t VertexLayout::getAttributeCount() const
{
    return _attributes.size();
}

size_t VertexLayout::getStride() const
{
    return _stride;
}

bool VertexLayout::operator == (const VertexLayout& layout) const
{
    if (_attributes.size() != layout._attributes.size())
        return false;

    for (size_t i = 0, count = _attributes.size(); i < count; ++i)
    {
        if (_attributes[i] != layout._attributes[i])
            return false;
    }
    return true;
}

bool VertexLayout::operator != (const VertexLayout& layout) const
{
    return !(*this == layout);
}

VertexLayout::Attribute::Attribute() :
    semantic(Semantic::ePosition),
    binding(0),
    location(0),
    offset(0)
{
}

VertexLayout::Attribute::Attribute(Semantic semantic,
                                   VertexFormat format,
                                   uint32_t binding,
                                   uint32_t location,
                                   uint32_t offset) :
    semantic(semantic),
    format(format),
    binding(binding),
    location(location),
    offset(offset)
{
}

VertexLayout::Attribute::~Attribute()
{
}

bool VertexLayout::Attribute::operator == (const VertexLayout::Attribute& attr) const
{
    return (semantic == attr.semantic) &&
           (format == attr.format) &&
           (binding == attr.binding) &&
           (location == attr.location) &&
           (offset == attr.offset);
}

bool VertexLayout::Attribute::operator != (const VertexLayout::Attribute& attr) const
{
    return !(*this == attr);
}

size_t VertexLayout::toStride(VertexFormat format)
{
    switch (format)
    {
        case VertexFormat::eFloat:
            return 4;
        case VertexFormat::eFloat2:
            return 8;
        case VertexFormat::eFloat3:
            return 12;
        case VertexFormat::eFloat4:
            return 16;
        case VertexFormat::eByte4:
        case VertexFormat::eByte4N:
        case VertexFormat::eUByte4:
        case VertexFormat::eUByte4N:
        case VertexFormat::eShort2:
        case VertexFormat::eShort2N:
            return 4;
        case VertexFormat::eShort4:
        case VertexFormat::eShort4N:
            return 8;
        case VertexFormat::eUndefined:
            return 0;        
    }
    return 0;
}

//------------------------------------------------------------------------------
// Texture
//------------------------------------------------------------------------------

Texture::Texture() :
    _type(Type::e2D),
    _width(0),
    _height(0),
    _depth(0),
    _mipLevels(1),
    _usage(Usage::eNone),
    _sampleCount(SampleCount::e1X),
    _hostVisible(true),
    _hostMemory(nullptr),
    _hostOwned(true)
{
}

Texture::~Texture()
{
}

Texture::Type Texture::getType() const
{
    return _type;
}

size_t Texture::getWidth() const
{
    return _width;
}

size_t Texture::getHeight() const
{
    return _height;
}

size_t Texture::getDepth() const
{
    return _depth;
}

size_t Texture::getMipLevels() const
{
    return _mipLevels;
}

Texture::Usage Texture::getUsage() const
{
    return _usage;
}

PixelFormat Texture::getPixelFormat() const
{
    return _pixelFormat;
}

Texture::SampleCount Texture::getSampleCount() const
{
    return _sampleCount;
}

bool Texture::isHostVisible() const
{
    return _hostVisible;
}

void* Texture::getHostMemory() const
{
    return _hostMemory;
}

bool Texture::isHostOwned() const
{
    return _hostOwned;
}

//------------------------------------------------------------------------------
// Sampler
//------------------------------------------------------------------------------

Sampler::Sampler() :
    _filterMag(Filter::eNearest),
    _filterMin(Filter::eNearest),
    _filterMip(Filter::eNearest),
    _addressModeU(AddressMode::eWrap),
    _addressModeV(AddressMode::eWrap),
    _addressModeW(AddressMode::eWrap),
    _borderColor(BorderColor::eBlackOpaque),
    _compareEnabled(true),
    _compareFunc(CompareFunc::eNever),
    _anisotropyEnabled(false),
    _anisotropyMax(0.0f),
    _lodMin(0.0f),
    _lodMax(0.0f),
    _lodMipBias(0.0f)
{
}

Sampler::~Sampler()
{
}

Sampler::Filter Sampler::getFilterMin() const
{
    return _filterMin;
}

Sampler::Filter Sampler::getFilterMag() const
{
    return _filterMag;
}

Sampler::Filter Sampler::getFilterMip() const
{
    return _filterMip;
}

Sampler::AddressMode Sampler::getAddressModeU() const
{
    return _addressModeU;
}

Sampler::AddressMode Sampler::getAddressModeV() const
{
    return _addressModeV;
}

Sampler::AddressMode Sampler::getAddressModeW() const
{
    return _addressModeW;
}

Sampler::BorderColor Sampler::getBorderColor() const
{
    return _borderColor;
}

bool Sampler::isCompareEnabled() const
{
    return _compareEnabled;
}

Sampler::CompareFunc Sampler::getCompareFunc() const
{
    return _compareFunc;
}

bool Sampler::isAnisotropyEnabled() const
{
    return _anisotropyEnabled;
}

float Sampler::getAnisotropyMax() const
{
    return _anisotropyMax;
}

float Sampler::getLodMin() const
{
    return _lodMin;
}

float Sampler::getLodMax() const
{
    return _lodMax;
}

float Sampler::getLodMipBias() const
{
    return _lodMipBias;
}

//------------------------------------------------------------------------------
// RenderPass
//------------------------------------------------------------------------------

RenderPass::RenderPass() :
    _width(0),
    _height(0),
    _colorAttachmentCount(0),
    _depthStencilFormat(PixelFormat::eUndefined),
    _sampleCount(Texture::SampleCount::e1X)
{
}

RenderPass::~RenderPass()
{
    _colorFormats.clear();
    _colorAttachments.clear();
}

size_t RenderPass::getWidth() const
{
    return _width;
}

size_t RenderPass::getHeight() const
{
    return _height;
}

size_t RenderPass::getColorAttachmentCount() const
{
    return _colorAttachmentCount;
}

PixelFormat RenderPass::getColorFormat(size_t index) const
{
    return _colorFormats[index];
}

PixelFormat RenderPass::getDepthStencilFormat() const
{
    return _depthStencilFormat;
}

Texture::SampleCount RenderPass::getSampleCount() const
{
    return _sampleCount;
}

std::shared_ptr<Texture> RenderPass::getColorAttachment(size_t index)
{
    return _colorAttachments[index];
}

std::shared_ptr<Texture> RenderPass::getColorMultisampleAttachment(size_t index)
{
    return _colorMultisampleAttachments[index];
}

std::shared_ptr<Texture> RenderPass::getDepthStencilAttachment()
{
    return _depthStencilAttachment;
}

//------------------------------------------------------------------------------
// Buffer
//------------------------------------------------------------------------------

Buffer::Buffer() :
    _usage(Usage::eVertex),
    _size(0),
    _stride(0),
    _hostVisible(true),
    _mapped(nullptr)
{
}

Buffer::~Buffer()
{
}

Buffer::Usage Buffer::getUsage() const
{
    return _usage;
}

size_t Buffer::getSize() const
{
    return _size;
}

size_t Buffer::getStride() const
{
    return _stride;
}

bool Buffer::isHostVisible() const
{
    return _hostVisible;
}

//------------------------------------------------------------------------------
// DescriptorSet
//------------------------------------------------------------------------------

DescriptorSet::Descriptor::Descriptor()
{
}

DescriptorSet::Descriptor::~Descriptor()
{
}

DescriptorSet::DescriptorSet()
{
}

DescriptorSet::DescriptorSet(const Descriptor* descriptors, size_t descriptorCount)
{
    GP_ASSERT(descriptors);
    GP_ASSERT(descriptorCount < GP_GRAPHICS_VERTEX_ATTRIBUTES_MAX);

    _descriptors.resize(descriptorCount);
    for (size_t i = 0; i < descriptorCount; ++i)
    {
        memcpy(&_descriptors[i], &descriptors[i], sizeof(Descriptor));
    }
}

DescriptorSet::~DescriptorSet()
{
}

size_t DescriptorSet::getDescriptorCount() const
{
    return _descriptors.size();
}

DescriptorSet::Descriptor DescriptorSet::getDescriptor(size_t index) const
{
    GP_ASSERT(index < _descriptors.size());
    return _descriptors[index];
}

//------------------------------------------------------------------------------
// Shader
//------------------------------------------------------------------------------

Shader::Shader()
{
}

Shader::~Shader()
{
}

//------------------------------------------------------------------------------
// RenderPipeline
//------------------------------------------------------------------------------

RenderPipeline::RenderPipeline() :
    _renderPass(nullptr),
    _descriptorSet(nullptr),
    _vertShader(nullptr),
    _tescShader(nullptr),
    _geomShader(nullptr),
    _fragShader(nullptr)
{
}

RenderPipeline::~RenderPipeline()
{
}

RenderPipeline::PrimitiveTopology RenderPipeline::getPrimitiveTopology() const
{
    return _primitiveTopology;
}

VertexLayout RenderPipeline::getVertexLayout() const
{
    return _vertexLayout;
}

RasterizerState RenderPipeline::getRasterizerState() const
{
    return _rasterizerState;
}

ColorBlendState RenderPipeline::getColorBlendState() const
{
    return _colorBlendState;
}

DepthStencilState RenderPipeline::getDepthStencilState() const
{
    return _depthStencilState;
}

std::shared_ptr<RenderPass> RenderPipeline::getRenderPass() const
{
    return _renderPass;
}

std::shared_ptr<DescriptorSet> RenderPipeline::getDescriptorSet() const
{
    return _descriptorSet;
}

std::shared_ptr<Shader> RenderPipeline::getVertexShader() const
{
    return _vertShader;
}

std::shared_ptr<Shader> RenderPipeline::getTessellationControlShader() const
{
    return _tescShader;
}

std::shared_ptr<Shader> RenderPipeline::getTessellationEvaluationShader() const
{
    return _teseShader;
}

std::shared_ptr<Shader> RenderPipeline::getGeometryShader() const
{
    return _geomShader;
}

std::shared_ptr<Shader> RenderPipeline::getFragmentShader() const
{
    return _fragShader;
}

} // end namespace rainbow
