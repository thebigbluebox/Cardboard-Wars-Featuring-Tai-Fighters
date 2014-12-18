#include "HUD.h"
#include "main.h"
#include "stdafx.h"

bool left = false;

Hud::Hud()
{
	width = 1920/2;
	height = 1080/2;
	bitmapColorArray[0] =  0x00;
	bitmapColorArray[1] = 0xff;
	int i, j;

	for (i = 0; i < 64; i++)
	{
		for (j = 0; j < 8; j++)
		{
			checker[i * 8 + j] = bitmapColorArray[(i / 8 + j) % 2];
		}
	}
}

Hud::Hud(int width, int height)
{
	width = width;
	height = height;
	bitmapColorArray[0] = 0x00;
	bitmapColorArray[1] = 0xff;
}

void Hud::setWindowSize(int width, int height)
{
	width = width;
	height = height;
}

void Hud::setleft(void)
{
	left = true;
}

void Hud::setright(void)
{
	left = false;
}

void Hud::drawSentence(const char* line, float startX, float startY, Vector3 color, void* font)
{
	glRasterPos2f(startX, startY);
	glColor3fv(color.v);
	for (size_t i = 0; i < std::strlen(line); ++i) {
		glutBitmapCharacter(font, line[i]); //GLUT_BITMAP_TIMES_ROMAN_24
	}
}

//deprectaed?
void Hud::drawScoreText() 
{
	GameInfo gameInfo = getGameInfo();
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	if (left){
		gluOrtho2D(-width*0.05, width + width*0.03, height, 0);
	}
	else{
		gluOrtho2D(-width*0.1, width - width*0.03, height, 0);
	}
	//gluOrtho2D(0, width, height, 0); // 0,0 is top left

	std::string text[2];
	text[0] = "Score: " + std::to_string(gameInfo.score);
	text[1] = "Lives: " + std::to_string(gameInfo.lives);

	for (int i = 0, Y = 20; i < 2; i++, Y += 20) {
		drawSentence(text[i].c_str(), 10, Y, { 1, 1, 1 }, GLUT_BITMAP_TIMES_ROMAN_24);
	}
}

void Hud::drawCrosshairs(void)
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if (left){
		gluOrtho2D((-width / 2) - width*0.01, (width / 2) - width*0.01, -height / 2, height / 2);
	}
	else{
		gluOrtho2D((-width / 2) + width*0.01, (width / 2) + width*0.01, -height / 2, height / 2);
	}

	glPointSize(5);
	glColor3d(0, 1, 1);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	
	// Outer square
	glBegin(GL_POLYGON);
		glVertex2f(-width / 2 + width*0.3f, -height / 2 + height*0.3f);
		glVertex2f(width / 2 - width*0.3f, -height / 2 + height*0.3f);
		glVertex2f(width / 2 - width*0.3f, height / 2 - height*0.3f);
		glVertex2f(-width / 2 + width*0.3f, height / 2 - height*0.3f);
	glEnd();
	
	// Inner square
	glBegin(GL_POLYGON);
		glVertex2f(-width / 20.0f, -height / 20.0f);
		glVertex2f(width / 20.0f, -height / 20.0f);
		glVertex2f(width / 20.0f, height / 20.0f);
		glVertex2f(-width / 20.0f, height / 20.0f);
	glEnd();
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	GameInfo gameInfo = getGameInfo();
	
	glRasterPos2i(-width / 2 + width*0.3f, -height / 2 + height*0.3f);
	for (int i = 0; i < gameInfo.currentAmmo; i++)
		glBitmap(32, 32, 0, 0, 32, 0, checker);
	if (gameInfo.currentAmmo == 0){
		glColor3d(1, 0, 0);
		drawSentence("Reload", -width / 2 + width*0.3f, -height / 2 + height*0.3f, { 1, 1, 1 }, GLUT_BITMAP_HELVETICA_18);
	}
	std::string text[2];
	text[0] = "Score: " + std::to_string(gameInfo.score);
	text[1] = "Lives: " + std::to_string(gameInfo.lives);

	for (int i = 0, Y = height / 2 - height*0.3f; i < 2; i++, Y += 20) {
		drawSentence(text[i].c_str(), -width / 2 + width*0.3f, Y, { 1, 1, 1 }, GLUT_BITMAP_HELVETICA_18);
	}
	glMatrixMode(GL_PROJECTION);
}

//decrepcated?
void Hud::drawAmmo(void)
{
	GameInfo gameInfo = getGameInfo();
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	if (left){
		gluOrtho2D(-width*0.05, width + width*0.03, height, 0);
	}
	else{
		gluOrtho2D(-width*0.1, width - width*0.03, height, 0);
	}	
	glRasterPos2i(32, height - 16);
	for (int i = 0; i < gameInfo.currentAmmo; i++)
		glBitmap(64, 64, 0, 0, 64, 0, checker);
}

void Hud::draw(void)
{
	drawCrosshairs();
	//drawScoreText();
	//drawAmmo();
}