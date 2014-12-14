#include "DrawClass.h"

DrawClass::DrawClass(void)
{

}

void DrawClass::increaseSpeed(void)
{
	speedFactor += 0.4;
}

void DrawClass::decreaseSpeed(void)
{
	if (speedFactor -= 0.4 <= 0)
	{
		speedFactor = 0;
	}
	else
	{
		speedFactor -= 0.4;
	}
}

void DrawClass::rotateLeft(void)
{

}

void DrawClass::rotateRight(void)
{

}

void DrawClass::angleUp(void)
{
	angleX += 0.2;
}

void DrawClass::angleDown(void)
{
	angleX -= 0.2;
}

void DrawClass::moveBackward(void)
{
	z -= 0.2*speedFactor;
}

void DrawClass::moveForward(void)
{
	z += 0.2*speedFactor;
}

void DrawClass::moveLeft(void)
{
	x -= 0.2*speedFactor;
}

void DrawClass::moveRight(void)
{
	x += 0.2*speedFactor;
}

void DrawClass::draw(void)
{
	GLfloat mat_ambient[] = { 1, 1, 1, 1 };
	GLfloat mat_diffuse[] = { 0.78, 0.57, 0.11, 1.0 };
	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat mat_shininess[] = { 70 };
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
	glTranslated(x, 0, z);
	glRotated(angleX, 1, 0, 0);
	glColor3f(1, 0, 1);
	glBegin(GL_QUADS);
		
		glVertex3f(-5,-2,-5);

		glVertex3f(5,-2,-5);
		
		glVertex3f(5, 2,-5);
		
		glVertex3f(-5,2,-5);
	glEnd();
	glColor3f(1, 1, 1);
	glBegin(GL_QUADS);

		glVertex3f(-5, -2, -5);

		glVertex3f(-5, -2, 5);

		glVertex3f(5, -2, 5);

		glVertex3f(5, -2, -5);
	glEnd();
	glColor3f(1, 0, 0);
	glutSolidCone(0.5, 2, 50, 60);
}