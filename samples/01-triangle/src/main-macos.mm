#include "rainbow.h"

using namespace rainbow;

int main(int argc, char** argv)
{
    Platform* platform = Platform::getPlatform();
    return platform->run();
}
