#pragma once

namespace rainbow
{

//------------------------------------------------------------------------------
// GraphicsConfig
//------------------------------------------------------------------------------
/**
 * @class GraphicsConfig
 * @brief central configuration constants of Graphics
 */
class GraphicsConfig
{
public:
    /// maximum number of components in vertex layout
    static const int MaxNumVertexLayoutComponents = 16;
};

//------------------------------------------------------------------------------
// IndexType
//------------------------------------------------------------------------------
/**
    @class IndexType
    @brief selects 16- or 32-bit indices
*/
class IndexType {
public:
    enum Enum
    {
        None = 0,
        Index16,
        Index32,

        NumIndexTypes,
        InvalidIndexType = 0xFFFFFFFF
    };
    /// get byte size of index type
    static int byteSize(IndexType::Enum c);
};


//------------------------------------------------------------------------------
// VertexAttr
//------------------------------------------------------------------------------
/**
    @class VertexAttr
    @brief vertex attribute enum (position, texcoord, ...)

    The VertexAttr definitions don't have a hardwired meaning, they just
    exist to make the binding of vertex components (living in vertex buffers)
    to vertex attribute definition in vertex shaders easier to understand.
    The maximum number of vertex attributes should not exceed 16
    (this is the GL_MAX_VERTEX_ATTRIBS value).
*/
class VertexAttr
{
public:
    enum Enum : uint8_t
    {
        Position = 0,   ///< "position"
        Normal,         ///< "normal"
        Color0,         ///< "color0"
        Color1,         ///< "color1"
        Color2,         ///< "color2"
        Color3,         ///< "color3"
        Tangent,        ///< "tangent
        Bitangent,      ///< "bitangent"
        Indices,        ///< "indices" (skin indices)
        Weight,         ///< "weight" (skin weight)
        TexCoord0,      ///< "texcoord0"
        TexCoord1,      ///< "texcoord1"
        TexCoord2,      ///< "texcoord2"
        TexCoord3,      ///< "texcoord3"
        TexCoord4,      ///< "texcoord4"
        TexCoord5,      ///< "texcoord5"
        TexCoord6,      ///< "texcoord6"
        TexCoord7,      ///< "texcoord7"
        //Instance0,      ///< "instance0"
        //Instance1,      ///< "instance1"
        //Instance2,      ///< "instance2"
        //Instance3,      ///< "instance3"

        NumVertexAttrs,
        InvalidVertexAttr,
    };

    /// convert to string
    static const char* toString(Enum c);
    /// convert from string
    static Enum fromString(const char* str);
};


//------------------------------------------------------------------------------
// VertexFormat
//------------------------------------------------------------------------------
/**
    @class VertexFormat
    @brief vertex component formats
    @see Vertex

    NOTE: The un-normalized integer formats are not compatible across
    GLES2 and D3D11! GLES2 needs to read those as float vec, but D3D11
    can only read them as int vec!
*/
class VertexFormat
{
public:
    /// format enum (don't change order, and append to end!)
    enum Enum : uint8_t
    {
        Float,          ///< single component float, expanded to (x, 0, 0, 1)
        Float2,         ///< 2-component float, expanded to (x, y, 0, 1)
        Float3,         ///< 3-component float, expanded to (x, y, z, 1)
        Float4,         ///< 4-component float
        Byte4,          ///< 4-component float (-128.0f..+127.0f) mapped to byte (-128..+127)
        Byte4N,         ///< 4-component float (-1.0f..+1.0f) mapped to byte (-128..+127)
        UByte4,         ///< 4-component float (0.0f..255.0f) mapped to byte (0..255)
        UByte4N,        ///< 4-component float (0.0f..+1.0) mapped to byte (-128..+127)
        Short2,         ///< 2-component float (-32768.0f..+32767.0f) mapped to short (-32768..+32767)
        Short2N,        ///< 2-component float (-1.0f..+1.0f) mapped to short (-32768..+32767)
        Short4,         ///< 4-component float (-32768.0f..+32767.0f) mapped to short (-32768..+32767)
        Short4N,        ///< 4-component float (-1.0f..+1.0f) mapped to short (-32768..+32767)
        UInt10_2N,      ///< 4-component packed, normalized 10-bit XYZ, 2-bit W (0.0 .. 1.0)

        NumVertexFormats,       ///< number of vertex formats
        InvalidVertexFormat,    ///< the invalid vertex format value
    };

    /// get the byte size of a vertex format code
    static int byteSize(Enum c);
    /// convert to string
    static const char* toString(Enum c);
};


//------------------------------------------------------------------------------
// VertexStepFunction
//------------------------------------------------------------------------------
/**
    @class VertexStepFunction
    @brief classify vertices in a buffer as per-vertex or per-instance data
*/
class VertexStepFunction
{
public:
    enum Enum : uint8_t
    {
        PerVertex = 0,
        PerInstance = 1,
    };
};

//------------------------------------------------------------------------------
// VertexLayout
//------------------------------------------------------------------------------
/**
    @class :VertexLayout
    @brief describes the data layout of a vertex in a vertex buffer
*/
class VertexLayout
{
public:
    /// a component in a vertex layout
    #pragma pack(push,1)
    class Component
    {
    public:
        /// default constructor
        Component() {}
        /// construct from vertex attr and format
        Component(VertexAttr::Enum attr, VertexFormat::Enum fmt) : attr(attr), format(fmt) {}
        /// return true if valid (attr and format set)
        bool isValid() const;
        /// clear the component (unset attr and format)
        void clear();
        /// get byte size of component
        int byteSize() const;

        VertexAttr::Enum attr = VertexAttr::InvalidVertexAttr;
        VertexFormat::Enum format = VertexFormat::InvalidVertexFormat;
    };
    #pragma pack(pop)

    /// the vertex step function, used for instancing, default is 'PerVertex'
    VertexStepFunction::Enum stepFunction = VertexStepFunction::PerVertex;
    /// the vertex step rate, used for instancing
    uint8_t stepRate = 1;

    /// default constructor
    VertexLayout();
    /// initializer list constructor
    VertexLayout(std::initializer_list<Component> l);
    /// clear the vertex layout, chainable
    VertexLayout& clear();
    /// return true if layout is empty
    bool empty() const;
    /// add a component
    VertexLayout& add(const Component& comp);
    /// add component by name and format
    VertexLayout& add(VertexAttr::Enum attr, VertexFormat::Enum format);
    /// add multiple components via initializer list
    VertexLayout& add(std::initializer_list<Component> l);
    /// enable layout for instancing, set StepFunction to PerInstance and StepRate to 1
    VertexLayout& enableInstancing();
    /// get number of components
    size_t getComponentCount() const;
    /// get component at index
    const Component& getComponent(size_t index) const;
    /// get component index by vertex attribute, return InvalidIndex if layout doesn't include attr
    int componentIndexByVertexAttr(VertexAttr::Enum attr) const;
    /// get byte size of vertex (aka stride)
    int byteSize() const;
    /// get byte offset of a component
    int componentByteOffset(int componentIndex) const;
    /// test if the layout contains a specific vertex attribute
    bool contains(VertexAttr::Enum attr) const;

private:
    std::array<Component, GraphicsConfig::MaxNumVertexLayoutComponents> _comps;
    std::array<uint8_t, GraphicsConfig::MaxNumVertexLayoutComponents> _byteOffsets;
    std::array<int8_t, VertexAttr::NumVertexAttrs> _attrCompIndices;  // maps vertex attributes to component indices
    uint8_t _numComps = 0;
    uint8_t _byteSize = 0;
};

}
