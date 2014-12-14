#include "GlutHeader.h"
#include "DrawClass.h"
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

void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'q':
	case 27:
		exit(0);
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
		scene.moveLeft();
		//Set.x += 0.1;
		break;

	case GLUT_KEY_RIGHT:
		scene.moveRight();
		//Set.x -= 0.1;
		break;

	case GLUT_KEY_UP:
		scene.moveForward();
		//Set.z += 0.1;
		break;

	case GLUT_KEY_DOWN:
		scene.moveBackward();
		//Set.z -= 0.1;
		break;

	case GLUT_KEY_HOME:
		scene.angleUp();
		break;

	case GLUT_KEY_END:
		scene.angleDown();
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

	glutSwapBuffers();
}

void reshape(int x, int y)
{
	Set.windowx = x;
	Set.windowy = y;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, (Set.windowx / 2) / (Set.windowy), 1, 100);
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