#include "../Core/Base.h"
#include "../Physics/PhysicsJointFixed.h"

namespace rainbow
{

PhysicsJointFixed::PhysicsJointFixed() 
{
}

PhysicsJointFixed::~PhysicsJointFixed()
{
}

PhysicsJoint::Type PhysicsJointFixed::getType() const
{
    return Type::eFixed;
}

std::shared_ptr<Serializable> PhysicsJointFixed::createObject()
{
    return std::shared_ptr<PhysicsJointFixed>();
}

std::string PhysicsJointFixed::getClassName()
{
    return "rainbow::PhysicsJointFixed";
}

void PhysicsJointFixed::onSerialize(Serializer * serializer)
{
}

void PhysicsJointFixed::onDeserialize(Serializer * serializer)
{
}

}
