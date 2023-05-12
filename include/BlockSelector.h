#ifndef BLOCKSELECTOR_H_
#define BLOCKSELECTOR_H_

#include <ngl/Vec3.h>

class BlockSelector 
{
    // private and public are different access levels
    // public can be accessed anywhere in the program
    // all variables under a class are denoted with 'm_' as per NCCA coding standard
    public :
        BlockSelector();
        // move block
        void move();
        // move block to next position
        void moveNextPosition();

        // public variables
        // width of block
        float m_width = 1.0f;
        // depth of block
        float m_depth = 1.0f;
        // height of block
        float m_height = 1.0f;
        // displacement amount for block

        // position of block
        ngl::Vec3 m_position;
        // next position of block
        ngl::Vec3 m_nextPosition;
        // displacement of block
        ngl::Vec3 m_displacement;
        // when key is clicked block will move by this amount
        float m_displacementAmount = 0.2f;
        
        float m_radius;
        ngl::Vec3 m_scale;
        float m_blockScale = 1.0f;
        int m_isPickup = 0;
        int m_hasBeenPickedUp = 0;
        ngl::Vec3 m_dimensions;
        // start position of firing block
        float m_xStartPosition = -10.0f;
        // bullet has been fired
        int m_hasBeenFired = 0;
        // enemy has been killed
        int m_hasBeenKilled = 0;
        

    // private can only be accessd from within the class
    private : 
};

#endif