/* CS 3GC3 - Texturing sample
 * by R. Teather
 */

#include "stdafx.h"
#include "TextureLoader.h"

float cols[6][3] = { {0.5,0.5,0.5}, {0.5,0.5,0.5}, {0.5,0.5,0.5}, {0.5,0.5,0.5}, {0.5,0.5,0.5}, {0.5,0.5,0.5} };

//an array for iamge data
GLubyte* hull_tex;
GLubyte* window_tex;
int width, height, max;

GLuint textures[2];

/* drawPolygon - takes 4 indices and an array of vertices
 *   and draws a polygon using the vertices indexed by the indices
 */
void drawPolygon(int a, int b, int c, int d, float v[8][3]){
	
	glBegin(GL_POLYGON);

		glTexCoord2f(0,0);
		glVertex3fv(v[a]);

		glTexCoord2f(1,0);
		glVertex3fv(v[b]);

		glTexCoord2f(1,1);
		glVertex3fv(v[c]);

		glTexCoord2f(0, 1);
		glVertex3fv(v[d]);
	glEnd();
}

/* cube - takes an array of 8 vertices, and draws 6 faces
 *  with drawPolygon, making up a box
 */
void cube(float v[8][3], bool windowTexture)
{
	if (windowTexture) {
		glBindTexture(GL_TEXTURE_2D, textures[1]);
	} else {
		glBindTexture(GL_TEXTURE_2D,textures[0]);
	}
	glColor3fv(cols[1]); 
	glNormal3f(0, 0, 1);
	drawPolygon(0, 3, 2, 1, v);

	glBindTexture(GL_TEXTURE_2D, textures[0]);
	glColor3fv(cols[2]); 
	glNormal3f(-1, 0, 0);
	drawPolygon(5, 4, 0, 1, v); //1 0 4 5
	
	glBindTexture(GL_TEXTURE_2D, textures[0]);
	glColor3fv(cols[3]); 
	glNormal3f(0, 1, 0);
	drawPolygon(5, 1, 2, 6, v);
	
	glBindTexture(GL_TEXTURE_2D, textures[0]);
	glColor3fv(cols[4]); 
	glNormal3f(1, 0, 0);
	drawPolygon(2, 3, 7, 6, v);
	
	glBindTexture(GL_TEXTURE_2D, textures[0]);
	glColor3fv(cols[5]); 
	glNormal3f(0, 0, -1);
	drawPolygon(6, 5, 4, 7, v);

	glBindTexture(GL_TEXTURE_2D, textures[0]);
	glColor3fv(cols[0]); 
	glNormal3f(0, -1, 0);
	drawPolygon(7, 3, 0, 4, v); 
}

/* drawBox - takes centre point, width, height and depth of a box,
 *  calculates its corner vertices, and draws it with the cube function
 */
void drawBox(float* c, float w, float h, float d, bool window)
{
	float vertices[8][3] = { {c[0]-w/2, c[1]-h/2, c[2]+d/2},
							 {c[0]-w/2, c[1]+h/2, c[2]+d/2},
							 {c[0]+w/2, c[1]+h/2, c[2]+d/2},
							 {c[0]+w/2, c[1]-h/2, c[2]+d/2}, 
							 {c[0]-w/2, c[1]-h/2, c[2]-d/2}, 
							 {c[0]-w/2, c[1]+h/2, c[2]-d/2}, 
							 {c[0]+w/2, c[1]+h/2, c[2]-d/2},
							 {c[0]+w/2, c[1]-h/2, c[2]-d/2} };

	cube(vertices, window);
}

void enemyModel(float* centre) {
	//draw fuselage
	float fuselage_size = 0.5;
	drawBox(centre,fuselage_size,fuselage_size,fuselage_size, true);
	
	//draw left wing connector
	centre[0] += fuselage_size / 2 + fuselage_size / 4;
	drawBox(centre,fuselage_size / 2, fuselage_size / 2, fuselage_size / 4, false);
	
	//draw left wing
	centre[0] += fuselage_size / 2;
	drawBox(centre,fuselage_size / 2, fuselage_size * 1.25, fuselage_size, false);
	centre[0] -= fuselage_size + fuselage_size / 4;

	//draw right wing connector
	centre[0] -= fuselage_size / 2 + fuselage_size / 4;
	drawBox(centre,fuselage_size / 2, fuselage_size / 2, fuselage_size / 4, false);
	
	//draw right wing
	centre[0] -= fuselage_size / 2;
	drawBox(centre,fuselage_size / 2, fuselage_size * 1.25, fuselage_size, false);
}




/* LoadPPM -- loads the specified ppm file, and returns the image data as a GLubyte 
 *  (unsigned byte) array. Also returns the width and height of the image, and the
 *  maximum colour value by way of arguments
 */
GLubyte* LoadPPM(char* file, int* width, int* height, int* max)
{
	GLubyte* img;
	FILE *fd;
	int n, m;
	int  k, nm;
	char c;
	int i;
	char b[100];
	float s;
	int red, green, blue;
	
	fd = fopen(file, "r");
	fscanf(fd,"%[^\n] ",b);
	if(b[0]!='P'|| b[1] != '3')
	{ 
		exit(0);
	}
	fscanf(fd, "%c",&c);
	while(c == '#') 
	{
		fscanf(fd, "%[^\n] ", b);
		printf("%s\n",b);
		fscanf(fd, "%c",&c);
	}
	ungetc(c,fd); 
	fscanf(fd, "%d %d %d", &n, &m, &k);

	nm = n*m;

	img = (GLubyte *)malloc(3*sizeof(GLuint)*nm);

	s=255.0/k;

	for(i=0;i<nm;i++) 
	{
		fscanf(fd,"%d %d %d",&red, &green, &blue );
		img[3*nm-3*i-3]=red*s;
		img[3*nm-3*i-2]=green*s;
		img[3*nm-3*i-1]=blue*s;
	}

	*width = n;
	*height = m;
	*max = k;

	return img;
}

void loadTextures(void)
{	
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



