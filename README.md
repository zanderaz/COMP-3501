# Assignment 3, COMP3501
### By Aidan, Evan, Zander

=================
-> HOW TO PLAY <-
=================

Your goal is to reach all 8 checkpoints without being hit by an enemy, or colliding into too many asteroids. You can collide with power-ups to increase your max speed, grow in size to accelerate/decelerate quicker, and shrink in size to dodge obstacles better and rotate faster.

Controls are as follows:
'w' to accelerate
's' to decelerate
'q' to shrink
'e' to grow
'ijkl' rotates the orientation of your rocket
'u' to roll left
'o' to roll right

Visual Guide:
- Your ship is the white sphere in front of the camera
- Enemies are red spheres
- Asteroids are grey cubes
- The next checkpoint is a green sphere
- Future checkpoints are cyan cylinders
- Power ups are neon blue

========================
-> BUILD INSTRUCTIONS <-
========================

Place all the files in src/ into the src/ directory a new OF project.
There is also music included in the bin/data/ directory that you may add, but it's not necessary for the game to run (or for the assignment spec).

============================
-> IMPLEMENTATION DETAILS <-
============================

We decided to go with a third-person camera approach, as it seemed like the easiest way to depict growing and shrinking the player rocket.

We used an offset to make it so that the camera follows behind the player. The movement functionality was moved to the player class, and the position of the camera is update by the player in the player update function. 

All requirements from the assignment spec are attempted in the code and working.

None of the bonuses (other than the third person camera one mentioned in class?) were attempted.

Similar code structure to Zander's implementation for assignment 2. We built off his GameObject base class, deriving it as needed for other parts of the game. Everything else is pretty standard.

