 /* CS 3GC3 - Texturing sample
 * by R. Teather
 */

#include "stdafx.h"
#include "TextureLoader.h"

GLuint textures[2];
GLubyte* hull_tex;
GLubyte* window_tex;
GLubyte* sky1;
GLubyte* sky2;
GLubyte* sky3;
GLubyte* sky4;
GLubyte* sky5;
GLubyte* sky6;



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

void enemyModel(Vector3 centre, Vector3 color)
{
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


void skyBox(Vector3 centre, GLint distance)
{
	GLuint faceTextures[6] = { textures[2], textures[3], textures[4], textures[5], textures[6], textures[7] };
	Vector3 size (distance,distance,distance);
	Vector3 color (1,1,1); //background color
	
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
			{ 0, 3, 2, 1 }, // back
			{ 4, 5, 3, 0 }, // left
			{ 3, 5, 6, 2 }, // bottom
			{ 2, 6, 7, 1 }, // right
			{ 1, 7, 4, 0 }, // top
			{ 7, 6, 5, 4 }  // front
	};

	float normals[6][3] = {
			{ 0, 0, 1 },
			{ 1, 0, 0 },
			{ 0, 1, 0 },
			{ -1, 0, 0 },
			{ 0, -1, 0 },
			{ 0, 0, -1 }
	};

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
	glGenTextures(8, textures);

	//load the texture
	hull_tex = LoadPPM("metal.ppm", &width, &height, &max);
	//setup first texture
	glBindTexture(GL_TEXTURE_2D, textures[0]);
	//set texture parameters
	setParam();
	//create a texture using the "hull_tex" array data
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, hull_tex);
	
	window_tex = LoadPPM("front.ppm", &width, &height, &max);
	glBindTexture(GL_TEXTURE_2D, textures[1]);
	setParam();
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, window_tex);

	sky1 = LoadPPM("sky1.ppm", &width, &height, &max);
	glBindTexture(GL_TEXTURE_2D, textures[2]);
	setParam();
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, sky1);
	
	sky2 = LoadPPM("sky2.ppm", &width, &height, &max);
	glBindTexture(GL_TEXTURE_2D, textures[3]);
	setParam();
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, sky2);
	
	sky3 = LoadPPM("sky3.ppm", &width, &height, &max);
	glBindTexture(GL_TEXTURE_2D, textures[4]);
	setParam();
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, sky3);
	
	sky4 = LoadPPM("sky4.ppm", &width, &height, &max);
	glBindTexture(GL_TEXTURE_2D, textures[5]);
	setParam();
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, sky4);
	
	sky5 = LoadPPM("sky5.ppm", &width, &height, &max);
	glBindTexture(GL_TEXTURE_2D, textures[6]);
	setParam();
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, sky5);
	
	sky6 = LoadPPM("sky6.ppm", &width, &height, &max);
	glBindTexture(GL_TEXTURE_2D, textures[7]);
	setParam();
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, sky6);
}

void setParam(void)
{
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
}