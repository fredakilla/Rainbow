#include "../Core/Base.h"
#include "GraphicsTypes.h"

namespace rainbow
{

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
//-----------------------------------------------------------------------------

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


} // end namespace rainbow
