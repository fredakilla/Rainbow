#pragma once

#include <vulkan/vulkan.h>

namespace rainbow
{

/**
 * Defines a command buffer for storing 
 * recorded gpu commands.
 */ 
class CommandBuffer
{
    friend class Graphics;

public: 

    /**
     * Constructor.
     */
    CommandBuffer();

    /**
     * Destructor.
     */
    ~CommandBuffer();

private:

    VkCommandBuffer _commandBuffer;
};

}
