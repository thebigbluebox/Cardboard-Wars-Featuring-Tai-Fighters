#include "GlutHeader.h"
#include "DrawClass.h"
#include "GUIClass.h"
#include <stdio.h>
struct setting{
	GLdouble windowx = 1000;
	GLdouble windowy = 1000;
	GLdouble sterooffset;
	GLdouble eyeDistance;
	GLdouble parallaxFactor;
	GLdouble convergenceDistance;
	GLdouble z = 0;
	GLdouble x = 0;
}Set;
	GLfloat lightposition[] = { 0, 2, 0 };
	DrawClass scene;
	GUIClass gui;

void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'q':
	case 27:
		exit(0);
		break;

	case 'w':
	case 'W':
		scene.moveForward();
		break;
	case 'a':
	case 'A':
		scene.moveLeft();
		break;
	case 's':
	case 'S':
		scene.moveBackward();
		break;
	case 'd':
	case 'D':
		scene.moveRight();
		break;

	}
	glutPostRedisplay();
}

void special(int key, int x, int y)
{
	/* arrow key presses move the camera */
	switch (key)
	{
	case GLUT_KEY_LEFT:
		scene.rollLeft();
		//Set.x += 0.1;
		break;

	case GLUT_KEY_RIGHT:
		scene.rollRight();
		//Set.x -= 0.1;
		break;

	case GLUT_KEY_UP:
		scene.pitchDown();
		//Set.z += 0.1;
		break;

	case GLUT_KEY_DOWN:
		scene.pitchUp();
		//Set.z -= 0.1;
		break;
	case GLUT_KEY_PAGE_DOWN:
	case GLUT_KEY_HOME:
		scene.yawLeft();
		break;
	case GLUT_KEY_PAGE_UP:
	case GLUT_KEY_END:
		scene.yawRight();
		break;
	}
	glutPostRedisplay();
}

void init(void)
{
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);
	//not sure we still need to enable color material?
	glEnable(GL_COLOR_MATERIAL);

	glEnable(GL_CULL_FACE);
	gui.set(Set.windowx/2, Set.windowy);
	
}

/* display function - GLUT display callback function
*		clears the screen, sets the camera position, draws the ground plane and movable box
*/
void display(void)
{
	glLightfv(GL_LIGHT0, GL_POSITION, lightposition);
	glShadeModel(GL_SMOOTH);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	//Viewport Left
	glViewport(0, 0, Set.windowx / 2, Set.windowy);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(65, (Set.windowx / 2) / (Set.windowy), 1, 50);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslated(0.2, 0, 0);
	gluLookAt(0, 0, 6, 0, 0, 0, 0, 1, 0);
	scene.draw();
	//gui has problems with coordinating to the two eyes turned off for comfort.
	//gui.draw();
	
	
	//Viewport Right
	glViewport(Set.windowx / 2, 0, Set.windowx / 2, Set.windowy);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(65, (Set.windowx / 2) / (Set.windowy), 1, 50);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslated(-0.2, 0, 0);
	gluLookAt(0, 0, 6, 0, 0, 0, 0, 1, 0);
	scene.draw();
	
	//gui.draw();

	glutSwapBuffers();
}

void reshape(int x, int y)
{
	Set.windowx = x;
	Set.windowy = y;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, (Set.windowx / 2) / (Set.windowy), 1, 100);
	gui.set(x, y);
}

/* main function - program entry point */
int main(int argc, char** argv)
{
	
	glutInit(&argc, argv);		
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	glutInitWindowSize(Set.windowx, Set.windowy);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Final Assignment");

	glutReshapeFunc(reshape);
	glutDisplayFunc(display);	
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(special);

	init();

	glutMainLoop();	
	return(0);	
}