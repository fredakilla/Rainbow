#include "../Core/Base.h"
#include "../Core/Activator.h"
#include "../Core/Game.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneObject.h"
#include "../Scene/Camera.h"
#include "../Scene/Light.h"

namespace rainbow
{

Activator::Activator()
{
}

Activator::~Activator()
{
}

Activator* Activator::getActivator()
{
    static Activator activator;
    return &activator;
}
       
std::shared_ptr<Serializable> Activator::createObject(const std::string& className)
{
    std::shared_ptr<Serializable> object = nullptr;
    std::map<std::string, CreateObjectCallback>::const_iterator itr = _classes.find(className);
    if (itr == _classes.end())
    {
        return nullptr;
    }
    
    CreateObjectCallback createObject = itr->second;
    if (createObject)
    {
        object = createObject();
    }
    return object;
}

std::string Activator::enumToString(const std::string& enumName, int value)
{
    std::map<std::string,std::pair<EnumToStringCallback, EnumParseCallback>>::const_iterator itr = _enums.find(enumName);
    if (itr != _enums.end())
    {
        EnumToStringCallback enumToString = itr->second.first;
        if (enumToString)
        {
            return enumToString(enumName, value);
        }
    }
    return nullptr;
}

int Activator::enumParse(const std::string& enumName, const std::string& str)
{
    std::map<std::string,std::pair<EnumToStringCallback, EnumParseCallback>>::const_iterator itr = _enums.find(enumName);
    if (itr != _enums.end())
    {
        EnumParseCallback enumParse = itr->second.second;
        if (enumParse)
        {
            return enumParse(enumName, str);
        }
    }
    return 0;
}

void Activator::registerType(const std::string&  className, CreateObjectCallback createObject)
{
    std::map<std::string,CreateObjectCallback>::const_iterator itr = _classes.find(className);
    if ( itr == _classes.end() )
    {
        _classes[className] = createObject;
    }
    else
    {
        GP_ERROR("className already registered:%s", className.c_str());
    }
}
    
void Activator::registerEnum(const std::string& enumName, EnumToStringCallback toString, EnumParseCallback parse)
{
    std::map<std::string, std::pair<EnumToStringCallback, EnumParseCallback>>::const_iterator itr = _enums.find(enumName);
    if (itr == _enums.end())
    {
        _enums[enumName] = std::make_pair(toString, parse);
    }
    else
    {
        GP_ERROR("enumName already registered:%s", enumName.c_str());
    }
}

void Activator::registerSystemTypes()
{
    if (!_enums.empty())
    {
        return;
    }
        // Register engine types with
    Activator::getActivator()->registerType("rainbow::Game::Config", Game::Config::createObject);
    Activator::getActivator()->registerType("rainbow::Scene", Scene::createObject);
    Activator::getActivator()->registerType("rainbow::SceneObject", SceneObject::createObject);
    Activator::getActivator()->registerType("rainbow::Camera", Camera::createObject);
    Activator::getActivator()->registerType("rainbow::Input::Mapping::KeyActionData", Input::Mapping::KeyActionData::createObject);
    Activator::getActivator()->registerType("rainbow::Input::Mapping::MouseButtonActionData", Input::Mapping::MouseButtonActionData::createObject);
    Activator::getActivator()->registerType("rainbow::Input::Mapping", Input::Mapping::createObject);
    Activator::getActivator()->registerType("rainbow::Input::Mappings", Input::Mappings::createObject);
    Activator::getActivator()->registerType("rainbow::Light", Light::createObject);

    // Register engine enums
    Activator::getActivator()->registerEnum("rainbow::Camera::Mode", Camera::enumToString, Camera::enumParse);
    Activator::getActivator()->registerEnum("rainbow::Input::GamepadAxis", Input::enumToString, Input::enumParse);
    Activator::getActivator()->registerEnum("rainbow::Input::Key", Input::enumToString, Input::enumParse);
    Activator::getActivator()->registerEnum("rainbow::Input::KeyModifiers", Input::enumToString, Input::enumParse);
    Activator::getActivator()->registerEnum("rainbow::Input::MouseAxis", Input::enumToString, Input::enumParse);
    Activator::getActivator()->registerEnum("rainbow::Input::MouseButton", Input::enumToString, Input::enumParse);
    Activator::getActivator()->registerEnum("rainbow::Input::Mapping::Action", Input::enumToString, Input::enumParse);
    Activator::getActivator()->registerEnum("rainbow::Input::Mapping::ActionProfile", Input::enumToString, Input::enumParse);
    Activator::getActivator()->registerEnum("rainbow::Input::Mapping::Type", Input::enumToString, Input::enumParse);
    Activator::getActivator()->registerEnum("rainbow::Light::Type", Light::enumToString, Light::enumParse);
    Activator::getActivator()->registerEnum("rainbow::Light::Mode", Light::enumToString, Light::enumParse);
    Activator::getActivator()->registerEnum("rainbow::Light::Shadows", Light::enumToString, Light::enumParse);
}

}
