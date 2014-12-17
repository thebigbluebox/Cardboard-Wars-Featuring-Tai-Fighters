#include "HUD.h"
#include "main.h"
#include "stdafx.h"

Hud::Hud()
{
	width = 1920/2;
	height = 1080/2;
}

Hud::Hud(int width, int height)
{
	width = width;
	height = height;
}

void Hud::setWindowSize(int width, int height)
{
	width = width;
	height = height;
}

void Hud::drawSentence(const char* line, float startX, float startY, Vector3 color, void* font)
{
	glRasterPos2f(startX, startY);
	glColor3fv(color.v);
	for (size_t i = 0; i < std::strlen(line); ++i) {
		glutBitmapCharacter(font, line[i]); //GLUT_BITMAP_TIMES_ROMAN_24
	}
}

void Hud::drawScoreText() 
{
	GameInfo gameInfo = getGameInfo();
	
	//glMatrixMode(GL_MODELVIEW);
	//glLoadIdentity();
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluOrtho2D(0, width, height, 0); // 0,0 is top left

	std::string text[2];
	text[0] = "Score: " + std::to_string(gameInfo.score);
	text[1] = "Lives: " + std::to_string(gameInfo.lives);

	for (int i = 0, Y = 20; i < 2; i++, Y += 20) {
		drawSentence(text[i].c_str(), 10, Y, { 1, 1, 1 }, GLUT_BITMAP_TIMES_ROMAN_24);
	}
	
	//glRasterPos2i(20, Set.windowy - 30);  // or wherever in window coordinates
	//if ('1' != NULL)
	//{
	//	for (char* p = Player.scoreAsString; *p; p++)
	//		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *p);
	//}

}

void Hud::drawCrosshairs(void)
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-width / 2, width / 2, -height / 2, height / 2);

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
	glMatrixMode(GL_PROJECTION);
}

void Hud::draw(void)
{
	drawCrosshairs();
	drawScoreText();
}