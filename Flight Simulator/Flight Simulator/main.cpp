#include "GlutHeader.h"
#include <stdio.h>
struct setting{
	GLdouble windowx = 1000;
	GLdouble windowy = 1000;
}Set;
	GLfloat lightposition[] = { 0, 2, 0 };


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

		break;

	case GLUT_KEY_RIGHT:

		break;

	case GLUT_KEY_UP:

		break;

	case GLUT_KEY_DOWN:

		break;

	case GLUT_KEY_HOME:

		break;

	case GLUT_KEY_END:

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
	glEnable(GL_COLOR_MATERIAL);

	glEnable(GL_CULL_FACE);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, (Set.windowx/2)/(Set.windowy), 1, 100);
	printf("%d",Set.windowx / Set.windowy);
	//gluPerspective(45, 1, 1, 100);

}

/* display function - GLUT display callback function
*		clears the screen, sets the camera position, draws the ground plane and movable box
*/
void display(void)
{
	glLightfv(GL_LIGHT0, GL_POSITION, lightposition);
	glShadeModel(GL_SMOOTH);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, Set.windowx/2, Set.windowy);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	gluLookAt(1, 0, 6, 0, 0, 0, 0, 1, 0);
	
	glColor3f(1, 1, 1);
	glutSolidCone(0.7, 0.5, 15, 30);

	glViewport(Set.windowx / 2, 0, Set.windowx / 2, Set.windowy);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(-1, 0, 6, 0, 0, 0, 0, 1, 0);

	glColor3f(1, 1, 1);
	glutSolidCone(0.7, 0.5, 15, 30);
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