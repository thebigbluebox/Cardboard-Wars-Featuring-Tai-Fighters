A Google Cardboard Implementation of a Flight Simulator
=================
This is a virtual reality flight simulator for android using google cardboard with interfaces to bluetooth controllers.

Lets Avoid Merge Conflicts and Problems
===============
Lets first do something call creating your own branches

Then before you start coding, pull from the origin so you got a nice fresh batch of code

Only push when the code still works push it to the main.

==================================================================

#VR-FlightSimulator
#####Erica Cheyne, Michael Currie, Ryan Davis, Don Pham, Tai Tong


###Graphics
A virtual terrain programmed in OpenGL. It will simulate a flying ship moving over the ground. Possibilities of different terrains, different weather conditions, changes in lighting based in a sun, shooting objects in the scene.

#####lighting 5%
Light sources that act as overhead lighting, point lights.

#####textures 10%
Ground textures.

#####particle system 5%
Snow and Rain.

#####picking 10%
Shooting and menu choices, controlled byt MOGA.

#####non-geo primatives 10%
Cockpit viewing.

#####extra 5-10%
See below.

###Cardboard - Tai
Will be used to view the scene, using a background app to send the current orientation to the computer via server.

###Controller - Ryan
Will be used to control movements of the user in the scene. 

Program in C++ using OpenGL and  GLUT.
Use a streaming application. 
Controlled with MOGA and Google cardboard.
Externally map MOGA to keybord and mouse.

