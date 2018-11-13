#include "Base.h"
#include "CommandBuffer.h"

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