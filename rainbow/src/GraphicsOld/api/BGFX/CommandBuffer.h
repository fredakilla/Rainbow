#pragma once

namespace gameplay
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

};

}
