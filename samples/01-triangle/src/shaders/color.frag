#version 150
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout (binding = 1) uniform UBO 
{
    vec4 color;
} ubo;


layout (location = 0) in vec3 inColor;

layout (location = 0) out vec4 outFragColor;

void main( void )
{
    //outFragColor = vec4(inColor, 1.0);
    outFragColor = ubo.color;
}
