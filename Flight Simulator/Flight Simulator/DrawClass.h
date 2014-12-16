#include "stdafx.h"

class DrawClass{
public:
	GLdouble x = 0;
	GLdouble y = 0;
	GLdouble z = 0;
	GLdouble angleX;
	GLdouble angleY;
	GLdouble angleZ;
	GLdouble speedFactor = 1;
	

	/*Creates the scene object, 
	should be configurable with additional parameters*/
	DrawClass(void);

	/*Increase speed factor*/
	void increaseSpeed(void);
	
	/*Decrease speed factor*/
	void decreaseSpeed(void);

	/*Rolls the scene to the left*/
	void rollLeft(void);

	/*Rolls the scene to the right*/
	void rollRight(void);
	
	/*Pitches the scene downwards, in response to camera input
	of looking up which means person is looking away from scene*/
	void pitchUp(void);

	/*Pitches the scene upwards, in response to camera input
	of looking down which means person is looking towards the scene*/
	void pitchDown(void);
	
	/*Yaws the scene to the left*/
	void yawLeft(void);

	/*Yaws the scene to the left*/
	void yawRight(void);

	/*Translates the scene backwards 
	for movement thats equivalent to forwards*/
	void moveBackward(void);

	/*Translates the scene forwards 
	for movement thats equivalent to backwards*/
	void moveForward(void);

	/*Translates the scene left 
	for movement thats equivalent to the right*/
	void moveLeft(void);

	/*Translates the scene right 
	for movement thats equivalent to the left*/
	void moveRight(void);

	/*The drawing of the scene*/
	void draw(void);


};
