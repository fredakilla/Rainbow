#include "Base.h"
#include "Sprite.h"

namespace rainbow
{

Sprite::Sprite()
{
}

Sprite::~Sprite()
{
}

std::shared_ptr<Serializable> Sprite::createObject()
{
    return std::shared_ptr<Sprite>();
}

std::string Sprite::getClassName()
{
    return "rainbow::Sprite";
}

void Sprite::onSerialize(Serializer* serializer)
{
    Renderer::onSerialize(serializer);
}

void Sprite::onDeserialize(Serializer* serializer)
{
    Renderer::onDeserialize(serializer);
}

}
