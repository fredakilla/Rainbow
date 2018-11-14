#pragma once

#include "../Scene/Component.h"
#include "../Scene/SceneObject.h"

namespace rainbow
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
