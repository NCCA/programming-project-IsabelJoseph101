# Isabel Joseph (s5221926) Assignment 

## Overview
A 3D space invaders game that involves interaction and collision with other objects.

The block selector class stores information such as the position, next position, dimensions, box sizes and other relevant information. 

This class was instanced multiple times in NGLScene for the ground blocks, the main player block, enemy block, bullets and the barriers.

I created a function that calculates the axis aligned bounding box (aabb) collisions between two blocks, where it essentially creates a bounding box area and then checks if these boxes overlap over the x,y and z axis.

These were used to determine if a bullet was colliding with an enemy block. If a bullet hit an enemy, this would delete the enemy block from the main screen by moving it to another location off screen. Then adding a score of 40 for every enemy that the user has killed.

Another use of this collision system was implemented with the bullet and the barriers. Just like space invaders, players are able to destroy barrier blocks by hitting them.

Enemies were created so that they move left and right and occasionally down, using a for loop that changes its position using a time variable and as well as incremental position values.

This was also based on Jon Macy’s firstPersonCamera demo and the snake game.


## Keys