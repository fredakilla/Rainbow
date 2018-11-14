#include "../Core/Base.h"
#include "../Core/Serializer.h"
#include "../Core/SerializerBinary.h"
#include "../Core/SerializerJson.h"
#include "../Core/FileSystem.h"
#include "../Core/Game.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneObject.h"
#include "../Scene/Camera.h"
#include "../Scene/Light.h"

namespace rainbow
{

Serializer::Serializer(Type type, const std::string& path, Stream* stream, uint32_t versionMajor, uint32_t versionMinor) : 
    _type(type),
    _path(path),
    _stream(stream)
{
    _version[0] = versionMajor;
    _version[1] = versionMinor;
}

Serializer::~Serializer()
{
    _stream->close();
    GP_SAFE_DELETE(_stream);
}

Serializer* Serializer::createReader(const std::string& path)
{
    Stream* stream = FileSystem::open(path);
    if (!stream)
        return nullptr;

    Serializer* serializer = SerializerBinary::create(path, stream);
    if (!serializer)
    {
        stream->rewind();
        serializer = SerializerJson::create(path, stream);
    }
    return serializer;
}

std::string Serializer::getPath() const
{
    return _path;
}
   
uint32_t Serializer::getVersionMajor() const
{
    return (uint32_t)_version[0];
}

uint32_t Serializer::getVersionMinor() const
{
    return (uint32_t)_version[1];
}

}
