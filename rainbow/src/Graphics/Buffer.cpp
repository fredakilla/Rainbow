#include "../Core/Base.h"
#include "../Graphics/Buffer.h"

namespace rainbow
{

Buffer::Buffer() :
    _usage(Usage::eVertex),
    _size(0), 
    _stride(0), 
    _hostVisible(true)
{
}

Buffer::~Buffer()
{
}

Buffer::Usage Buffer::getUsage() const
{
    return _usage;
}

size_t Buffer::getSize() const
{
    return _size;
}

size_t Buffer::getStride() const
{
    return _stride;
}

bool Buffer::isHostVisible() const
{
    return _hostVisible;
}

}
