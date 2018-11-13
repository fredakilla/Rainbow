#include "Base.h"
#include "Semaphore.h"

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