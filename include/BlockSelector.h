#ifndef BLOCKSELECTOR_H_
#define BLOCKSELECTOR_H_

#include <ngl/Vec3.h>

class BlockSelector 
{
    public :
        BlockSelector();
        float displacementAmount = 0.2f;
        ngl::Vec3 position = {ngl::Vec3(0.0f,0.0f,0.0f)};
        ngl::Vec3 nextPosition;
        ngl::Vec3 displacement;
        float radius;
        ngl::Vec3 scale;
        float blockScale = 1.0f;
        int isPickup = 0;
        int isHit = 0;
        int hasBeenPickedUp = 0;
        ngl::Vec3 Velocityo;
        float xStartPosition = -10.0f;
        int hasBeenFired = 0;

        void move() {
            position = position + displacement;
        }

        void moveNextPosition() {
            nextPosition = position + displacement;
            
        }
};

#endif