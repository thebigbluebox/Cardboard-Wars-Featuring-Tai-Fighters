#include "stdafx.h"
#include "DrawClass.h"
#include "GUIClass.h"

struct setting {
	int windowx = 1000;
	int windowy = 1000;
	GLdouble sterooffset;
	GLdouble eyeDistance;
	GLdouble parallaxFactor;
	GLdouble convergenceDistance;
	GLdouble z = 0;
	GLdouble x = 0;
} Set;

int totalTime = 0;
bool keyStates[256] = { false }; // keyboard state
bool specialKeys[256] = { false };

DrawClass scene;
GUIClass gui;
EnemyHandler enemies = EnemyHandler();

GLfloat lightposition[] = { 0, 2, 0 };
Vector3 playerPos = { 0, 0, 0 }; // aka cam pos // the camera looks in the negative z direction
Vector3 lookAt = { 0, 0, -1 };
Vector3 cameraUp = { 0, 1, 0 };
Vector3 mover;
float thetaR = 0;
float thetaY = 0;
float thetaP = 0;

void updatePlayer(int deltaTime)
{
	mover = playerPos.directionTo(lookAt); // Vector3(lookAt.x - playerPos.x, lookAt.y - playerPos.y, lookAt.z - playerPos.z).normalize();
	//playerPos = playerPos.add(mover.scale(+0.1f));
	//lookAt.z -= 0.01;
	
}

void updateEnemies(int deltaTime)
{
	enemies.update(playerPos, deltaTime);
}

/* Keypresses for buttons that can happen at the same time */
void updateKeyboard(void)
{
	// WASD
	if (keyStates['w'] || keyStates['W']) {
		
	}
	else if (keyStates['s'] || keyStates['S']) {
		
	}
	if (keyStates['a'] || keyStates['A']) {
		thetaR += 0.1;
		cameraUp.x = cos(thetaR);
	}
	else if (keyStates['d'] || keyStates['D']) {
		thetaR -= 0.1;
		cameraUp.x = cos(thetaR);
	}

	// Special Keys
	if (specialKeys[GLUT_KEY_LEFT]) {
		thetaY -= 0.1;
		lookAt.x = cos(thetaY) + playerPos.x;
		lookAt.z = sin(thetaY) + playerPos.z;
	}
	else if (specialKeys[GLUT_KEY_RIGHT]) {
		thetaY += 0.1;
		lookAt.x = cos(thetaY) + playerPos.x;
		lookAt.z = sin(thetaY) + playerPos.z;
	}
	if (specialKeys[GLUT_KEY_UP]) {
		/*thetaP += 0.1;
		lookAt.y = 200 * cos(thetaP) + playerPos.y;
		lookAt.z = 200 * sin(thetaP) + playerPos.z;
		cameraUp.y = 300 * cos(thetaP);
		cameraUp.z = 300 * sin(thetaP);*/
	}
	else if (specialKeys[GLUT_KEY_DOWN]) {
		/*thetaP -= 0.1;
		lookAt.y = 300*cos(thetaP) + playerPos.y;
		lookAt.z = 300*sin(thetaP) + playerPos.z;
		cameraUp.y = 300*cos(thetaP);
		cameraUp.z = 300*sin(thetaP);*/
	}
	if (specialKeys[GLUT_KEY_PAGE_DOWN] || specialKeys[GLUT_KEY_HOME]) {
		scene.yawLeft();
	}
	else if (specialKeys[GLUT_KEY_PAGE_UP] || specialKeys[GLUT_KEY_END]) {
		scene.yawRight();
	}
}


/* timer function. */
void update(int value)
{
	// deltaTime
	int elapsedTime = glutGet(GLUT_ELAPSED_TIME);
	int deltaTime = elapsedTime - totalTime;
	totalTime = elapsedTime;

	// Other update routines
	updateKeyboard();
	updateEnemies(deltaTime);
	updatePlayer(deltaTime);

	glutPostRedisplay();
	glutTimerFunc(16, update, 0);
}

/* key released */
void keyboardUp(unsigned char key, int x, int y)
{
	keyStates[key] = false;
}

/* key pressed */
void keyboard(unsigned char key, int x, int y)
{
	keyStates[key] = true;
	switch (key)
	{
	case 'q':
	case 27:
		exit(0);
		break;
	case 'h':
	case 'H':
		glutFullScreen();
		break;
	}
	glutPostRedisplay();
}

void specialUp(int key, int x, int y)
{
	specialKeys[key] = false;
}

void special(int key, int x, int y)
{
	specialKeys[key] = true;

	switch (key)
	{
	}
}

void init(void)
{
	// Background Color
	glClearColor(0, 0, 0, 0);

	// Culling
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);

	// Shading
	//toggleShading(flatShading);
	glShadeModel(GL_SMOOTH);

	// Lighting
	//toggleLighting(lightingEnabled);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	float pureWhite[4] = { 1, 1, 1, 1 };
	float light0ambient[4] = { 0.804f, 1.f, 0.98f, 1.f };
	float light0diffuse[4] = { 0.804f, 1.f, 0.98f, 1.f };
	glLightfv(GL_LIGHT0, GL_AMBIENT, light0ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, pureWhite);

	// Meterials
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE); // use glColor for material

	gui.set(Set.windowx/2, Set.windowy);
	
}

void drawEnemies(void)
{
	for (auto it = enemies.list.begin(); it != enemies.list.end(); ++it) {
		glPushMatrix();
			glColor3fv(it->color.v);
			glTranslatef(it->position.x, it->position.y, it->position.z);
			glutSolidCube(it->size);
		glPopMatrix();
	}
}

void draw(void)
{
	drawEnemies();
	//scene.draw();
	//gui has problems with coordinating to the two eyes turned off for comfort.
	//gui.draw();
}

/* display function - GLUT display callback function
*		clears the screen, sets the camera position, draws the ground plane and movable box
*/
void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//Viewport Left
	glViewport(0, 0, Set.windowx / 2, Set.windowy);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(125, (Set.windowx / 2.0) / (Set.windowy), 1, 200);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0.2f, 0, 0);
	gluLookAt(playerPos.x, playerPos.y, playerPos.z, lookAt.x, lookAt.y, lookAt.z, cameraUp.x, cameraUp.y, cameraUp.z);
	draw();
	
	//Viewport Right
	glViewport(Set.windowx / 2, 0, Set.windowx / 2, Set.windowy);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(65, (Set.windowx / 2.0) / (Set.windowy), 1, 200);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(-0.2f, 0, 0);
	gluLookAt(playerPos.x, playerPos.y, playerPos.z, lookAt.x, lookAt.y, lookAt.z, cameraUp.x, cameraUp.y, cameraUp.z);
	draw();

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
	glutKeyboardUpFunc(keyboardUp);
	glutSpecialFunc(special);
	glutSpecialUpFunc(specialUp);

	glutTimerFunc(16, update, 0);

	init();

	glutMainLoop();	
	return(0);	
}