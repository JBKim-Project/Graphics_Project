/*
 * Skeleton code for CSE471 Fall 2019
 *
 * Won-Ki Jeong, wkjeong@unist.ac.kr
 */

#include <stdio.h>
#include <GL/glew.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <iostream>
#include <assert.h>
#include "math.h"

#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <GL/glew.h>



 //
 // Definitions
 //
float eyePosition[3] = { 0,0,1 };
float up[3] = { 0,1,0 };
float scale = 1;

GLfloat Ipos[4] = { 0,0,10,0 };
GLfloat diffuse[] = { 1,1,1,0 };
GLfloat specular[] = { 1,1,1,0 };
GLfloat ambient[] = { 1,1,1,0 };

int time = 0;
int check = 0;

int p = 1;

int mouse_prev_x = 0, mouse_prev_y = 0;
int mouse_dx = 0, mouse_dy = 0;

bool left_button_pressed = false;
bool middle_button_pressed = false;

float anglex = 0, angley = 0, anglez = 0;
float zoom = 1.35;
float positionx = 0, positiony = 0, positionz = 5;

GLfloat width, height;

float* vertex = NULL;
int* face = NULL;
float* normal = NULL;
float* vertexnormal = NULL;
int num_vertex, num_face, zero;



typedef struct {
	unsigned char x, y, z, w;
} uchar4;
typedef unsigned char uchar;

// BMP loader
void LoadBMPFile(uchar4** dst, int* width, int* height, const char* name);

#define screenSize 1024
#define	imageSize 512

//
// Variables
//

// array to store synthetic cubemap raw image
static GLubyte image1[4][4][4];
static GLubyte image2[4][4][4];
static GLubyte image3[4][4][4];
static GLubyte image4[4][4][4];
static GLubyte image5[4][4][4];
static GLubyte image6[4][4][4];

// texture object handles, FBO handles
GLuint cube_tex, color_tex;
GLuint fb, depth_rb;

//
// Functions
//
void get_vertex_face(void)
{
	FILE* MeshFile = NULL;
	MeshFile = fopen("../bunny.off", "r");

	if (MeshFile == NULL) {
		printf("File Open Error\n");
		return;
	}
	char off[4];



	fscanf(MeshFile, "%s\n", off);
	if (strcmp(off, "OFF") != 0) {
		printf("The first line of this file is not string OFF");
		return;
	}
	fscanf(MeshFile, "%d %d %d", &num_vertex, &num_face, &zero);
	printf("%d, %d\n", num_vertex, num_face);

	vertex = new float[num_vertex * 3];
	face = new int[num_face * 3];
	normal = new float[num_face * 3];
	vertexnormal = new float[num_vertex * 3];

	for (int i = 0; i < num_vertex * 3; i++)
	{
		fscanf(MeshFile, "%f", &vertex[i]);

	}

	int three = 0;
	for (int i = 0; i < num_face * 3; i++)
	{
		if (i % 3 == 0)
			fscanf(MeshFile, "%d", &three);
		fscanf(MeshFile, "%d", &face[i]);
	}

	fclose(MeshFile);
}

void calculate_normal_vertex(void)
{

	for (int i = 0; i < num_face * 3; i++)
	{
		if (i % 3 == 0) {

			float x1, x2, y1, y2, z1, z2, t;

			x1 = vertex[3 * face[i + 1]] - vertex[3 * face[i]];
			x2 = vertex[3 * face[i + 2]] - vertex[3 * face[i]];
			y1 = vertex[3 * face[i + 1] + 1] - vertex[3 * face[i] + 1];
			y2 = vertex[3 * face[i + 2] + 1] - vertex[3 * face[i] + 1];
			z1 = vertex[3 * face[i + 1] + 2] - vertex[3 * face[i] + 2];
			z2 = vertex[3 * face[i + 2] + 2] - vertex[3 * face[i] + 2];

			normal[i] = (y1 * z2 - z1 * y2);
			normal[i + 1] = (x1 * z2 - z1 * x2) * (-1);
			normal[i + 2] = (x1 * y2 - y1 * x2);

			t = sqrt(normal[i] * normal[i] + normal[i + 1] * normal[i + 1] + normal[i + 2] * normal[i + 2]);
			normal[i] = normal[i] / t;
			normal[i + 1] = normal[i + 1] / t;
			normal[i + 2] = normal[i + 2] / t;

		}

	}

	int k = 0;
	float sum[3] = { 0,0,0 };

	for (int i = 0; i < num_vertex; i++)
	{
		sum[0] = 0;
		sum[1] = 0;
		sum[2] = 0;
		k = 0;
		for (int j = 0; j < num_face * 3; j++)
		{
			if (j % 3 == 0)
			{
				if (face[j] == i || face[j + 1] == i || face[j + 2] == i)
				{
					sum[0] += normal[j];
					sum[1] += normal[j + 1];
					sum[2] += normal[j + 2];
					k++;
				}
			}
		}
		vertexnormal[i * 3] = sum[0] / k;
		vertexnormal[i * 3 + 1] = sum[1] / k;
		vertexnormal[i * 3 + 2] = sum[2] / k;
	}
}


void CreateCube(void)
{
	glBegin(GL_QUADS);
	// Remove Front side
	//glTexCoord2d(0.34, 0.25); glVertex3d(-1.0, -1.0, 1.0);
	//glTexCoord2d(0.66, 0.25); glVertex3d(1.0, -1.0, 1.0);
	//glTexCoord2d(0.66, 0.5); glVertex3d(1.0, 1.0, 1.0);
	//glTexCoord2d(0.34, 0.5); glVertex3d(-1.0, 1.0, 1.0);

	glTexCoord2d(0, 0); glVertex3d(-1.0, -1.0, -1.0);
	glTexCoord2d(1, 0); glVertex3d(1.0, -1.0, -1.0);
	glTexCoord2d(1, 1); glVertex3d(1.0, -1.0, 1.0);
	glTexCoord2d(0, 1); glVertex3d(-1.0, -1.0, 1.0);

	glTexCoord2d(0, 0); glVertex3d(-1.0, 1.0, 1.0);
	glTexCoord2d(1, 0); glVertex3d(1.0, 1.0, 1.0);
	glTexCoord2d(1, 1); glVertex3d(1.0, 1.0, -1.0);
	glTexCoord2d(0, 1); glVertex3d(-1.0, 1.0, -1.0);

	glTexCoord2d(0, 0); glVertex3d(-1.0, 1.0, -1.0);
	glTexCoord2d(1, 0); glVertex3d(1.0, 1.0, -1.0);
	glTexCoord2d(1, 1); glVertex3d(1.0, -1.0, -1.0);
	glTexCoord2d(0, 1); glVertex3d(-1.0, -1.0, -1.0);

	glTexCoord2d(0, 0); glVertex3d(-1.0, -1.0, 1.0);
	glTexCoord2d(1, 0); glVertex3d(-1.0, 1.0, 1.0);
	glTexCoord2d(1, 1); glVertex3d(-1.0, 1.0, -1.0);
	glTexCoord2d(0, 1); glVertex3d(-1.0, -1.0, -1.0);

	glTexCoord2d(0, 0); glVertex3d(1.0, 1.0, 1.0);
	glTexCoord2d(1, 0); glVertex3d(1.0, -1.0, 1.0);
	glTexCoord2d(1, 1); glVertex3d(1.0, -1.0, -1.0);
	glTexCoord2d(0, 1); glVertex3d(1.0, 1.0, -1.0);
	glEnd();
}

// create synthetic data for static cubemap
void makeSyntheticImages(void)
{
	int i, j, c;

	for (i = 0; i < 4; i++) {
		for (j = 0; j < 4; j++) {
			c = ((((i & 0x1) == 0) ^ ((j & 0x1)) == 0)) * 255;
			image1[i][j][0] = (GLubyte)c;
			image1[i][j][1] = (GLubyte)c;
			image1[i][j][2] = (GLubyte)c;
			image1[i][j][3] = (GLubyte)255;

			image2[i][j][0] = (GLubyte)c;
			image2[i][j][1] = (GLubyte)c;
			image2[i][j][2] = (GLubyte)0;
			image2[i][j][3] = (GLubyte)255;

			image3[i][j][0] = (GLubyte)c;
			image3[i][j][1] = (GLubyte)0;
			image3[i][j][2] = (GLubyte)c;
			image3[i][j][3] = (GLubyte)255;

			image4[i][j][0] = (GLubyte)0;
			image4[i][j][1] = (GLubyte)c;
			image4[i][j][2] = (GLubyte)c;
			image4[i][j][3] = (GLubyte)255;

			image5[i][j][0] = (GLubyte)255;
			image5[i][j][1] = (GLubyte)c;
			image5[i][j][2] = (GLubyte)c;
			image5[i][j][3] = (GLubyte)255;

			image6[i][j][0] = (GLubyte)c;
			image6[i][j][1] = (GLubyte)c;
			image6[i][j][2] = (GLubyte)255;
			image6[i][j][3] = (GLubyte)255;
		}
	}
}

void init(void)
{
	// You need to ues glew

	glewInit();

	GLfloat diffuse[4] = { 1.0, 1.0, 1.0, 1.0 };

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);

	// make synthetic cubemap data
	makeSyntheticImages();
	

	//
	// Creating a 2D texture from image
	// 
	int width, height;
	uchar4* dst;
	//LoadBMPFile(&dst, &width, &height, "../CLOUD2.bmp"); // this is how to load image
	//LoadBMPFile(&dst, &width, &height, "../Yob.jpg"); // this is how to load image
	LoadBMPFile(&dst, &width, &height, "../brick.bmp"); // this is how to load image
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, &color_tex);
	glBindTexture(GL_TEXTURE_2D, color_tex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, dst);

#define STATIC_CUBEMAP
	//#define DYNAMIC_CUBEMAP //STATIC_CUBEMAP 

		//
		// creating cube map texture (either static or dynamic)
		//

#ifdef STATIC_CUBEMAP
	// create static cubemap from synthetic data
	glGenTextures(1, &cube_tex);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cube_tex);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);


	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGBA, 4, 4, 0, GL_RGBA, GL_UNSIGNED_BYTE, image1);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGBA, 4, 4, 0, GL_RGBA, GL_UNSIGNED_BYTE, image2);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGBA, 4, 4, 0, GL_RGBA, GL_UNSIGNED_BYTE, image3);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGBA, 4, 4, 0, GL_RGBA, GL_UNSIGNED_BYTE, image4);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGBA, 4, 4, 0, GL_RGBA, GL_UNSIGNED_BYTE, image5);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGBA, 4, 4, 0, GL_RGBA, GL_UNSIGNED_BYTE, image6);

	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP);
	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP);
	glTexGeni(GL_R, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP);

	// ToDo...

#endif


// generate cubemap on-the-fly

}

void idle()
{
	// do something for animation here b/c this will be called when idling

	glutPostRedisplay();
}

// dynamically update cubemap
void update_cubemap()
{
	// bind FBO to render to texture
	glBindFramebuffer(GL_FRAMEBUFFER, fb);

	// render to +x face
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X, cube_tex, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();
	gluLookAt(0, 0, 0, 1, 0, 0, 0, 1, 0);
	CreateCube();
	// render the entire scene here...

	// render to -x face
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_NEGATIVE_X, cube_tex, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();
	gluLookAt(0, 0, 0, -1, 0, 0, 0, 1, 0);
	CreateCube();

	// render the entire scene here...

	// render to +y face
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_Y, cube_tex, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();
	gluLookAt(0, 0, 0, 0, 1, 0, 0, 1, 0);
	CreateCube();
	// render the entire scene here...

	// render to -y face
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, cube_tex, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();
	gluLookAt(0, 0, 0, 0, -1, 0, 0, 1, 0);
	CreateCube();
	// render the entire scene here...

	// render to +z face
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_Z, cube_tex, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();
	gluLookAt(0, 0, 0, 0, 0, 1, 0, 1, 0);
	CreateCube();
	// render the entire scene here...

	// render to -z face
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, cube_tex, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();
	gluLookAt(0, 0, 0, 0, 0, -1, 0, 1, 0);
	CreateCube();
	// render the entire scene here...

	// unbind FBO
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
// text added
void text(double x, double y)
{
	char text[32];

	sprintf(text, "Zoom: %.1f", zoom);

	glColor3f(1.0f, 1.0f, 1.0f);

	glRasterPos2f(x, y);
	for (int i = 0; text[i] != '\0'; i++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, text[i]);
}
void display(void)
{
	// update dynamic cubemap per frame
#ifdef DYNAMIC_CUBEMAP
	update_cubemap();
#endif
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	glLightfv(GL_LIGHT0, GL_POSITION, Ipos);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);


	// render something here...
	if (p == 1) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		gluLookAt(eyePosition[0], eyePosition[1], eyePosition[2],
			0.0, 0.0, 0.0,
			0.0f, 1.0f, 0.0f);
		glEnable(GL_TEXTURE_2D);
		glPushMatrix();
		glTranslatef(0, 0, 5);
		//glScalef(zoom, zoom, zoom);
		//glRotatef(anglex, 1.0f, 0.0f, 0.0f);
		//glRotatef(angley, 0.0f, 1.0f, 0.0f);

		CreateCube();
		glPopMatrix();

		//glPushMatrix();

		//glTranslatef(0, time, time + 7);
		//glutSolidSphere(0.2f, 100, 100);
		////glTranslatef(positionx, positiony, 7);
		//glPopMatrix();


		glLoadIdentity();
		gluLookAt(0, 0.0, 3.0,
			0.0, 0.0, 0.0,
			0.0f, 1.0f, 0.0f);
		glPushMatrix();
		text(-0.6, -0.6); // text added
		glTranslatef(positionx, positiony, 1);
		glScalef(zoom, zoom, zoom); //scale up down 


		GLfloat temp_matrix[16];
		glRotatef(anglex, 1.0f, 0.0f, 0.0f);
		glRotatef(angley, 0.0f, 1.0f, 0.0f);


		glColor3ub(169, 200, 250);
		GLuint vertexbuffer;
		glGenBuffers(1, &vertexbuffer);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glBufferData(GL_ARRAY_BUFFER, num_vertex * 3 * 4 * 2, 0, GL_STATIC_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER, 0, num_vertex * 3 * 4, vertex);
		glBufferSubData(GL_ARRAY_BUFFER, num_vertex * 3 * 4, num_vertex * 3 * 4, vertexnormal);

		GLuint indexbuffer;
		glGenBuffers(1, &indexbuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexbuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, num_face * 3 * 4, face, GL_STATIC_DRAW);


		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexbuffer);

		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_NORMAL_ARRAY);

		glVertexPointer(3, GL_FLOAT, 0, (void*)0);
		glNormalPointer(GL_FLOAT, 0, (void*)(num_vertex * 3 * 4));

		glDrawElements(GL_TRIANGLES, num_face * 3, GL_UNSIGNED_INT, (void*)0);


		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		glDeleteBuffers(1, &vertexbuffer);
		glDeleteBuffers(1, &indexbuffer);

		glPopMatrix();

		glPushMatrix();
		glTranslatef(-0.3, time * 0.01, 1 + time);
		//glColor3f(0, 0, 1);
		//glTranslatef(positionx, positiony, positionz);
		glRotatef(anglex, 1.0f, 0.0f, 0.0f);
		glRotatef(angley, 0.0f, 1.0f, 0.0f);
		glutSolidSphere(0.05f, 100, 100);

		glPopMatrix();


		glDisable(GL_TEXTURE_2D);

		if (time > 1)
			check = 1;
		else if (time < -1)
			check = 0;

		if (check == 0)
			time = time + 0.005;
		else
			time = time - 0.005;

	}


	glFlush();

	glutSwapBuffers();
}

void reshape(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(40.0, (GLfloat)w / (GLfloat)h, 1.0, 300.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0.0, 0.0, -20.0);
	width = w;
	height = h;
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case '1':
		p = 1;
		break;
	case '2':
		p = 2;
		break;
	case '3':
		p = 3;
		break;
	case 'o':
		positionz += 1;
		break;

	case 'b':
		zoom += 0.05;
		//printf("zoom: %f\n", zoom);
		break;
	case 'v':
		zoom -= 0.05;
		//printf("zoom: %f\n", zoom);
		break;
	case 27:
		exit(0);
		break;
	}
	//glutPostRedisplay();
}
//
//void mousemotion(int x, int y)
//{
//	printf("x: %d\n", x);
//	printf("y: %d\n", y);
//
//	mouse_dx = x - mouse_prev_x;
//	mouse_dy = y - mouse_prev_y;
//
//	mouse_prev_x = x;
//	mouse_prev_y = y;
//
//	if (left_button_pressed == true) {
//		anglex += (float)mouse_dy;
//		angley -= (float)mouse_dx;
//	}
//	else if (middle_button_pressed == true) {
//		positionx += mouse_dx * 0.006;
//		positiony -= mouse_dy * 0.006;
//	}
//
//	glutPostRedisplay();
//
//}
//
//void mousebutton(int button, int state, int x, int y)
//{
//	if (button == GLUT_RIGHT_BUTTON)
//	{
//		if (state == GLUT_DOWN)
//			zoom -= 0.1;
//
//	}
//	else if (button == GLUT_MIDDLE_BUTTON)
//	{
//		if (state == GLUT_DOWN)
//		{
//			if (middle_button_pressed == false) {
//				middle_button_pressed = true;
//				mouse_prev_x = x;
//				mouse_prev_y = y;
//			}
//		}
//
//		else if (state == GLUT_UP)
//		{
//			middle_button_pressed = false;
//			mouse_dx = 0;
//			mouse_dy = 0;
//		}
//	}
//
//	else if (button == GLUT_LEFT_BUTTON) {
//
//		if (state == GLUT_DOWN)
//		{
//			if (left_button_pressed == false) {
//				left_button_pressed = true;
//				mouse_prev_x = x;
//				mouse_prev_y = y;
//			}
//		}
//		else if (state == GLUT_UP)
//		{
//			left_button_pressed = false;
//			mouse_dx = 0;
//			mouse_dy = 0;
//		}
//
//	}
//
//	glutPostRedisplay();
//}
//

int mouseButton = -1;
int mousePos[2];
void  mouseClick(int button, int state, int x, int y) {
	mouseButton = button;
	mousePos[0] = x;
	mousePos[1] = y;
}
float* normalize(float* a) {
	float* t = new float[3];
	t[0] = a[0] / sqrt(a[0] * a[0] + a[1] * a[1] + a[2] * a[2]);
	t[1] = a[1] / sqrt(a[0] * a[0] + a[1] * a[1] + a[2] * a[2]);
	t[2] = a[2] / sqrt(a[0] * a[0] + a[1] * a[1] + a[2] * a[2]);
	return t;
}
float* cross_product(float* a, float* b) {
	float* t = new float[3];
	t[0] = a[1] * b[2] - b[1] * a[2];
	t[1] = a[2] * b[0] - b[2] * a[0];
	t[2] = a[0] * b[1] - b[0] * a[1];
	return t;
}
void mouseMove(int x, int y) {
	if (mouseButton == GLUT_LEFT_BUTTON) {
		float* t1 = new float[3]{ -eyePosition[0], -eyePosition[1], -eyePosition[2] };
		float* t2 = normalize(cross_product(up, t1));
		float t3[3];
		t3[0] = eyePosition[0] + t2[0] * (x - mousePos[0]) / 1000;
		t3[1] = eyePosition[1] + t2[1] * (x - mousePos[0]) / 1000;
		t3[2] = eyePosition[2] + t2[2] * (x - mousePos[0]) / 1000;
		t2 = normalize(t3);
		eyePosition[0] = t2[0] * 5;
		eyePosition[1] = t2[1] * 5;
		eyePosition[2] = t2[2] * 5;

		t1 = new float[3]{ -eyePosition[0], -eyePosition[1], -eyePosition[2] };
		t2 = normalize(cross_product(up, t1));
		t3[0] = eyePosition[0] + up[0] * (y - mousePos[1]) / 1000;
		t3[1] = eyePosition[1] + up[1] * (y - mousePos[1]) / 1000;
		t3[2] = eyePosition[2] + up[2] * (y - mousePos[1]) / 1000;
		t2 = normalize(cross_product(t1, t2));
		up[0] = t2[0];
		up[1] = t2[1];
		up[2] = t2[2];

		t2 = normalize(t3);
		eyePosition[0] = t2[0] * 5;
		eyePosition[1] = t2[1] * 5;
		eyePosition[2] = t2[2] * 5;


	}
	if (mouseButton == GLUT_RIGHT_BUTTON) {
		scale += 1.0 * (mousePos[1] - y) / 20000;
	}
	glutPostRedisplay();
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(screenSize, screenSize);
	glutInitWindowPosition(100, 100);
	glutCreateWindow(argv[0]);
	init();

	//get_vertex_face();
	//calculate_normal_vertex();

	glutDisplayFunc(display);
	glutIdleFunc(idle);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouseClick);
	glutMotionFunc(mouseMove);

	glutMainLoop();
	return 0;
}