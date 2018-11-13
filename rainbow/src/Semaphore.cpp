#include "Base.h"
#include "Semaphore.h"

namespace prana
{

Semaphore::Semaphore() :
    _semaphore(VK_NULL_HANDLE)
{
}

Semaphore::~Semaphore()
{
}

}