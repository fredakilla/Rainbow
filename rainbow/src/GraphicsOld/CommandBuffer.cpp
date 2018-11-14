#include "../Core/Base.h"
#include "../Graphics/CommandBuffer.h"

namespace rainbow
{

CommandBuffer::CommandBuffer() : 
    _commandBuffer(VK_NULL_HANDLE)
{
}
CommandBuffer::~CommandBuffer()
{
}

}
