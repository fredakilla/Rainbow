#include "../Core/Base.h"
#include "../Graphics/Shader.h"

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
