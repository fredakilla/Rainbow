#include "Base.h"
#include "PhysicsCharacter.h"

namespace rainbow
{

PhysicsCharacter::PhysicsCharacter()
{
}

PhysicsCharacter::~PhysicsCharacter()
{
}

std::shared_ptr<Serializable> PhysicsCharacter::createObject()
{
    return std::shared_ptr<PhysicsCharacter>();
}

std::string PhysicsCharacter::getClassName()
{
    return "rainbow::PhysicsCharacter";
}

void PhysicsCharacter::onSerialize(Serializer * serializer)
{
}

void PhysicsCharacter::onDeserialize(Serializer * serializer)
{
}

}
