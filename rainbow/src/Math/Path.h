#pragma once

#include "../Core/Serializable.h"

namespace rainbow
{

/**
 * Defines a path renderer.
 */
class Path : public Serializable
{
    friend class Activator;

public:

    /**
     * Constructor.
     */
    Path();
    
    /**
     * Destructor.
     */
    ~Path();

protected:

    /**
     * @see Activator::createObject
     */
    static std::shared_ptr<Serializable> createObject();

    /**
     * @see Serializable::getClassName
     */
    std::string getClassName();
    
    /**
     * @see Serializable::onSerialize
     */
    void onSerialize(Serializer* serializer);
    
    /**
     * @see Serializable::onDeserialize
     */
    void onDeserialize(Serializer* serializer);
};

}
