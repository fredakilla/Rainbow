#include "Base.h"
#include "PhysicsJointSocket.h"

namespace rainbow
{

PhysicsJointSocket::PhysicsJointSocket() 
{
}

PhysicsJointSocket::~PhysicsJointSocket()
{
}

PhysicsJoint::Type PhysicsJointSocket::getType() const
{
    return Type::eSocket;
}

void PhysicsJointSocket::setLimitEnabled(bool enabled)
{
}

bool PhysicsJointSocket::isLimitEnabled() const
{
    return false;
}

void PhysicsJointSocket::setLimitAngleY(float angleY)
{
}

float PhysicsJointSocket::getLimitAngleY() const
{
    return 0.0f;
}

void PhysicsJointSocket::setLimitAngleZ(float angleZ)
{
}

float PhysicsJointSocket::getLimitAngleZ() const
{
    return 0.0f;
}

void PhysicsJointSocket::setLimitContactDistance(float contactDistance)
{
}

float PhysicsJointSocket::getLimitContactDistance() const
{
    return 0.0f;
}

void PhysicsJointSocket::setLimitBounce(float bounce)
{
}

float PhysicsJointSocket::getLimitBounce() const
{
    return 0.0f;
}

void PhysicsJointSocket::setLimitBounceThreshold(float bounceThreshold)
{
}

float PhysicsJointSocket::getLimitBounceThreshold() const
{
    return 0.0f;
}

void PhysicsJointSocket::setLimitSpringEnabled(bool enabled)
{
}

bool PhysicsJointSocket::isLimitSpringEnabled()
{
    return false;
}

void PhysicsJointSocket::setLimitSpringStiffness(float stiffness)
{
}

float PhysicsJointSocket::getLimitSpringStiffness() const
{
    return 0.0f;
}

void PhysicsJointSocket::setLimitSpringDampening(float dampening)
{
}

float PhysicsJointSocket::getLimitSpringDampening() const
{
    return 0.0f;
}

std::shared_ptr<Serializable> PhysicsJointSocket::createObject()
{
    return std::shared_ptr<PhysicsJointSocket>();
}

std::string PhysicsJointSocket::getClassName()
{
    return "rainbow::PhysicsJointSocket";
}

void PhysicsJointSocket::onSerialize(Serializer * serializer)
{
    PhysicsJoint::onSerialize(serializer);
}

void PhysicsJointSocket::onDeserialize(Serializer * serializer)
{
    PhysicsJoint::onDeserialize(serializer);
}

}
