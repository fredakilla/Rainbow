#include "rainbow.h"

using namespace rainbow;


int main(int argc, const char** argv)
{
    Platform* platform = Platform::getPlatform();
    return platform->run();
}
