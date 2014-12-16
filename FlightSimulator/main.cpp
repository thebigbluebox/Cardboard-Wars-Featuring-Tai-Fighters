#include "stdafx.h"

// Time
int totalTime = 0;

// Lights
float light0[4] = { 5,5,5,1 };
float light1[4] = { -5,-5,-5,1 };
Vector3 playerPos = { 0, 0, 0 }; // aka cam pos // the camera looks in the negative z direction
Vector3 lookAt = { 0, 0, -1 };

// Environment
EnemyHandler enemies = EnemyHandler();

// States
bool keyStates[256] = { false }; // keyboard state
int windowX = 0;
int windowY = 0;
float roll = 0;
float yaw = 0;
float pitch = 0;

void updateEnemies(int deltaTime)
{
	enemies.update(playerPos, deltaTime);
}

void updatePlayer(int deltaTime)
{
	//printf("deltaTime: %d\n", deltaTime);
	//playerPos.z -= 0.01f;
	//lookAt = playerPos;
	//lookAt.z -= 1;

	if (keyStates['z'])
	{
		printf("%f %f %f\n", lookAt.x, lookAt.y, lookAt.z);
	}
	if (keyStates['w']) {
		lookAt.y += 1;
	}
	else if (keyStates['s']) {
		lookAt.y -= 1;
	}
	if (keyStates['a']) {
		yaw -= (yaw > -PI / 2.0f) ? PI / 12.0f : 0;
	}
	else if (keyStates['d']) {
		yaw += (yaw < PI / 2.0f) ? PI / 12.0f : 0;
	}
	if (keyStates['r']) {
		pitch += (pitch < PI / 2.0f) ? PI / 12.0f : 0;
	}
	else if (keyStates['f']) {
		pitch -= (pitch > -PI / 2.0f) ? PI / 12.0f : 0;
	}

	//printf("%f", pitch);
	//lookAt.x = cos(yaw);
	lookAt.y = sin(pitch);
	lookAt.x = sin(yaw); //sin(pitch) * cos(yaw);

	// Move to where the player is looking
	
	Vector3 normPos = playerPos.directionTo(lookAt);
	lookAt = playerPos.add(normPos);
	playerPos = playerPos.add(normPos.scale(0.02f));
	printf("%f %f %f\n", playerPos.x, playerPos.y, playerPos.z);
	printf("%f \n", yaw);
	/*
	lookAt = playerPos;
	lookAt.z -=1;
	*/
	if (!keyStates['d']) {
		yaw = (yaw + PI / 24.0f < 0) ? 0 : yaw + PI / 24.0f;
	}
	if (!keyStates['a']) {
		yaw = (yaw - PI / 24.0f > 0) ? 0 : yaw - PI / 24.0f;
	}
	if (!keyStates['r']) {
		if (pitch > 0.2) pitch -= PI / 24.0f;
	}
	if (!keyStates['f']) {
		if (pitch < -0.2) pitch += PI / 24.0f;
	}
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


/********************** glut functions below this line *****************/

/* timer function. */
void update(int value)
{
	int elapsedTime = glutGet(GLUT_ELAPSED_TIME);
	int deltaTime = elapsedTime - totalTime;
	totalTime = elapsedTime;

	// Lights
	glLightfv(GL_LIGHT0, GL_POSITION, light0);
	glLightfv(GL_LIGHT1, GL_POSITION, light1);

	updatePlayer(deltaTime);
	updateEnemies(deltaTime);

	glutPostRedisplay();
	glutTimerFunc(16, update, 0);
}

void draw(void)
{
	drawEnemies();
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//Viewport Left
	glViewport(0, 0, windowX / 2, windowY);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(65, (windowX / 2.0f) / (windowY), 1, 50);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0.2f, 0, 0);
	gluLookAt(playerPos.x, playerPos.y, playerPos.z, lookAt.x, lookAt.y, lookAt.z, 0, 1, 0);
	draw();

	//Viewport Right
	glViewport(windowX / 2, 0, windowX / 2, windowY);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(65, (windowX / 2.0f) / (windowY), 1, 50);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(-0.2f, 0, 0);
	gluLookAt(playerPos.x, playerPos.y, playerPos.z, lookAt.x, lookAt.y, lookAt.z, 0, 1, 0);
	draw();


	glutSwapBuffers();
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
	glShadeModel(GL_FLAT);

	// Lighting
	//toggleLighting(lightingEnabled);
	glEnable(GL_LIGHTING);
	float pureWhite[4] = { 1, 1, 1, 1 };
	float light0ambient[4] = { 0.804f, 1.f, 0.98f, 1.f };
	float light0diffuse[4] = { 0.804f, 1.f, 0.98f, 1.f };
	float light1ambient[4] = { 0.8f, 0.2f, 0.4f, 1.f };
	float light1diffuse[4] = { 0.8f, 1.f, 0.4f, 1.f };
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_POSITION, light0);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light0ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, pureWhite);
	glEnable(GL_LIGHT1);
	glLightfv(GL_LIGHT1, GL_POSITION, light1);
	glLightfv(GL_LIGHT1, GL_AMBIENT, light1ambient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light1diffuse);
	glLightfv(GL_LIGHT1, GL_SPECULAR, pureWhite);

	// Meterials
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE); // use glColor for material
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
	switch (key) {
	case 'q':
		exit(0);
		break;
	}
}

void reshape(int w, int h)
{
	windowX = w;
	windowY = h;
	/*
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, (float)w / (float)h, 1, 500);
	*/
}

int main(int argc, char* argv[])
{
	srand(time(NULL));

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);

	glutInitWindowSize(1920/2, 1080/2);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Flight Simulator");

	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutKeyboardUpFunc(keyboardUp);
	/*glutKeyboardFunc(keyboard);
	glutSpecialFunc(special);
	glutSpecialUpFunc(specialUp);
	glutMouseFunc(activeMouse);
	glutMotionFunc(passiveMouse);*/
	glutTimerFunc(16, update, 0);
	init();

	glutMainLoop();
	return(0);
}
