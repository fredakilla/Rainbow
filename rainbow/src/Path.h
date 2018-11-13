#pragma once

#include "Renderer.h"

namespace prana
{

/**
 * Defines a path renderer.
 */
class Path : public Renderer
{
    friend class Graphics;
    friend class Activator;
    friend class SceneObject;

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
