#include "../Core/Base.h"
#include "../Graphics/Semaphore.h"

namespace rainbow
{

Semaphore::Semaphore() :
    _semaphore(VK_NULL_HANDLE)
{
}

Semaphore::~Semaphore()
{
}

}
