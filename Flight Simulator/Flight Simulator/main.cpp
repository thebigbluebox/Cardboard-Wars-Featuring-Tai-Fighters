#include "stdafx.h"
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
	int mode = 1;
} Set;

int totalTime = 0;
bool keyStates[256] = { false }; // keyboard state
bool specialKeys[256] = { false };

Hud hud;
EnemyHandler enemies = EnemyHandler();
GameInfo gameInfo = { 0,		// score
					  3,		// lives
					  0,		// lastShotTime
					  5,		// ammo
					  5,		// currentAmmo
					  false,	// gameOver
					  false,	// leftCannon
					  0,		// anglex
					  0,		// angley
					  0			// level
					};

GameInfo getGameInfo(void){	return gameInfo;}

double thetaR = 3.14159 / 2.0f; //rolling angle
double thetaY = 3 * 3.14159 / 2.0f; //yaw angle
int rollDirection = 1;
bool isRolling = false;
float playerSpeed = 1.0f/40.0f;

Vector3 playerPos = { 0, 0, 0 }; // aka cam pos // the camera looks in the negative z direction
Vector3 lookAt = { 300 * cosf(thetaY), 0, 300 * sinf(thetaY) };
Vector3 cameraUp = { 300 * cosf(thetaR), 300 * sinf(thetaR), 0 };
Vector3 mover;
GLfloat lightposition[] = { 0.0f, 0.0f, 5.0f, 1.0f };

void updateGameInfo(void)
{
	//gameInfo.score = totalTime/100;
	//gameInfo.lives = 3;
}

void updateLighting(void)
{
	glLightfv(GL_LIGHT0, GL_POSITION, lightposition);
}

/*Updates the players movement and direction*/
void updatePlayer(int deltaTime)
{	
	lookAt.z -= 0.01;
	mover = playerPos.directionTo(lookAt);
	playerPos = playerPos.add(mover.scale(playerSpeed));
	lookAt = lookAt.add(mover.scale(playerSpeed));

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
	//insreases speed
	if (keyStates['w'] || keyStates['W']) {
		playerSpeed += (playerSpeed < 0.1f) ? 0.002f : 0;
	}
	//decreases speed
	else if (keyStates['s'] || keyStates['S']) {
		playerSpeed += (playerSpeed > 0.02f) ? -0.002f : 0;
	}
	//rolls counter clockwise
	if ((keyStates['a'] || keyStates['A'])) {
		if (!isRolling) rollDirection = -1;
		isRolling = true;	
	}
	//rolls clockwise
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
	//yaw left
	if (specialKeys[GLUT_KEY_LEFT] && thetaY > 13*3.14/12) {
		thetaY -= 0.01;
		lookAt.x = 300 * cos(thetaY);
		lookAt.z = 300 * sin(thetaY);
		speedUp();
	}
	//yaw right
	else if (specialKeys[GLUT_KEY_RIGHT] && thetaY <23*3.14/12) {
		thetaY += 0.01;
		lookAt.x = 300*cos(thetaY);
		lookAt.z = 300*sin(thetaY);
		speedUp();
		
	}
	//pitch up
	if (specialKeys[GLUT_KEY_UP] && lookAt.y < 400) {
		lookAt.y += 3;
		speedUp();
	}
	//pitch down
	else if (specialKeys[GLUT_KEY_DOWN] && lookAt.y > -400) {
			lookAt.y -= 3;
			speedUp();	
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
	//update level
	if (gameInfo.score == 10)
		gameInfo.level = 1;
	if (gameInfo.score == 20)
		gameInfo.level = 2;
	if (gameInfo.score == 30)
		gameInfo.level = 3;
	if (gameInfo.score == 40)
		gameInfo.level = 4;
	// Other update routines
	updateKeyboard();
	updateEnemies(deltaTime);
	updatePlayer(deltaTime);
	updateLighting();

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
	case '1':
		if (Set.mode == 1)
		{
			Set.mode = 2;
		}
		else
		{
			Set.mode = 1;
		}
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
	float pureWhite[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	float light0ambient[4] = { 0.804f, 1.f, 0.98f, 1.f };
	float light0diffuse[4] = { 0.804f, 1.f, 0.98f, 1.f };
	glLightfv(GL_LIGHT0, GL_AMBIENT, light0ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, pureWhite);
	glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 60.0f);
	glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, 100.0f);

	// Meterials
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE); // use glColor for material
	glMaterialfv(GL_FRONT, GL_SPECULAR, pureWhite);
	glMateriali(GL_FRONT, GL_SHININESS, 8);

	// Textures
	glEnable(GL_TEXTURE_2D);
	loadTextures();
}


void drawSkybox(void)
{
	glPushMatrix();
	glTranslatef(0, 0, -10);
	skyBox(playerPos,50);
	glPopMatrix();
}

void draw(void)
{
	enemies.drawEnemies();
	enemies.updateBullets();
	//hud.draw(); // moved to display
	drawSkybox();
}

	

/* display function - GLUT display callback function
*		clears the screen, sets the camera position, draws the ground plane and movable box
*/
void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glMatrixMode(GL_MODELVIEW);
	//glLoadIdentity();

	if (Set.mode == 1)
	{
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
	}
	if (Set.mode == 2)
	{
		glViewport(0, 0, Set.windowx, Set.windowy);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(65, Set.windowx / Set.windowy, 1, 200);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glPushMatrix(); // push!
		gluLookAt(playerPos.x, playerPos.y, playerPos.z, lookAt.x, lookAt.y, lookAt.z, cameraUp.x, cameraUp.y, cameraUp.z);
		draw();
		glPopMatrix(); // pop!
		hud.setright();
		hud.draw();
	}
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