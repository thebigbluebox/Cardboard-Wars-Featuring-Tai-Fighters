#ifndef __HUD_H__
#define __HUD_H__
#include "stdafx.h"

class Hud {
private:
	void drawSentence(const char* line, float startX, float startY, Vector3 color, void* font);
	void drawCrosshairs(void);
	void drawScoreText(void);
	int height;
	int width;
public:
	/*Creates the GUI object,
	should be configurable with additional parameters*/
	Hud(void);

	Hud(int width, int height);
	
	/*Sets the GUI's height and width so it can be resized*/
	void setWindowSize(int width, int height);
	
	/*The drawing of the GUI*/
	void draw(void);
};

#endif