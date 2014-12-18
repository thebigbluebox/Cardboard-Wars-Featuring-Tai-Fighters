/* CS 3GC3 - Texturing sample
 * by R. Teather
 */

#include "stdafx.h"
#include "TextureLoader.h"

GLuint textures[2];
GLubyte* hull_tex;
GLubyte* window_tex;

/* Draws a textured box centered at a given position.
*           0 - 1
*         /   / |
*    +y  4 -37  2 -z
*        |   | /
* -x -y  5 - 6  +z +x
 */
void drawBox(Vector3 centre, Vector3 size, GLuint* faceTextures, Vector3 color)
{
	float vertices[8][3] = {
		{ centre.x - size.x / 2, centre.y + size.y / 2, centre.z - size.z / 2 }, // back LT
		{ centre.x + size.x / 2, centre.y + size.y / 2, centre.z - size.z / 2 }, // back RT
		{ centre.x + size.x / 2, centre.y - size.y / 2, centre.z - size.z / 2 }, // back RB
		{ centre.x - size.x / 2, centre.y - size.y / 2, centre.z - size.z / 2 }, // back LB
		{ centre.x - size.x / 2, centre.y + size.y / 2, centre.z + size.z / 2 }, // front LT
		{ centre.x - size.x / 2, centre.y - size.y / 2, centre.z + size.z / 2 }, // front LB
		{ centre.x + size.x / 2, centre.y - size.y / 2, centre.z + size.z / 2 }, // front RB
		{ centre.x + size.x / 2, centre.y + size.y / 2, centre.z + size.z / 2 }  // front RT
	};

	int faces[6][4] = {
		{ 1, 2, 3, 0 }, // back
		{ 0, 3, 5, 4 }, // left
		{ 2, 6, 5, 3 }, // bottom
		{ 1, 7, 6, 2 }, // right
		{ 0, 4, 7, 1 }, // top
		{ 4, 5, 6, 7 }  // front
	};

	float normals[6][3] = {
		{ 0, 0, -1 },
		{ -1, 0, 0 },
		{ 0, -1, 0 },
		{ 1, 0, 0 },
		{ 0, 1, 0 },
		{ 0, 0, 1 }
	};

	/*float faceColors[6][3] = { 
		{ 0.5f, 0.5f, 0.5f }, 
		{ 0.5f, 0.5f, 0.5f }, 
		{ 0.5f, 0.5f, 0.5f }, 
		{ 0.5f, 0.5f, 0.5f }, 
		{ 0.5f, 0.5f, 0.5f }, 
		{ 0.5f, 0.5f, 0.5f } 
	};*/

	float texCoord[4][2] = { 
		{ 0, 0 }, { 1, 0 }, 
		{ 1, 1 }, { 0, 1 } 
	};

	for (int i = 0; i < 6; i++) {
		glBindTexture(GL_TEXTURE_2D, faceTextures[i]);
		//glColor3fv(faceColors[i]);
		glColor3fv(color.v); // single color for the whole box
		glBegin(GL_POLYGON);
		for (int j = 0; j < 4; j++) {
			glTexCoord2fv(texCoord[j]);
			glNormal3fv(normals[i]);
			glVertex3fv(vertices[faces[i][j]]);
	}
		glEnd();
}
}

void enemyModel(Vector3 centre, Vector3 color) {
	GLuint cubeFaceTextures[6] = { textures[0], textures[0], textures[0], textures[0], textures[0], textures[1] };

	//draw fuselage
	float fuselage_size = 0.5;
	drawBox(centre, { fuselage_size, fuselage_size, fuselage_size }, cubeFaceTextures, color);
	
	cubeFaceTextures[5] = textures[0];

	//draw left wing connector
	centre.x += fuselage_size / 2 + fuselage_size / 4;
	drawBox(centre, { fuselage_size / 2, fuselage_size / 2, fuselage_size / 4 }, cubeFaceTextures, color);
	
	//draw left wing
	centre.x += fuselage_size / 2;
	drawBox(centre, { fuselage_size / 2, fuselage_size * 1.25f, fuselage_size }, cubeFaceTextures, color);
	centre.x -= fuselage_size + fuselage_size / 4;

	//draw right wing connector
	centre.x -= fuselage_size / 2 + fuselage_size / 4;
	drawBox(centre, { fuselage_size / 2, fuselage_size / 2, fuselage_size / 4 }, cubeFaceTextures, color);
	
	//draw right wing
	centre.x -= fuselage_size / 2;
	drawBox(centre, { fuselage_size / 2.0f, fuselage_size * 1.25f, fuselage_size }, cubeFaceTextures, color);
}


/* LoadPPM -- loads the specified ppm file, and returns the image data as a GLubyte 
 *  (unsigned byte) array. Also returns the width and height of the image, and the
 *  maximum colour value by way of arguments
 */
GLubyte* LoadPPM(char* file, int* width, int* height, int* max)
{
	GLubyte* img;
	FILE *fd;
	int size; // texture size
	char c;
	char b[100]; // holds the first line
	float scale; // color scale
	int red, green, blue;
	int w, h, m; // width, height, max

	// Open file
	fd = fopen(file, "r");
	fscanf(fd,"%[^\n] ",b); // Read in the first line
	if(b[0]!='P'|| b[1] != '3') exit(1); // Error if != P3

	// Remove comments
	fscanf(fd, "%c",&c);
	printf("%c\n", c);
	while(c == '#') 
	{
		fscanf(fd, "%[^\n] ", b);
		printf("%s\n",b);
		fscanf(fd, "%c",&c);
	}
	ungetc(c,fd); 

	// Get texture info
	fscanf(fd, "%d %d %d", &w, &h, &m);
	size = w*h;
	scale=255.0/m;
	img = (GLubyte*)malloc(3*sizeof(GLuint)*size);

	// Store colors
	for(int i = 0; i < size; i++) {
		fscanf(fd,"%d %d %d",&red, &green, &blue );
		img[3*size-3*i-3]=red*scale;
		img[3*size-3*i-2]=green*scale;
		img[3*size-3*i-1]=blue*scale;
	}

	// Return
	*width = w;
	*height = h;
	*max = m;
	return img;
}

void loadTextures(void)
{	
	int width, height, max;
	//generate 2 texture IDs, store them in array "textures"
	glGenTextures(2, textures);

	//load the texture
	hull_tex = LoadPPM("metal.ppm", &width, &height, &max);
	//setup first texture
	glBindTexture(GL_TEXTURE_2D, textures[0]);
	//set texture parameters
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//create a texture using the "hull_tex" array data
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, hull_tex);
	
	//load the texture
	window_tex = LoadPPM("front.ppm", &width, &height, &max);
	//setup second texture
	glBindTexture(GL_TEXTURE_2D, textures[1]);
	//set texture parameters
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//create a texture using the "tex" array
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, window_tex);	
}