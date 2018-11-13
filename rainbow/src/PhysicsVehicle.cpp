#include "Base.h"
#include "PhysicsVehicle.h"

namespace prana
{

PhysicsVehicle::PhysicsVehicle()
{
}

PhysicsVehicle::~PhysicsVehicle()
{
}

std::shared_ptr<Serializable> PhysicsVehicle::createObject()
{
    return std::shared_ptr<PhysicsVehicle>();
}

std::string PhysicsVehicle::getClassName()
{
    return "prana::PhysicsVehicle";
}

void PhysicsVehicle::onSerialize(Serializer * serializer)
{
}

void PhysicsVehicle::onDeserialize(Serializer * serializer)
{
}

}
