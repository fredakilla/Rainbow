#include "Base.h"
#include "Activator.h"
#include "Game.h"
#include "Scene.h"
#include "SceneObject.h"
#include "Camera.h"
#include "Light.h"

namespace prana
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
    Activator::getActivator()->registerType("prana::Game::Config", Game::Config::createObject);
    Activator::getActivator()->registerType("prana::Scene", Scene::createObject);
    Activator::getActivator()->registerType("prana::SceneObject", SceneObject::createObject);
    Activator::getActivator()->registerType("prana::Camera", Camera::createObject);
    Activator::getActivator()->registerType("prana::Input::Mapping::KeyActionData", Input::Mapping::KeyActionData::createObject);
    Activator::getActivator()->registerType("prana::Input::Mapping::MouseButtonActionData", Input::Mapping::MouseButtonActionData::createObject);
    Activator::getActivator()->registerType("prana::Input::Mapping", Input::Mapping::createObject);
    Activator::getActivator()->registerType("prana::Input::Mappings", Input::Mappings::createObject);
    Activator::getActivator()->registerType("prana::Light", Light::createObject);

    // Register engine enums
    Activator::getActivator()->registerEnum("prana::Camera::Mode", Camera::enumToString, Camera::enumParse);
    Activator::getActivator()->registerEnum("prana::Input::GamepadAxis", Input::enumToString, Input::enumParse);
    Activator::getActivator()->registerEnum("prana::Input::Key", Input::enumToString, Input::enumParse);
    Activator::getActivator()->registerEnum("prana::Input::KeyModifiers", Input::enumToString, Input::enumParse);
    Activator::getActivator()->registerEnum("prana::Input::MouseAxis", Input::enumToString, Input::enumParse);
    Activator::getActivator()->registerEnum("prana::Input::MouseButton", Input::enumToString, Input::enumParse);
    Activator::getActivator()->registerEnum("prana::Input::Mapping::Action", Input::enumToString, Input::enumParse);
    Activator::getActivator()->registerEnum("prana::Input::Mapping::ActionProfile", Input::enumToString, Input::enumParse);
    Activator::getActivator()->registerEnum("prana::Input::Mapping::Type", Input::enumToString, Input::enumParse);
    Activator::getActivator()->registerEnum("prana::Light::Type", Light::enumToString, Light::enumParse);
    Activator::getActivator()->registerEnum("prana::Light::Mode", Light::enumToString, Light::enumParse);
    Activator::getActivator()->registerEnum("prana::Light::Shadows", Light::enumToString, Light::enumParse);
}

}
