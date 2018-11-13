#include "Base.h"
#include "CommandBuffer.h"

namespace prana
{

CommandBuffer::CommandBuffer() : 
    _commandBuffer(VK_NULL_HANDLE)
{
}
CommandBuffer::~CommandBuffer()
{
}

}