#include "../Core/Base.h"
#include "../Physics/PhysicsRigidBody.h"
#include "../Physics/PhysicsMaterial.h"

namespace rainbow
{

PhysicsMaterial::PhysicsMaterial()
{
}

PhysicsMaterial::~PhysicsMaterial()
{
}

void PhysicsMaterial::setDynamicFriction(float dynamicFriction)
{
}

float PhysicsMaterial::getDynamicFriction() const
{
    return 0.0f;
}

void PhysicsMaterial::setStaticFriction(float staticFriction)
{
}

float PhysicsMaterial::getStaticFriction() const
{
    return 0.0f;
}

void PhysicsMaterial::setFrictionCombineMode(PhysicsMaterial::CombineMode combineMode)
{
}

PhysicsMaterial::CombineMode PhysicsMaterial::getFrictionCombineMode()
{
    return CombineMode::eAverage;
}

void PhysicsMaterial::setBounce(float bounce)
{
}

float PhysicsMaterial::getBounce() const
{
    return 0.0f;
}

void PhysicsMaterial::setBounceCombineMode(PhysicsMaterial::CombineMode combineMode)
{
}

PhysicsMaterial::CombineMode PhysicsMaterial::getBounceCombineMode()
{
    return CombineMode::eAverage;
}

}
