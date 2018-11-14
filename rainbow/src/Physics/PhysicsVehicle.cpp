#include "../Core/Base.h"
#include "../Physics/PhysicsVehicle.h"

namespace rainbow
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
    return "rainbow::PhysicsVehicle";
}

void PhysicsVehicle::onSerialize(Serializer * serializer)
{
}

void PhysicsVehicle::onDeserialize(Serializer * serializer)
{
}

}
