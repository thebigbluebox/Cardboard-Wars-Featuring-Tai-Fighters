#include "GlutHeader.h"

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

	/*Rotates the scene to the left*/
	void rotateLeft(void);

	/*Rotates the scene to the right*/
	void rotateRight(void);
	
	/*Angles the scene downwards, in response to camera input
	of looking up which means person is looking away from scene*/
	void angleUp(void);

	/*Angles the scene upwards, in response to camera input
	of looking down which means person is looking towards the scene*/
	void angleDown(void);

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
