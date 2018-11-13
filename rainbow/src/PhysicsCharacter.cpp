#include "Base.h"
#include "PhysicsCharacter.h"

namespace prana
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
    return "prana::PhysicsCharacter";
}

void PhysicsCharacter::onSerialize(Serializer * serializer)
{
}

void PhysicsCharacter::onDeserialize(Serializer * serializer)
{
}

}
