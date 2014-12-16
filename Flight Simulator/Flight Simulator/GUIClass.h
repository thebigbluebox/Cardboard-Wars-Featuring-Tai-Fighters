#include "stdafx.h"

class GUIClass{
public:
	GLdouble width;
	GLdouble height;
	/*Creates the GUI object,
	should be configurable with additional parameters*/
	GUIClass(void);

	GUIClass(GLdouble width, GLdouble height);
	
	/*Sets the GUI's height and width so it can be resized*/
	void set(GLdouble width, GLdouble height);
	
	/*The drawing of the GUI*/
	void draw(void);
};
