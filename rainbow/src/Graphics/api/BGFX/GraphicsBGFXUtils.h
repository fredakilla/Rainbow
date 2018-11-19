#pragma once

#include "GraphicsTypesBGFX.h"
#include <bgfx/bgfx.h>

namespace rainbow
{

/**
 * Maps rainbow::VertexLayout::Semantic to bgfx::Attrib::Enum
 */
bgfx::Attrib::Enum LOOKUP_BGFX_VERTEX_ATTRIB[] =
{
    bgfx::Attrib::Enum::Position,   //  ePosition,
    bgfx::Attrib::Enum::Normal,     //  eNormal,
    bgfx::Attrib::Enum::Color0,     //  eColor,
    bgfx::Attrib::Enum::Color0,     //  eColor0,
    bgfx::Attrib::Enum::Color1,     //  eColor1,
    bgfx::Attrib::Enum::Color2,     //  eColor2,
    bgfx::Attrib::Enum::Color3,     //  eColor3,
    bgfx::Attrib::Enum::Tangent,    //  eTangent,
    bgfx::Attrib::Enum::Bitangent,  //  eBitangent,
    bgfx::Attrib::Enum::TexCoord0,  //  eTexCoord,
    bgfx::Attrib::Enum::TexCoord0,  //  eTexCoord0,
    bgfx::Attrib::Enum::TexCoord1,  //  eTexCoord1,
    bgfx::Attrib::Enum::TexCoord2,  //  eTexCoord2,
    bgfx::Attrib::Enum::TexCoord3,  //  eTexCoord3,
    bgfx::Attrib::Enum::TexCoord4,  //  eTexCoord4,
    bgfx::Attrib::Enum::TexCoord5,  //  eTexCoord5,
    bgfx::Attrib::Enum::TexCoord6,  //  eTexCoord6,
    bgfx::Attrib::Enum::TexCoord7,  //  eTexCoord7
};

/**
 * Struct to store bgfx vertex format declaration
 */
struct BgfxAttributeFormat
{
    bgfx::AttribType::Enum type;
    bool normalized;
    uint8_t num;
    bool asInt;
};

/**
 * map rainbow::VertexLayout::VertexFormat <=> BgfxAttributeFormat
 */
static BgfxAttributeFormat LOOKUP_BGFX_ATTRIB_TYPE[] =
{
    //        AttribType        normalized  num  asInt

    { bgfx::AttribType::Float,      false,   1,  false  },  // eFloat,
    { bgfx::AttribType::Float,      false,   2,  false  },  // eFloat2,
    { bgfx::AttribType::Float,      false,   3,  false  },  // eFloat3,
    { bgfx::AttribType::Float,      false,   4,  false  },  // eFloat4,
    { bgfx::AttribType::Uint8,      false,   4,  true   },  // eByte4,
    { bgfx::AttribType::Uint8,      true ,   4,  true   },  // eByte4N,
    { bgfx::AttribType::Uint8,      false,   4,  true   },  // eUByte4,
    { bgfx::AttribType::Uint8,      true ,   4,  true   },  // eUByte4N,
    { bgfx::AttribType::Int16,      false,   2,  true   },  // eShort2,
    { bgfx::AttribType::Int16,      true ,   2,  true   },  // eShort2N,
    { bgfx::AttribType::Int16,      false,   4,  true   },  // eShort4,
    { bgfx::AttribType::Int16,      true ,   4,  true   },  // eShort4N,
    { bgfx::AttribType::Count,      false ,  0,  false  },  // eUndefined,
};

/**
 * ColorBlendState::BlendFactor to BGFX_STATE_BLEND_*
 */
static const uint64_t LOOKUP_BGFX_BLEND_FACTOR[] =
{
    BGFX_STATE_BLEND_ZERO,              // eZero,
    BGFX_STATE_BLEND_ONE,               // eOne,
    BGFX_STATE_BLEND_SRC_COLOR,         // eSrcColor,
    BGFX_STATE_BLEND_INV_SRC_COLOR,     // eOneMinusSrcColor,
    BGFX_STATE_BLEND_DST_COLOR,         // eDstColor,
    BGFX_STATE_BLEND_INV_DST_COLOR,     // eOneMinusDstColor,
    BGFX_STATE_BLEND_SRC_ALPHA,         // eSrcAlpha,
    BGFX_STATE_BLEND_INV_SRC_ALPHA,     // eOneMinuseSrcAlpha,
    BGFX_STATE_BLEND_DST_ALPHA,         // eDstAlpha,
    BGFX_STATE_BLEND_INV_DST_ALPHA,     // eOneMinuseDstAlpha,
    BGFX_STATE_BLEND_SRC_ALPHA_SAT,     // eSrcAlphaSaturate,
    BGFX_STATE_BLEND_ZERO,              // eSrc1Color,              // !! not supported in bgfx
    BGFX_STATE_BLEND_ZERO,              // eOneMinusSrc1Color,      // !! not supported in bgfx
    BGFX_STATE_BLEND_ZERO,              // eSrc1Alpha,              // !! not supported in bgfx
    BGFX_STATE_BLEND_ZERO,              // eOneMinuseSrc1Alpha      // !! not supported in bgfx
    BGFX_STATE_BLEND_FACTOR,            // eBlendFactor,
    BGFX_STATE_BLEND_INV_FACTOR,        // eOneMinuseBlendFactor
};

/**
 * maps ColorBlendState::BlendOp to BGFX_STATE_BLEND_EQUATION_*
 */
static const uint64_t LOOKUP_BGFX_BLEND_OP[] =
{
    BGFX_STATE_BLEND_EQUATION_ADD,      // eAdd,
    BGFX_STATE_BLEND_EQUATION_SUB,      // eSubtract,
    BGFX_STATE_BLEND_EQUATION_REVSUB,   // eReverseSubstact,
    BGFX_STATE_BLEND_EQUATION_MIN,      // eMin,
    BGFX_STATE_BLEND_EQUATION_MAX,      // eMax
};

/**
 * maps Sampler::CompareFunc to BGFX_STATE_DEPTH_TEST_*
 */
static const uint64_t LOOKUP_BGFX_DEPTH_TEST[] =
{
    BGFX_STATE_DEPTH_TEST_NEVER,        // eNever,
    BGFX_STATE_DEPTH_TEST_LESS,         // eLess,
    BGFX_STATE_DEPTH_TEST_EQUAL,        // eEqual,
    BGFX_STATE_DEPTH_TEST_LEQUAL,       // eLessOrEqual,
    BGFX_STATE_DEPTH_TEST_GREATER,      // eGreater,
    BGFX_STATE_DEPTH_TEST_NOTEQUAL,     // eNotEqual,
    BGFX_STATE_DEPTH_TEST_GEQUAL,       // eGreaterOrEqual,
    BGFX_STATE_DEPTH_TEST_ALWAYS        // eAlways
};

}
