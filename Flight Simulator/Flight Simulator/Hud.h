#ifndef __HUD_H__
#define __HUD_H__
#include "stdafx.h"

class Hud {
private:
	void drawSentence(const char* line, float startX, float startY, Vector3 color, void* font); //prints a sentence onto the screen
	int height;
	int width;
	GLubyte bitmapColorArray[2]; //the true false value array for the bitmaps
	GLubyte ammoSprite[512]; //the ammo bitmap
public:
	Hud(void);
	Hud(int width, int height);
	
	/*Sets the GUI's height and width so it can be resized*/
	void setWindowSize(int width, int height);
	
	/*The drawing of the HUD*/
	void draw(void);

	/*Sets parameter for right or left eye*/
	void setright(void);
	void setleft(void);
};

#endif