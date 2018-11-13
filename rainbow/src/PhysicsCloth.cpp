#include "Base.h"
#include "PhysicsCloth.h"

namespace prana
{

PhysicsCloth::PhysicsCloth()
{
}

PhysicsCloth::~PhysicsCloth()
{
}

std::shared_ptr<Serializable> PhysicsCloth::createObject()
{
    return std::shared_ptr<PhysicsCloth>();
}

std::string PhysicsCloth::getClassName()
{
    return "prana::PhysicsCloth";
}

void PhysicsCloth::onSerialize(Serializer * serializer)
{
}

void PhysicsCloth::onDeserialize(Serializer * serializer)
{
}

}
