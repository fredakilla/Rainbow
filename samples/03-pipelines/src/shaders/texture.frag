#version 450

layout (binding = 1) uniform UBO 
{
    vec4 color;
} ubo;

layout (binding = 2) uniform sampler2D samplerColorMap;


layout (location = 0) in vec3 inColor;
layout (location = 1) in vec2 inUV;

layout (location = 0) out vec4 outFragColor;

void main( void )
{
    //outFragColor = vec4(inColor, 1.0);
    outFragColor = texture(samplerColorMap, inUV);
}
