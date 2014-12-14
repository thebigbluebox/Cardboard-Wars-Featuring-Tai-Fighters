#include "GUIClass.h"

GUIClass::GUIClass()
{
	width = 100;
	height = 100;
}

GUIClass::GUIClass(GLdouble width, GLdouble height)
{
	width = width;
	height = height;
}

void GUIClass::set(GLdouble width, GLdouble height)
{
	width = width;
	height = height;
}

void GUIClass::draw(void)
{
	glPointSize(5);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-width/2, width/2, -height/2, height/2);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glColor3d(0,1,1);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glBegin(GL_POLYGON);
		
		glVertex2d(-width / 2 + width*0.3, -height / 2 + height*0.3);
		
		glVertex2d(width / 2 - width*0.3, -height / 2 + height*0.3);

		glVertex2d(width / 2 - width*0.3, height / 2 - height*0.3);

		glVertex2d(-width / 2 + width*0.3, height / 2 - height*0.3);
	glEnd();
	glBegin(GL_POLYGON);
		glVertex2d(-2, -2);

		glVertex2d(2, -2);

		glVertex2d(2, 2);

		glVertex2d(-2, 2);
	glEnd();
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

}