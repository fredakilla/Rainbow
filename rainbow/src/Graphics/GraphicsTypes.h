#pragma once

#include "../Core/Base.h"

namespace rainbow
{

#if 0

//------------------------------------------------------------------------------
// GraphicsConfig
//------------------------------------------------------------------------------
/**
 * Central configuration constants of Graphics
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
 * IndexType
 * Selects 16- or 32-bit indices
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
 * VertexAttr
 * Vertex attribute enum (position, texcoord, ...)
 *
 * The VertexAttr definitions don't have a hardwired meaning, they just
 * exist to make the binding of vertex components (living in vertex buffers)
 * to vertex attribute definition in vertex shaders easier to understand.
 * The maximum number of vertex attributes should not exceed 16
 * (this is the GL_MAX_VERTEX_ATTRIBS value).
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
 * VertexFormat
 * vertex component formats
 * @see Vertex
 *
 * NOTE: The un-normalized integer formats are not compatible across
 * GLES2 and D3D11! GLES2 needs to read those as float vec, but D3D11
 * can only read them as int vec!
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
 * VertexStepFunction
 * classify vertices in a buffer as per-vertex or per-instance data
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
 * VertexLayout
 * describes the data layout of a vertex in a vertex buffer
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

//------------------------------------------------------------------------------
// PrimitiveType
//------------------------------------------------------------------------------
/**
 * Primitive type enum (triangle strips, lists, etc...)
 */
class PrimitiveType
{
public:
    /// primitive type enum (don't change order, append to end!)
    enum Enum
    {
        Points = 0,
        Lines,
        LineStrip,
        Triangles,
        TriangleStrip,

        NumPrimitiveTypes,
        InvalidPrimitiveType = 0xFFFFFFFF,
    };

    /// convert primitive type to string
    static const char* ToString(Enum c);
};

#endif















/**
 * Defines a layout of vertex attributes.
 */
class VertexLayout
{
public:

    /**
     * Defines semantics for vertex attributes.
     */
    enum class Semantic
    {
        ePosition,
        eNormal,
        eColor,
        eColor0,
        eColor1,
        eColor2,
        eColor3,
        eTangent,
        eBitangent,
        eTexCoord,
        eTexCoord0,
        eTexCoord1,
        eTexCoord2,
        eTexCoord3,
        eTexCoord4,
        eTexCoord5,
        eTexCoord6,
        eTexCoord7
    };

    /**
     * Defines vertex format for semantics.
     */
    enum class VertexFormat
    {
        eFloat,         ///< single component float, expanded to (x, 0, 0, 1)
        eFloat2,        ///< 2-component float, expanded to (x, y, 0, 1)
        eFloat3,        ///< 3-component float, expanded to (x, y, z, 1)
        eFloat4,        ///< 4-component float
        eByte4,         ///< 4-component float (-128.0f..+127.0f) mapped to byte (-128..+127)
        eByte4N,        ///< 4-component float (-1.0f..+1.0f) mapped to byte (-128..+127)
        eUByte4,        ///< 4-component float (0.0f..255.0f) mapped to byte (0..255)
        eUByte4N,       ///< 4-component float (0.0f..+1.0) mapped to byte (-128..+127)
        eShort2,        ///< 2-component float (-32768.0f..+32767.0f) mapped to short (-32768..+32767)
        eShort2N,       ///< 2-component float (-1.0f..+1.0f) mapped to short (-32768..+32767)
        eShort4,        ///< 4-component float (-32768.0f..+32767.0f) mapped to short (-32768..+32767)
        eShort4N,       ///< 4-component float (-1.0f..+1.0f) mapped to short (-32768..+32767)
        eUInt10_2N,     ///< 4-component packed, normalized 10-bit XYZ, 2-bit W (0.0 .. 1.0)

        eCount,         ///< number of vertex formats
        eUndefined,     ///< the invalid vertex format value
    };

    /**
     * Defines a single vertex attribute a within a vertex layout.
     */
    class Attribute
    {
    public:

        Semantic semantic = Semantic::ePosition;
        VertexFormat format = VertexFormat::eUndefined;
        uint32_t binding = 0;
        uint32_t location = 0;
        uint32_t offset = 0;

        /**
         * Constructor.
         */
        Attribute();

        /**
         * Constructor.
         *
         * @param semantic The semantic.
         * @param format The format of the attribute
         * @param binding The attribute binding
         * @param location The attibute location
         * @param offset The attribute offset
         */
        Attribute(Semantic semantic,
                  VertexFormat format,
                  uint32_t binding,
                  uint32_t location,
                  uint32_t offset);

        /**
         * Destructor.
         */
        ~Attribute();

        /**
         * Compares two vertex attributes for equality.
         *
         * @param attr The vertex attribute to compare.
         * @return true if this attribute matches the specified one, false otherwise.
         */
        bool operator == (const Attribute& attr) const;

        /**
         * Compares to vertex attributes for inequality.
         *
         * @param attr The vertex attribute to compare.
         * @return true if this attribute does not match the specified one, false otherwise.
         */
        bool operator != (const Attribute& attr) const;
    };

    /**
     * Constructor.
     */
    VertexLayout();

    /**
     * Constructor.
     *
     * The passed in vertex attribute array is copied into the new VertexLayout.
     *
     * @param attributes The array of vertex attributes defining the vertex layout.
     * @param count The number of items in the attributes array.
     */
    VertexLayout(const Attribute* attributes, size_t count);

    /**
     * Destructor.
     */
    ~VertexLayout();

    /**
     * Gets the vertex attribute at the specified index.
     *
     * @param index The index of the attribute to retrieve.
     */
    const Attribute& getAttribute(size_t index) const;

    /**
     * Gets the number of vertex attributes in this VertexLayout.
     *
     * @return The number of attribute in the array.
     */
    size_t getAttributeCount() const;

    /**
     * Gets the stride (size in bytes) of the layout destribing a vertex.
     */
    size_t getStride() const;

    /**
     * Gets the vertex stride for a specified format.
     *
     * @param format The format to get the stride for.
     * @return The vertex stride for a specified format.
     */
    static size_t toStride(VertexFormat format);

    /**
     * Compares two vertex layouts for equality.
     *
     * @param layout The vertex layout to compare.
     * @return true if the attributes in this VertexLayout matches the specified one, false otherwise.
     */
    bool operator == (const VertexLayout& layout) const;

    /**
     * Compares to vertex layouts for inequality.
     *
     * @param layout The vertex layout to compare.
     * @return true if the attributes in this VertexLayout are not equal to the specified one, false otherwise.
     */
    bool operator != (const VertexLayout& layout) const;

private:

    std::vector<Attribute> _attributes;
    size_t _stride;
};























//------------------------------------------------------------------------------
// PixelFormat
//------------------------------------------------------------------------------

enum class PixelFormat
{
    eUndefined,
    eR8Unorm,
    eR16Unorm,
    eR16Float,
    eR32Uint,
    eR32Float,
    eR8G8Unorm,
    eR16G16Unorm,
    eR16G16Float,
    eR32G32Uint,
    eR32G32Float,
    eR32G32B32Uint,
    eR32G32B32Float,
    eB8G8R8A8Unorm,
    eR8G8B8A8Unorm,
    eR16G16B16A16Unorm,
    eR16G16B16A16Float,
    eR32G32B32A32Uint,
    eR32G32B32A32Float,
    eD16Unorm,
    eX8D24UnormPack32,
    eD32Float,
    eS8Uint,
    eD24UnormS8Uint,
    eD32FloatS8Uint
};


//------------------------------------------------------------------------------
// Texture
//------------------------------------------------------------------------------
/**
 * Defines an allocation of formatted image data
 * with a specified texture type and pixel format.
 *
 * Texture objects are used as source textures for
 * vertex, fragment, or compute functions,
 * as well as to store graphics rendering output.
 */
class Texture
{
    friend class Graphics;
    friend class Activator;

public:

    /// Defines the type of texture.
    enum class Type
    {
        e1D = 0,
        e2D,
        e3D
    };

    /// Defines the usage for a texture.
    enum class Usage : uint32_t
    {
        eNone                   = 0x00000000,
        eTransferSrc            = 0x00000001,
        eTransferDst            = 0x00000002,
        eSampledImage           = 0x00000004,
        eStorage                = 0x00000008,
        eColorAttachment        = 0x00000010,
        eDepthStencilAttachment = 0x00000020,
        eResolveSrc             = 0x00000040,
        eResolveDst             = 0x00000080,
        ePresent                = 0x00000100
    };

    /// Defines the supported sample counts for texture and used for storage operations.
    enum class SampleCount : uint32_t
    {
        e1X     = 1,
        e2X     = 2,
        e4X     = 4,
        e8X     = 8,
        e16X    = 16
    };

    /// Constructor.
    Texture();

    /// Destructor.
    ~Texture();

    /**
     * Gets the type of texture.
     *
     * @return The type of texture.
     */
    Type getType() const;

     /**
      * Gets the width of the the texture.
      *
      * @return The width of the the texture.
      */
    size_t getWidth() const;

     /**
      * Gets the height of the texture.
      *
      * @return The height of the texture.
      */
    size_t getHeight() const;

     /**
      * Gets the depth of the texture.
      *
      * @return The depth of the texture.
      */
    size_t getDepth() const;

    /**
     * Gets the number of mip levels.
     *
     * @return The number of mip levels.
     */
    size_t getMipLevels() const;

    /**
     * Gets the pixel format.
     *
     * @return the pixel format.
     */
    PixelFormat getPixelFormat() const;

    /**
     * Gets the usage for the texture.
     *
     * @return The usage for the texture.
     */
    Usage getUsage() const;

    /**
     * Gets the number of samples.
     *
     * @return The number of samples.
     */
    Texture::SampleCount getSampleCount() const;

    /**
     * Deterines if the buffer has visible access to host memory.
     *
     * @return true if the buffer has visible access to host memory, false if static
     */
    bool isHostVisible() const;

    /**
     * Gets the pointer to the mapped host memory.
     *
     * @return The pointer to the mapped host memory.
     */
    void* getHostMemory() const;

    /**
     * Is the texture owned by the host.
     *
     * @return true if host owns the texture and memory, false if swapchain owned.
     */
    bool isHostOwned() const;

private:

    Type _type;
    size_t _width;
    size_t _height;
    size_t _depth;
    size_t _mipLevels;
    PixelFormat _pixelFormat;
    Usage _usage;
    Texture::SampleCount _sampleCount;
    bool _hostVisible;
    void* _hostMemory;
    bool _hostOwned;
};


//------------------------------------------------------------------------------
// Sampler
//------------------------------------------------------------------------------
/**
 * Defines a sampler used to read image data
 * and apply filtering and other transformations
 * for the shader.
 */
class Sampler
{
    friend class Graphics;

public:

    /**
     * Defines a sampling filters for mag, min and mip.
     */
    enum class Filter
    {
        eNearest,
        eLinear
    };

    /**
     * Defines the address mode use for U, V, W.
     */
    enum class AddressMode
    {
        eWrap,
        eMirror,
        eClampEdge,
        eClampBorder,
        eMirrorOnce
    };

    /**
     * Defines a comparison function.
     */
    enum class CompareFunc
    {
        eNever,
        eLess,
        eEqual,
        eLessOrEqual,
        eGreater,
        eNotEqual,
        eGreaterOrEqual,
        eAlways
    };

    /**
     * Defines the border color used for texture lookups.
     */
    enum class BorderColor
    {
        eBlackTransparent,
        eBlackOpaque,
        eWhiteOpaque
    };

    /**
     * Constructor.
     */
    Sampler();

    /**
     * Destructor.
     */
    ~Sampler();

    /**
     * Gets the value specifying the minification filter to apply to lookups.
     *
     * @return The value specifying the minification filter to apply to lookups.
     */
    Filter getFilterMin() const;

    /**
     * Gets the value specifying the magnification filter to apply to lookups.
     *
     * @return The value specifying the magnification filter to apply to lookups.
     */
    Filter getFilterMag() const;

    /**
     * Gets the value specifying the mipmap filter to apply to lookups.
     *
     * @return The value specifying the mipmap filter to apply to lookups.
     */
    Filter getFilterMip() const;

    /**
     * Gets the value specifying the addressing mode for outside [0..1] range for U coordinate.
     *
     * @return The value specifying the addressing mode for outside [0..1] range for U coordinate.
     */
    AddressMode getAddressModeU() const;

    /**
     * Gets the value specifying the addressing mode for outside [0..1] range for V coordinate.
     *
     * @return The value specifying the addressing mode for outside [0..1] range for V coordinate.
     */
    AddressMode getAddressModeV() const;

    /**
     * Gets the value specifying the addressing mode for outside [0..1] range for W coordinate.
     *
     * @return The value specifying the addressing mode for outside [0..1] range for W coordinate.
     */
    AddressMode getAddressModeW() const;

    /**
     * Gets the value specifying the border color used for texture lookup.
     *
     * @return The value specifying the border color used for texture lookup.
     */
    BorderColor getBorderColor() const;

    /**
     * Determines if the comparison function to apply to fetched data before filtering.
     *
     * @return true Determines if the comparison function to apply to fetched data before filtering.
     */
    bool isCompareEnabled() const;

    /**
     * Gets the value specifying the comparison function to apply to fetched data before filtering.
     *
     * @return The value specifying the comparison function to apply to fetched data before filtering.
     */
    CompareFunc getCompareFunc() const;

    /**
     * Determines if anisotropy if applied.
     *
     * @return true if anisotropy if applied, false if disabled.
     */
    bool isAnisotropyEnabled() const;

    /**
     * Gets The maximum anisotropy value to clamp to.
     *
     * @return The maximum anisotropy value to clamp to.
     */
    float getAnisotropyMax() const;

    /**
     * Gets the minimum value used to clamp the computed level-of-detail value.
     *
     * @return The minimum value used to clamp the computed level-of-detail value.
     */
    float getLodMin() const;

    /**
     * Gets the maximum value used to clamp the computed level-of-detail value.
     *
     * @return The maximum value used to clamp the computed level-of-detail value.
     */
    float getLodMax() const;

    /**
     * Gets the bias to be added to mipmap LOD calculation and bias provided by image sampling functions.
     *
     * @return The bias to be added to mipmap LOD calculation and bias provided by image sampling functions.
     */
    float getLodMipBias() const;

protected:

    Filter _filterMag;
    Filter _filterMin;
    Filter _filterMip;
    AddressMode _addressModeU;
    AddressMode _addressModeV;
    AddressMode _addressModeW;
    BorderColor _borderColor;
    bool _compareEnabled;
    CompareFunc _compareFunc;
    bool _anisotropyEnabled;
    float _anisotropyMax;
    float _lodMin;
    float _lodMax;
    float _lodMipBias;
};

//------------------------------------------------------------------------------
// RasterizerState
//------------------------------------------------------------------------------
/**
 * Defines the state which a primitive is rasterized
 * to a two-dimensional image in a render pipeline.
 */
class RasterizerState
{
public:

    /// Defines the mode in which polygons are filled.
    enum class FillMode
    {
        eSolid,
        eWireframe
    };


    /// Defines the triangle culling mode.
    enum class CullMode
    {
        eNone,
        eBack,
        eFront
    };


    /// Define the polygon front-facing orientation.
    enum class FrontFace
    {
        eCCW,
        eCW
    };

    /**
     * The polygon fill mode.
     */
    FillMode fillMode = FillMode::eSolid;

    /**
     * The triangle culling mode.
     */
    CullMode cullMode = CullMode::eNone;

    /**
     * The polygon front-facing orientation.
     */
    FrontFace frontFace = FrontFace::eCCW;

    /**
     * Whether to bias fragment depth values.
     */
    uint32_t depthBias = 0;

    /**
     * The maximum (or minimum) depth bias of a fragment.
     */
    float depthBiasClamp = 0.0f;

    /**
     * Whether to clip the fragment's depth values instead of clipping primitives to the z planes of the frustum.
     */
    bool depthClipEnabled = false;

    /**
     * The width of rasterized line segments.
     */
    float lineWidth = 1.0f;

    /**
     * Whether multi-sampling is enabled.
     */
    bool multiSampleEnabled = false;

    /**
     * The texture multi-sampling count.
     */
    Texture::SampleCount multiSampleCount = Texture::SampleCount::e1X;
};


//------------------------------------------------------------------------------
// ColorBlendState
//------------------------------------------------------------------------------
/**
 * Defines the blending state across color attachments
 * for a render pipeline.
 *
 * @see RenderPipeline
 */
class ColorBlendState
{
public:

    /**
     * Defines the framebuffer blending operations.
     */
    enum class BlendOp
    {
        eAdd,
        eSubtract,
        eReverseSubstact,
        eMin,
        eMax
    };

    /**
     * Defines the framebuffer blending factors.
     */
    enum class BlendFactor
    {
        eZero,
        eOne,
        eSrcColor,
        eOneMinusSrcColor,
        eDstColor,
        eOneMinusDstColor,
        eSrcAlpha,
        eOneMinuseSrcAlpha,
        eDstAlpha,
        eOneMinuseDstAlpha,
        eSrcAlphaSaturate,
        eSrc1Color,
        eOneMinusSrc1Color,
        eSrc1Alpha,
        eOneMinuseSrc1Alpha,
        eBlendFactor,
        eOneMinuseBlendFactor,
    };

    /**
     * Defines whether the final color values R, G, B and A
     * are written to the framebuffer attachment(s).
     */
    enum class WriteMask : uint32_t
    {
        eRed    = 0x00000001,
        eGreen  = 0x00000002,
        eBlue   = 0x00000004,
        eAlpha  = 0x00000008
    };

   /**
    * Whether blending is enabled for the corresponding color attachment.
    * If blending is not enabled, the source fragment's color for that
    * attachment is passed tColorBlendStatehrough unmodified.
    */
    bool blendEnabled = false;

    /**
     * Selects which blend factor is used to determine the source factors (Sr,Sg,Sb).
     */
    BlendFactor colorBlendSrc = BlendFactor::eOne;

    /**
     * Selects which blend factor is used to determine the destination factors (Dr,Dg,Db).
     */
    BlendFactor colorBlendDst = BlendFactor::eZero;

    /**
     * Selects which blend operation is used to calculate the RGB values to write to the color attachment.
     */
    BlendOp colorBlendOp = BlendOp::eAdd;

    /**
     * The WriteMask specifying which of the R, G, B, and/or A components are enabled for writing.
     */
    uint32_t colorWriteMask = 0xf;

    /**
     * Selects which blend factor is used to determine the source factor Sa.
     */
    BlendFactor alphaBlendSrc = BlendFactor::eOne;

    /**
     * Selects which blend factor is used to determine the destination factor Da.
     */
    BlendFactor alphaBlendDst = BlendFactor::eZero;

    /**
     * Selects which blend operation is use to calculate the alpha values to write to the color attachment.
     */
    BlendOp alphaBlendOp = BlendOp::eAdd;
};


//------------------------------------------------------------------------------
// DepthStencilState
//------------------------------------------------------------------------------
/**
 * Defines the depth stencil state for the render pipeline.
 *
 * @see RenderPipeline
 */
class DepthStencilState
{
public:

    /**
     * Defines the operation to the stored stencil value
     * if this or certain subsequent tests fail or pass,
     */
    enum class StencilOp
    {
        eKeep,
        eZero,
        eReplace,
        eIncrementAndClamp,
        eDecrementAndClamp,
        eInvert,
        eIncrementAndWrap,
        eDecrementAndWrap
    };

    /**
     * Defines the stencil tests.
     */
    class StencilOpState
    {
    public:

        /**
         * The action performed on samples that fail the stencil test.
         */
        StencilOp failOp = StencilOp::eKeep;

        /**
         * The action performed on samples that pass the stencil test and fail the depth test.
         */
        StencilOp passOp = StencilOp::eKeep;

        /**
         * The action performed on samples that fail both the depth and stencil tests.
         */
        StencilOp depthFailOp = StencilOp::eKeep;

        /**
         * The depth compare function.
         */
        Sampler::CompareFunc compareFunc = Sampler::CompareFunc::eNever;

         /**
          * Selects the bits of the stencil values participating
          * in the stencil test.
          */
        unsigned char compareMask = 0xff;

        /**
         * Selects the bits of the uint32_t stencil values updated
         * by the stencil test in the stencil framebuffer attachment
         */
        unsigned char writeMask = 0xff;
    };

    /**
     * Determines if depth testing is enabled.
     */
    bool depthEnabled = true;

    /**
     * The depth write mask.
     */
    bool  depthWrite = true;

    /**
     * The comparison function used in the depth test sampling.
     */
    Sampler::CompareFunc depthFunc = Sampler::CompareFunc::eLessOrEqual;

    /**
     * Determines if stencil testing is enabled.
     */
    bool stencilEnabled = false;

    /**
     * Stencil state of the front stencil test.
     */
    StencilOpState stencilOpStateFront;

    /**
     * Stencil state of the back stencil test.
     */
    StencilOpState stencilOpStateBack;
};





//------------------------------------------------------------------------------
// RenderPass
//------------------------------------------------------------------------------
/**
 * Defines a collection of attachments used for
 * the output of render pass operations in a
 * render pipeline.
 *
 * @see RenderPipeline
 */
class RenderPass
{
    friend class Graphics;

public:

    /**
     * Constructor.
     */
    RenderPass();

    /**
     * Constructor.
     */
    RenderPass(size_t width, size_t height,
               size_t colorAttachmentCount,
               PixelFormat colorFormat,
               PixelFormat depthStancilFormat,
               Texture::SampleCount sampleCount,
               std::vector<std::shared_ptr<Texture>> colorAttachments,
               std::vector<std::shared_ptr<Texture>> colorMultisampleAttachments,
               std::shared_ptr<Texture> depthStencilAttachment);

    /**
     * Destructor.
     */
    ~RenderPass();

    /**
     * Gets the width of the render pass and all attachments.
     *
     * @return The width of the render pass and all attachments.
     */
    size_t getWidth() const;

    /**
     * Gets the height of the render pass and all attachments.
     *
     * @return The height of the render pass and all attachments.
     */
    size_t getHeight() const;

    /**
     * Gets the number of color attachments.
     *
     * @return The number of color attachments.
     */
    size_t getColorAttachmentCount() const;

    /**
     * Gets the formats of the color attachments.
     *
     * @return The formats of the color attachments.
     */
    PixelFormat getColorFormat(size_t index) const;

    /**
     * Gets the format for the depth stencil attachment.
     *
     * @return The format for the depth stencil attachment.
     */
    PixelFormat getDepthStencilFormat() const;

    /**
     * Gets the sampling count for the render pass.
     *
     * @return The sampling count for the render pass.
     */
    Texture::SampleCount getSampleCount() const;

    /**
     * Gets the color attachment as a texture as the specifed index.
     *
     * @return The color attachment as a texture as the specifed index.
     */
    std::shared_ptr<Texture> getColorAttachment(size_t index);

    /**
     * Gets the color multi-sample attachment as a texture as the specifed index.
     *
     * @return The color multi-sample attachment as a texture as the specifed index.
     */
    std::shared_ptr<Texture> getColorMultisampleAttachment(size_t index);

    /**
     * Gets the depth stencil attachment as a texture.
     *
     * @return The depth stencil attachment as a texture.
     */
    std::shared_ptr<Texture> getDepthStencilAttachment();

private:

    size_t _width;
    size_t _height;
    size_t _colorAttachmentCount;
    std::vector<PixelFormat> _colorFormats;
    PixelFormat _depthStencilFormat;
    Texture::SampleCount _sampleCount;
    std::vector<std::shared_ptr<Texture>> _colorAttachments;
    std::vector<std::shared_ptr<Texture>> _colorMultisampleAttachments;
    std::shared_ptr<Texture> _depthStencilAttachment;
};



//------------------------------------------------------------------------------
// Buffer
//------------------------------------------------------------------------------
/**
 * Defines an allocation of unformatted memory
 * that can contain any type of data.
 *
 * Buffers are often used for vertex, shader
 * and compute state data.
 */
class Buffer
{
    friend class GraphicsBGFX;

public:

    /**
     * Defines the usage of Buffer.
     */
    enum class Usage : uint32_t
    {
        eVertex  =  0x00000000,
        eIndex   =  0x00000001,
        eUniform =  0x00000002
    };

    /**
     * Constructor.
     */
    Buffer();

    /**
     * Destructor.
     */
    ~Buffer();

    /**
     * Gets the usage of buffer.
     *
     * @return The usage of buffer.
     */
    Usage getUsage() const;

    /**
     * Gets the buffer size (in bytes).
     *
     * @return the buffer size (in bytes).
     */
    size_t getSize() const;

    /**
     * Gets the stride of one element in the buffer.
     */
    size_t getStride() const;

    /**
     * Deterines if the buffer has visible access to host memory.
     *
     * @return true if the buffer has visible access to host memory, false if static
     */
    bool isHostVisible() const;

    /**
     * Gets the pointer to the mapped host memory.
     *
     * @return The pointer to the mapped host memory.
     */
    virtual void* getHostMemory() const = 0;

protected:

    Usage _usage;
    size_t _size;
    size_t _stride;
    bool _hostVisible;
};



//------------------------------------------------------------------------------
// DescriptorSet
//-----------------------------------------------------------------------------
/**
 * Defines a set of resource descriptors to be bound to a RenderPipeline
 */
class DescriptorSet
{
    friend class Graphics;

public:

    /**
     * Defines an opaque resource descriptor.
     */
    class Descriptor
    {
    public:

        /**
         * Constructor.
         */
        Descriptor();

        /**
         * Destructor.
         */
        ~Descriptor();

        /**
         * Defines the type of resource.
         */
        enum class Type
        {
            eUndefined,
            eUniform,
            eTexture,
            eSampler
        };

        /**
         * Defines the shader stages.
         */
        enum class ShaderStages : uint32_t
        {
            eNone = 0x00000000,
            eVert = 0x00000001,
            eTesc = 0x00000002,
            eTese = 0x00000004,
            eGeom = 0x00000008,
            eFrag = 0x00000010
        };

        /**
         * The type of descriptor.
         */
        Type type = Type::eUndefined;

        /**
         * The descriptor binding index.
         */
        uint32_t binding = 0;

        /**
         * The number of resources.
         */
        size_t count = 0;

        /**
         * The stage in the shader that the resource is accessible from.
         */
        ShaderStages shaderStages = ShaderStages::eNone;

        std::shared_ptr<Buffer> uniforms[GP_GRAPHICS_DESCRIPTOR_ENTRIES_MAX];
        std::shared_ptr<Texture> textures[GP_GRAPHICS_DESCRIPTOR_ENTRIES_MAX];
        std::shared_ptr<Sampler>  samplers[GP_GRAPHICS_DESCRIPTOR_ENTRIES_MAX];

    };

    /**
     * Constructor.
     */
    DescriptorSet();

    /**
     * Constructor.
     */
    DescriptorSet(const Descriptor* descriptors, size_t descriptorCount);

    /**
     * Destructor.
     */
    ~DescriptorSet();

    /**
     * Gets the number of descriptors in the set.
     *
     * @return The number of descriptors in the set.
     */
    size_t getDescriptorCount() const;

    /**
     * Gets the descriptor at the specified index.
     *
     * @param index The index to access.
     */
    DescriptorSet::Descriptor getDescriptor(size_t index) const;

private:

    std::vector<Descriptor> _descriptors;
};



//------------------------------------------------------------------------------
// Shader
//------------------------------------------------------------------------------

/**
 * Defines a shader (module/function) for use in
 * a shader stage in a Pipeline.
 */
class Shader
{
    friend class Graphics;

public:

    /**
     * Constructor.
     */
    Shader();

    /**
     * Destructor.
     */
    ~Shader();
};


//------------------------------------------------------------------------------
// RenderPipeline
//------------------------------------------------------------------------------
/**
 * Defines a pipeline for processing for rendering programs
 */
class RenderPipeline
{
    friend class Graphics;

public:

    /**
     * Defines the primitive topology.
     */
    enum class PrimitiveTopology
    {
        ePointList,
        eLineList,
        eLineStrip,
        eTriangleList,
        eTriangleStrip,
    };

    /**
     * Constructor.
     */
    RenderPipeline();

    /**
     * Constructor.
     */
    RenderPipeline(PrimitiveTopology primitiveTopology,
                   VertexLayout vertexLayout,
                   RasterizerState rasterizerState,
                   ColorBlendState colorBlendState,
                   DepthStencilState depthStencilState,
                   std::shared_ptr<RenderPass> renderPass,
                   std::shared_ptr<DescriptorSet> descriptorSet,
                   std::shared_ptr<Shader> vertShader,
                   std::shared_ptr<Shader> tescShader,
                   std::shared_ptr<Shader> teseShader,
                   std::shared_ptr<Shader> geomShader,
                   std::shared_ptr<Shader> fragShader);

    /**
     * Destructor.
     */
    ~RenderPipeline();

    /**
     * Gets the primitive topology.
     *
     * @return The primitive topology.
     */
    PrimitiveTopology getPrimitiveTopology() const;

    /**
     * Gets the vertex layout.
     *
     * @return The vertex layout.
     */
    VertexLayout getVertexLayout() const;

    /**
     * Gets the rasterizer state.
     *
     * @return The rasterizer state.
     */
    RasterizerState getRasterizerState() const;

    /**
     * Gets the color blend state.
     *
     * @return The color blend state.
     */
    ColorBlendState getColorBlendState() const;

    /**
     * Gets the depth stencil state.
     *
     * @return The depth stencil state.
     */
    DepthStencilState getDepthStencilState() const;

    /**
     * Gets the render pass.
     *
     * @return The render pass.
     */
    std::shared_ptr<RenderPass> getRenderPass() const;

    /**
     * Gets the descriptor set.
     *
     * @return The  descriptor set.
     */
    std::shared_ptr<DescriptorSet> getDescriptorSet() const;

    /**
     * Gets the vertex shader.
     *
     * @return The vertex shader.
     */
    std::shared_ptr<Shader> getVertexShader() const;

    /**
     * Gets the tessellation control shader.
     *
     * @return The tessellation control shader.
     */
    std::shared_ptr<Shader> getTessellationControlShader() const;

    /**
     * Gets the tessellation evaluation shader.
     *
     * @return The tessellation evaluation shader.
     */
    std::shared_ptr<Shader> getTessellationEvaluationShader() const;

    /**
     * Gets the geometry shader.
     *
     * @return The geometry shader.
     */
    std::shared_ptr<Shader> getGeometryShader() const;

    /**
     * Gets the fragment shader.
     *
     * @return The fragment shader.
     */
    std::shared_ptr<Shader> getFragmentShader() const;

private:

    PrimitiveTopology _primitiveTopology;
    VertexLayout _vertexLayout;
    RasterizerState _rasterizerState;
    ColorBlendState _colorBlendState;
    DepthStencilState _depthStencilState;
    std::shared_ptr<RenderPass> _renderPass;
    std::shared_ptr<DescriptorSet> _descriptorSet;
    std::shared_ptr<Shader> _vertShader;
    std::shared_ptr<Shader> _tescShader;
    std::shared_ptr<Shader> _teseShader;
    std::shared_ptr<Shader> _geomShader;
    std::shared_ptr<Shader> _fragShader;
};

//------------------------------------------------------------------------------
// Semaphore
//------------------------------------------------------------------------------
/**
 * Defines a synchronization primitive that can be used
 * to insert a dependency between batches submitted to queues.
 */
class Semaphore
{
    friend class Graphics;

public:

    /**
     * Constructor.
     */
    Semaphore() {}

    /**
     * Destructor.
     */
    ~Semaphore() {}
};


//------------------------------------------------------------------------------
// CommandBuffer
//------------------------------------------------------------------------------
/**
 * Defines a command buffer for storing
 * recorded gpu commands.
 */
class CommandBuffer
{
    friend class Graphics;

public:

    /**
     * Constructor.
     */
    CommandBuffer() {}

    /**
     * Destructor.
     */
    ~CommandBuffer() {}
};

} // end namespace rainbow

GP_ENABLE_BITWISE_OPERATORS(rainbow::ColorBlendState::WriteMask)
