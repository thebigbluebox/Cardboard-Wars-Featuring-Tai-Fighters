#include "stdafx.h"
#include "DrawClass.h"
#include "Hud.h"
#include "main.h"
#include "TextureLoader.h"

struct setting {
	int windowx = 1920/2;
	int windowy = 1080/2;
	//float stereoOffset;
	float eyeDistance = 0.2f;
	//float parallaxFactor;
	//float convergenceDistance;
} Set;

int totalTime = 0;
bool keyStates[256] = { false }; // keyboard state
bool specialKeys[256] = { false };

Hud hud;
EnemyHandler enemies = EnemyHandler();
GameInfo gameInfo = { 3, 3 };
GameInfo getGameInfo(void){	return gameInfo;}

double thetaR = 3.14 / 2;
double thetaY = 3 * 3.14 / 2;
GLfloat lightposition[] = { 0, 2, 0 };
Vector3 playerPos = { 0, 0, 0 }; // aka cam pos // the camera looks in the negative z direction
Vector3 lookAt = { 300 * cosf(thetaY), 0, 300 * sinf(thetaY) };
Vector3 cameraUp = { 300 * cosf(thetaR), 300 * sinf(thetaR), 0 };
Vector3 mover;
double thetaP = 3.14;
float between;
int count = 0;

void updateGameInfo(void)
{
	gameInfo.score = totalTime/100;
	gameInfo.lives = 3;
}



void updatePlayer(int deltaTime)
{
	playerPos.z -= 0.01;
	lookAt.z -= 0.01;
	mover = Vector3(lookAt.x - playerPos.x, lookAt.y - playerPos.y, lookAt.z - playerPos.z).normalize();
	playerPos.x += mover.x/80;
	playerPos.y += mover.y/80;
	playerPos.z += mover.z/80;
	lookAt.x += mover.x / 80;
	lookAt.y += mover.y / 80;
	lookAt.z += mover.z / 80;
	Vector3 y = { 0, 1, 0 };
	between = acos(dotproduct(cameraUp, y) / (magnitude(y)*magnitude(cameraUp)));
	/*Vector3 rotater = crossproduct(lookAt, cameraUp);
	Vector3 x = { 1, 0, 0 };
	between = acos(dotproduct(x, rotater)/(magnitude(x)*magnitude(rotater)));*/
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
		cameraUp.x = 300 * cos(thetaR)/50;
		cameraUp.y = 300 * sin(thetaR)/50;
		
		//glRotatef(1,cameraUp.x, cameraUp.y, cameraUp.z);
		
	}
	else if (keyStates['d'] || keyStates['D']) {
		thetaR -= 0.1;
		cameraUp.x = 300 * cos(thetaR)/50;
		cameraUp.y = 300 * sin(thetaR)/50;
		
	}

	//space
	if (keyStates[' '])
	{
		enemies.spawnBullet(playerPos, playerPos.directionTo(lookAt));
	}

	// Special Keys
	if (specialKeys[GLUT_KEY_LEFT] && thetaY > 7*3.14/6) {
		thetaY -= 0.01;
		lookAt.x = 300 * cos(thetaY);
		lookAt.z = 300 * sin(thetaY);
		//Set.x += 0.1;
	}
	else if (specialKeys[GLUT_KEY_RIGHT] && thetaY <11*3.14/6) {
		thetaY += 0.01;
		lookAt.x = 300*cos(thetaY);
		lookAt.z = 300*sin(thetaY);
		//lookAt.x += 0.1;
		
	}
	if (specialKeys[GLUT_KEY_UP] && lookAt.y < 400/*&& thetaP > 4*3.14/6*/) {
		lookAt.y += 3;
		//lookAt.x += cameraUp.x;
		/*thetaP -= 0.01;
		lookAt.y = 300*sin(thetaP);
		lookAt.z = 300*cos(thetaP);*/
		//cameraUp.y = 300 * sin(thetaP - 3.14 / 2);
		//cameraUp.z = 300 * cos(thetaP - 3.14 / 2);
		
	}
	else if (specialKeys[GLUT_KEY_DOWN] && lookAt.y > -400 /*(thetaP-between) < 8*3.14/6*/) {
			lookAt.y -= 3;
			//lookAt.x -= cameraUp.x;
			/*thetaP += 0.01;
			float thetaU = thetaP - (3.14 / 2);
			lookAt.y = 300 * sin(thetaP-between);
			lookAt.z = 300 * cos(thetaP-between);
			cameraUp.y = 300 * sin(thetaU - between);
			cameraUp.z = 300 * cos(thetaU - between);
			printf("theta %d between %d \n", thetaP, between);*/

		
	}
	if (specialKeys[GLUT_KEY_PAGE_DOWN] || specialKeys[GLUT_KEY_HOME]) {
		
	}
	else if (specialKeys[GLUT_KEY_PAGE_UP] || specialKeys[GLUT_KEY_END]) {
		
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
	updateGameInfo();

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

	// Textures
	glEnable(GL_TEXTURE_2D);
	loadTextures();
		}


void draw(void)
{
	enemies.drawEnemies();
	enemies.updateBullets();
	//hud.draw(); // moved to display
}



/* display function - GLUT display callback function
*		clears the screen, sets the camera position, draws the ground plane and movable box
*/
void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glMatrixMode(GL_MODELVIEW);
	//glLoadIdentity();

	//Viewport Left
	glViewport(0, 0, Set.windowx / 2, Set.windowy);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(65, (Set.windowx / 2.0) / (Set.windowy), 1, 200);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(Set.eyeDistance, 0, 0);
	glPushMatrix(); // Pushing to attempt to save the translate for hud.draw(); not the right idea.
	gluLookAt(playerPos.x, playerPos.y, playerPos.z, lookAt.x, lookAt.y, lookAt.z, cameraUp.x, cameraUp.y, cameraUp.z);
	draw();
	glPopMatrix(); // pop!
	hud.draw();
	
	//Viewport Right
	glViewport(Set.windowx / 2, 0, Set.windowx / 2, Set.windowy);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(65, (Set.windowx / 2.0) / (Set.windowy), 1, 200);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(-Set.eyeDistance, 0, 0);
	glPushMatrix(); // push!
	gluLookAt(playerPos.x, playerPos.y, playerPos.z, lookAt.x, lookAt.y, lookAt.z, cameraUp.x, cameraUp.y, cameraUp.z);
	draw();
	glPopMatrix(); // pop!
	hud.draw();

	glutSwapBuffers();
}

void reshape(int x, int y)
{
	Set.windowx = x;
	Set.windowy = y;
	hud.setWindowSize(x, y);
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