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
	int recoilTime = 300; //in ms
} Set;

int totalTime = 0;
bool keyStates[256] = { false }; // keyboard state
bool specialKeys[256] = { false };

Hud hud;
EnemyHandler enemies = EnemyHandler();
GameInfo gameInfo = { 0, 3, 0, 5, 5 , false}; //score, lives
GameInfo getGameInfo(void){	return gameInfo;}

double thetaP = 3.14159;
double thetaR = 3.14159 / 2.0f;
double thetaY = 3 * 3.14159 / 2.0f;
int rollDirection = 1;
bool isRolling = false;
float playerSpeed = 1.0f/40.0f;

Vector3 playerPos = { 0, 0, 0 }; // aka cam pos // the camera looks in the negative z direction
Vector3 lookAt = { 300 * cosf(thetaY), 0, 300 * sinf(thetaY) };
Vector3 cameraUp = { 300 * cosf(thetaR), 300 * sinf(thetaR), 0 };
Vector3 mover;
GLfloat lightposition[] = { 0, 2, 0 };

//float oldx;
//float oldy;

//float between;
//int count = 0;

void updateGameInfo(void)
{
	//gameInfo.score = totalTime/100;
	//gameInfo.lives = 3;
}



void updatePlayer(int deltaTime)
{	
	lookAt.z -= 0.01;
	mover = playerPos.directionTo(lookAt);
	playerPos = playerPos.add(mover.scale(playerSpeed));
	lookAt = lookAt.add(mover.scale(playerSpeed));

	//Vector3 y = { 0, 1, 0 };
	//between = acos(dotproduct(cameraUp, y) / (magnitude(y)*magnitude(cameraUp)));
	/*Vector3 rotater = crossproduct(lookAt, cameraUp);
	Vector3 x = { 1, 0, 0 };
	between = acos(dotproduct(x, rotater)/(magnitude(x)*magnitude(rotater)));*/

	// Roll
	if (isRolling) {
		thetaR += rollDirection*(3.14159f / 30.0f); // rotation amount
		
		if (sin(thetaR) < 1) { // rotate camera
			cameraUp.x = 300 * cos(thetaR);
			cameraUp.y = 300 * sin(thetaR);
		}	
		if (cameraUp.y >= 300) { // stop rolling
			isRolling = false;
		}
	}
}

void updateEnemies(int deltaTime)
{
	enemies.update(playerPos, deltaTime);
}

/*increases speed*/
void speedUp(){
	playerPos.x += mover.x / 20.0f;
	playerPos.y += mover.y / 20.0f;
	playerPos.z += mover.z / 20.0f;
}


/* Keypresses for buttons that can happen at the same time */
void updateKeyboard(void)
{
	// WASD
	if (keyStates['w'] || keyStates['W']) {
		playerSpeed += (playerSpeed < 0.1f) ? 0.002f : 0;
	}
	else if (keyStates['s'] || keyStates['S']) {
		playerSpeed += (playerSpeed > 0.02f) ? -0.002f : 0;
	}
	if ((keyStates['a'] || keyStates['A'])) {
		if (!isRolling) rollDirection = -1;
		isRolling = true;	
	}
	else if (keyStates['d'] || keyStates['D']) {
		if (!isRolling) rollDirection = 1;
		isRolling = true;
	}

	//space
	if (keyStates[' '] && gameInfo.gameOver == false)
	{
		//only shoot if the recoil has finished
		if (gameInfo.lastShotTime + Set.recoilTime < totalTime)
		{
			enemies.spawnBullet(playerPos, playerPos.directionTo(lookAt));
			gameInfo.lastShotTime = totalTime;
		}

	}
	//restart
	if (keyStates['R']||keyStates['r'])
	{
		gameInfo.gameOver = false;
		gameInfo.lives = 3;
		gameInfo.score = 0;
		gameInfo.currentAmmo = 5;

	}

	// Special Keys
	if (specialKeys[GLUT_KEY_LEFT] && thetaY > 7*3.14/6) {
		thetaY -= 0.01;
		lookAt.x = 300 * cos(thetaY);
		lookAt.z = 300 * sin(thetaY);
		speedUp();
	}
	else if (specialKeys[GLUT_KEY_RIGHT] && thetaY <11*3.14/6) {
		thetaY += 0.01;
		lookAt.x = 300*cos(thetaY);
		lookAt.z = 300*sin(thetaY);
		speedUp();
		
	}
	if (specialKeys[GLUT_KEY_UP] && lookAt.y < 400/*&& thetaP > 4*3.14/6*/) {
		lookAt.y += 3;
		speedUp();
		//lookAt.x += cameraUp.x;						attempts at real pitch
		/*thetaP -= 0.01;
		lookAt.y = 300*sin(thetaP);
		lookAt.z = 300*cos(thetaP);*/ 
		//cameraUp.y = 300 * sin(thetaP - 3.14 / 2);
		//cameraUp.z = 300 * cos(thetaP - 3.14 / 2);
		
	}
	else if (specialKeys[GLUT_KEY_DOWN] && lookAt.y > -400 /*(thetaP-between) < 8*3.14/6*/) {
			lookAt.y -= 3;
			speedUp();
			//lookAt.x -= cameraUp.x;						attempts at real pitch
			/*thetaP += 0.01;
			float thetaU = thetaP - (3.14 / 2);
			lookAt.y = 300 * sin(thetaP-between);
			lookAt.z = 300 * cos(thetaP-between);
			cameraUp.y = 300 * sin(thetaU - between);
			cameraUp.z = 300 * cos(thetaU - between);*/
		
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

	//game over
	if (gameInfo.lives < 0){
		gameInfo.gameOver = true;
	}
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
	hud.setleft();
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
	hud.setright();
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
	glutInitWindowPosition(400, 100);
	glutCreateWindow("Turret Gunner");

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