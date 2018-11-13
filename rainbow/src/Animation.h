#pragma once

#include "Component.h"
#include "SceneObject.h"

namespace prana
{

/**
 * Defines an animation componenent.
 */
class Animation : public Component
{
    friend class SceneObject;
    friend class Activator;

public:

    /**
     * Constructor.
     */
    Animation();

    /**
     * Destructor
     */
    ~Animation();
};

}
