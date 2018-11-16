#include "../../../Core/Base.h"
#include "BufferBGFX.h"

namespace rainbow
{

BufferBGFX::BufferBGFX() :
    Buffer(),
    _hostMemory(nullptr)
{
}

BufferBGFX::~BufferBGFX()
{
}

void* BufferBGFX::getHostMemory() const
{
    return _hostMemory;
}

}
