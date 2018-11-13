#pragma once

#include "Renderer.h"

namespace rainbow
{

/**
 * Defines a text renderer.
 */
class Text : public Renderer
{
    friend class Graphics;
    friend class Activator;
    friend class SceneObject;

public:

    /**
     * Constructor.
     */
    Text();
    
    /**
     * Destructor.
     */
    ~Text();

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
