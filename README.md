# Isabel Joseph (s5221926) Assignment 

## Overview
A 3D space invaders inspired game that involves interaction and collision with other objects. 

I transformed the popular retro game that came out in 1978, and made it a 3D version of the game where you can pan and move around in the scene. In addition, for an interesting change I decided to elevate the dark theme of the game and had it feature light pastel colours. Using Jon Macy’s firstPersonCamera demo and the snake game as a basis I began development on this game.

The block selector class stores information such as key variables such as position, width, and a variable representing if a block has been picked up.It also stores functions such as ‘moveNextPosition’, which calculates the next position of the player using an equation. This class was instanced multiple times in NGLScene for the ground blocks, the main player block, enemy block, bullets and the barriers. By creating many instances we apply the object oriented concept of composition and polymorphism. Through calling on the ‘BlockSelector’ class, each instance inherits its variables and functions.

To create the grid, I specify a number of rows and a number for the columns, which was used through a for loop to create the grid of blocks. Then to add the pastel colour to the grid this was achieved by creating a function that takes in the inputs of hue, saturation, and value.  It uses these to calculate RGB values that range from 0 to 360, which has it sets variable values to generate a pastel gradient on the ground blocks. This function was passed through the grid creation for loop which successfully applied it and changed its colour.

In terms of collisions, which for this game had to be applied to the bullet when it hits an enemy and also to a bullet when it hits a barrier. To do this I created a function that calculates the axis aligned bounding box (AABB) collisions between two blocks, where it essentially creates a bounding box area and then checks if these boxes overlap over the x,y and z axis. They take in the position, width, depth and height of two blocks and calculate if it's been collided or not. Once this was complete it was used to determine if a bullet was colliding with an enemy block. If a bullet hit an enemy, this would delete the enemy block from the main screen by moving it to another location off screen. Then adding a score of 40 for every enemy that the user has killed. To show this score, I used Qt for the generation of text showing the score and its updates for when a collision is detected with a bullet and enemy. In addition Qt was also used to inform the player that they’ve won the game, when the player manages to kill all the enemies.

Another use of this collision system was implemented with the bullet and the barriers. Just like space invaders, players are able to destroy barrier blocks by hitting them, and I attempted to replicate this function in my game.

Enemies were created as an instantiation of the Block selector. Their main function is to move left and right and occasionally down, using a for loop that changes its position using a time variable and as well as incremental position values. 


![screenshot of game](https://github.com/NCCA/programming-project-IsabelJoseph101/blob/main/images/screenshot.png)

## Game Controls
* **Left and Right:** movement
* **Space:** shooting
* **W,A,S,D:** moving camera
* **Mouse:** panning around scene