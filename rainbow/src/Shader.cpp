#include "Base.h"
#include "Shader.h"

namespace rainbow
{

Shader::Shader() :
    _shaderModule(VK_NULL_HANDLE)
{
}

Shader::~Shader()
{
}

}