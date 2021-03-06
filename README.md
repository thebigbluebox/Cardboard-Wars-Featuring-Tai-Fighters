# Cardboard Wars Featuring *Tai* Fighters #

## Group Members: Don Pham, Erica Cheyne, Ryan C Davis, Michael Currie, Tai Rui Tong ##

A stereoscopic 3D game made using OpenGL, designed for the [Google Cardboard](https://www.google.com/get/cardboard/).

If you loved the 2D version of space invaders, you’ll love it even more after we add another dimension to it. The objectives are to kill all the Tai fighters, and to survive the longest in the harsh outer space.


![alt-tag](http://i.imgur.com/4zg5NP2.png "Shoot down space ships before they shoot you")

![alt-tag](http://i.imgur.com/RlJQupY.png "Collect health packs")

## Controls: ##

 Key | Function
:---:|:---
 W | Increase forward speed
 A | Barrel roll left
 S | Decrease forward speed
 D |Barrel roll right
 LEFT | Look left
 RIGHT | Look right
 UP | Look up
 DOWN | Look down
 \+ | Zoom in
 \- | Zoom out
 R | Restarts game, reset game settings
 SPACE | Shoots projectile
 L | Light (On/Off)
 H | Full screen (On/Off)
 1 | Stereoscopic (On/Off)
 Q | Closes application and window

## Gameplay Mechanics ##

+ Destroy oncoming Tai fighters, and avoid getting hit by their projectiles.
+ The amount of lives will decrease by 1 when crashing into other Tai fighters, and by being hit by the Tai fighter’s projectiles.
+ Crashing into a health pack will increase the lives by 1.
+ Careful not to shoot health packs as they are destructible.
+ Destroying Tai fighters will increase your score by 1
+ Every 10 points, the level will increase. At each level increase, a harder type of enemy AI will be spawnable. Each AI type has its own distinct color.
+ Using stereoscopic display and Google Cardboard to show the game, and using the Moga Pro gamepad to control the game

## Required Library ##

+ FreeGLUT (glut.h)

## Starting the Program ##

+ This is a Visual Studio 2013 solution. An exe is in the debug folder.
+ There is also a Makefile included to compile to Windows and/or Linux.

