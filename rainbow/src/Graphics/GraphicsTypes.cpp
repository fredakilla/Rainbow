#include "../Core/Base.h"
#include "GraphicsTypes.h"

namespace rainbow
{

#if 0

static const int32_t INVALID_INDEX = -1;


//------------------------------------------------------------------------------
// IndexType
//------------------------------------------------------------------------------

int IndexType::byteSize(IndexType::Enum c)
{
    switch (c)
    {
        case None:      return 0;
        case Index16:   return 2;
        case Index32:   return 4;
        default:
            GP_ERROR("IndexType::ByteSize() called with invalid type!\n");
            return 0;
    }
}

//------------------------------------------------------------------------------
// VertexAttr
//------------------------------------------------------------------------------

const char* VertexAttr::toString(Enum c)
{
    switch (c)
    {
    case Position:      return "position";
    case Normal:        return "normal";
    case Color0:        return "color0";
    case Color1:        return "color1";
    case Color2:        return "color2";
    case Color3:        return "color3";
    case Tangent:       return "tangent";
    case Bitangent:     return "bitangent";
    case Indices:       return "indices";
    case Weight:        return "weight";
    case TexCoord0:     return "texcoord0";
    case TexCoord1:     return "texcoord1";
    case TexCoord2:     return "texcoord2";
    case TexCoord3:     return "texcoord3";
    case TexCoord4:     return "texcoord4";
    case TexCoord5:     return "texcoord5";
    case TexCoord6:     return "texcoord6";
    case TexCoord7:     return "texcoord7";
    //case Instance0:     return "instance0";
    //case Instance1:     return "instance1";
    //case Instance2:     return "instance2";
    //case Instance3:     return "instance3";
    default:
        GP_ERROR("VertexAttr::ToString(): invalid value!\n");
        return nullptr;
    }
}

VertexAttr::Enum VertexAttr::fromString(const char* str)
{
    if (str)
    {
        if      (0 == std::strcmp("position"    , str)) return Position;
        else if (0 == std::strcmp("normal"      , str)) return Normal;
        else if (0 == std::strcmp("color0"      , str)) return Color0;
        else if (0 == std::strcmp("color1"      , str)) return Color1;
        else if (0 == std::strcmp("color2"      , str)) return Color2;
        else if (0 == std::strcmp("color3"      , str)) return Color3;
        else if (0 == std::strcmp("tangent"     , str)) return Tangent;
        else if (0 == std::strcmp("bitangent"   , str)) return Bitangent;
        else if (0 == std::strcmp("indices"     , str)) return Indices;
        else if (0 == std::strcmp("weight"      , str)) return Weight;
        else if (0 == std::strcmp("texcoord0"   , str)) return TexCoord0;
        else if (0 == std::strcmp("texcoord1"   , str)) return TexCoord1;
        else if (0 == std::strcmp("texcoord2"   , str)) return TexCoord2;
        else if (0 == std::strcmp("texcoord3"   , str)) return TexCoord3;
        else if (0 == std::strcmp("texcoord4"   , str)) return TexCoord4;
        else if (0 == std::strcmp("texcoord5"   , str)) return TexCoord5;
        else if (0 == std::strcmp("texcoord6"   , str)) return TexCoord6;
        else if (0 == std::strcmp("texcoord7"   , str)) return TexCoord7;
        //else if (0 == std::strcmp("instance0"   , str)) return Instance0;
        //else if (0 == std::strcmp("instance1"   , str)) return Instance1;
        //else if (0 == std::strcmp("instance2"   , str)) return Instance2;
        //else if (0 == std::strcmp("instance3"   , str)) return Instance3;
    }
    return InvalidVertexAttr;
}

//------------------------------------------------------------------------------
// VertexFormat
//------------------------------------------------------------------------------

int VertexFormat::byteSize(Enum c)
{
    switch (c)
    {
        case Float:
            return 4;
        case Float2:
            return 8;
        case Float3:
            return 12;
        case Float4:
            return 16;
        case Byte4:
        case Byte4N:
        case UByte4:
        case UByte4N:
        case Short2:
        case Short2N:
        case UInt10_2N:
            return 4;
        case Short4:
        case Short4N:
            return 8;
        default:
            GP_ERROR("VertexFormat::ByteSize() called with invalid format!\n");
            return 0;
    }
}

const char* VertexFormat::toString(Enum c)
{
    switch (c)
    {
        case Float:     return "Float";
        case Float2:    return "Float2";
        case Float3:    return "Float3";
        case Float4:    return "Float4";
        case Byte4:     return "Byte4";
        case Byte4N:    return "Byte4N";
        case UByte4:    return "UByte4";
        case UByte4N:   return "Ubyte4N";
        case Short2:    return "Short2";
        case Short2N:   return "Short2N";
        case Short4:    return "Short4";
        case Short4N:   return "Short4N";
        case UInt10_2N: return "Int10_2N";
        default:
            GP_ERROR("VertexFormat::ToString(): invalid value!\n");
            return nullptr;
    }
}

//------------------------------------------------------------------------------
// VertexLayout
//------------------------------------------------------------------------------

bool VertexLayout::Component::isValid() const
{
    return (VertexAttr::InvalidVertexAttr != this->attr);
}

void VertexLayout::Component::clear()
{
    this->attr = VertexAttr::InvalidVertexAttr;
    this->format = VertexFormat::InvalidVertexFormat;
}

int VertexLayout::Component::byteSize() const
{
    return VertexFormat::byteSize(this->format);
}

VertexLayout::VertexLayout()
{
    this->clear();
}

VertexLayout::VertexLayout(std::initializer_list<Component> l)
{
    this->clear();
    for (const auto& c : l) {
        this->add(c);
    }
}

VertexLayout& VertexLayout::clear()
{
    this->stepFunction = VertexStepFunction::PerVertex;
    this->stepRate = 1;
    this->_numComps = 0;
    this->_byteSize = 0;
    this->_attrCompIndices.fill(INVALID_INDEX);
    this->_byteOffsets.fill(0);
    return *this;
}

bool VertexLayout::empty() const
{
    return 0 == this->_numComps;
}

VertexLayout& VertexLayout::add(const Component& comp)
{
    GP_ASSERT(this->_numComps < GraphicsConfig::MaxNumVertexLayoutComponents);
    GP_ASSERT(INVALID_INDEX == this->_attrCompIndices[comp.attr]);
    this->_comps[this->_numComps] = comp;
    this->_attrCompIndices[comp.attr] = this->_numComps;
    this->_byteOffsets[this->_numComps] = this->_byteSize;
    this->_byteSize += comp.byteSize();
    GP_ASSERT(this->_byteSize < 248);
    this->_numComps++;
    return *this;
}

VertexLayout& VertexLayout::add(VertexAttr::Enum attr, VertexFormat::Enum format)
{
    return this->add(Component(attr, format));
}

VertexLayout& VertexLayout::add(std::initializer_list<Component> l)
{
    for (const auto& c : l) {
        this->add(c);
    }
    return *this;
}

VertexLayout& VertexLayout::enableInstancing()
{
    this->stepFunction = VertexStepFunction::PerInstance;
    this->stepRate = 1;
    return *this;
}

size_t VertexLayout::getComponentCount() const
{
    return this->_numComps;
}

const VertexLayout::Component& VertexLayout::getComponent(size_t index) const
{
    return this->_comps[index];
}

int VertexLayout::componentIndexByVertexAttr(VertexAttr::Enum attr) const
{
    return this->_attrCompIndices[attr];
}

int VertexLayout::byteSize() const
{
    return this->_byteSize;
}

int VertexLayout::componentByteOffset(int componentIndex) const
{
    return this->_byteOffsets[componentIndex];
}

bool VertexLayout::contains(VertexAttr::Enum attr) const
{
    return INVALID_INDEX != this->componentIndexByVertexAttr(attr);
}

//------------------------------------------------------------------------------
// PrimitiveType
//------------------------------------------------------------------------------

const char* PrimitiveType::ToString(PrimitiveType::Enum c) {
    switch (c)
    {
        case Points:        return "Points";
        case Lines:         return "Lines";
        case LineStrip:     return "LineStrip";
        case Triangles:     return "Triangles";
        case TriangleStrip: return "TriangleStrip";
        default:
            GP_ERROR("PrimitiveType::ToString(): invalid value!\n");
            return 0;
    }
}

#endif







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
        default:
            GP_ERROR("VertexLayout::toStride() called with invalid format!\n");
            return 0;
    }
    return 0;


    /*switch (format)
    {
    case Format::eR8Unorm:
        return 1;
    case Format::eR16Unorm:
        return 2;
    case Format::eR16Float:
        return 2;
    case Format::eR32Uint:
        return 4;
    case Format::eR32Float:
        return 4;
    case Format::eR8G8Unorm:
        return 2;
    case Format::eR16G16Unorm:
        return 2;
    case Format::eR16G16Float:
        return 4;
    case Format::eR32G32Uint:
        return 8;
    case Format::eR32G32Float:
        return 8;
    case Format::eR32G32B32Uint:
        return 12;
    case Format::eR32G32B32Float:
        return 12;
    case Format::eB8G8R8A8Unorm:
        return 4;
    case Format::eR8G8B8A8Unorm:
        return 4;
    case Format::eR16G16B16A16Unorm:
        return 8;
    case Format::eR16G16B16A16Float:
        return 8;
    case Format::eR32G32B32A32Uint:
        return 16;
    case Format::eR32G32B32A32Float:
        return 16;
    case Format::eUndefined:
    case Format::eD16Unorm:
    case Format::eX8D24UnormPack32:
    case Format::eD32Float:
    case Format::eS8Uint:
    case Format::eD24UnormS8Uint:
    case Format::eD32FloatS8Uint:
        return 0;
    }
    return 0;*/
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
    _hostVisible(true)
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
