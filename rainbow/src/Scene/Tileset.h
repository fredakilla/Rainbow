#pragma once

#include "../Core/Serializable.h"

namespace rainbow
{

/**
 * Defines a text renderer.
 */
class Tileset : public Serializable
{
    friend class Activator;

public:

    /**
     * Constructor.
     */
    Tileset();
    
    /**
     * Destructor.
     */
    ~Tileset();

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
