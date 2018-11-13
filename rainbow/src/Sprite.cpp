#include "Base.h"
#include "Sprite.h"

namespace prana
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
    return "prana::Sprite";
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
