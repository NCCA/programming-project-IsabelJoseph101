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
        void move();
        void moveNextPosition();

        // public variables
        float m_width = 1.0f;
        float m_depth = 1.0f;
        float m_height = 1.0f;
        float m_displacementAmount = 0.2f;
        ngl::Vec3 m_position = {ngl::Vec3(0.0f, 0.0f, 7.0f)};
        ngl::Vec3 m_displacement;
        ngl::Vec3 m_nextPosition;
        float m_radius;
        ngl::Vec3 m_scale;
        float m_blockScale = 1.0f;
        int m_isPickup = 0;
        int m_isHit = 0;
        int m_hasBeenPickedUp = 0;
        ngl::Vec3 m_dimensions;
        float m_xStartPosition = -10.0f;
        int m_hasBeenFired = 0;
        int m_hasBeenKilled = 0;
    
    // private can only be accessd from within the class
    private : 
};

#endif