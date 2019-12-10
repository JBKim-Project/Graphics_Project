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
#include <time.h>
#include <fstream>
//#define MUSIC_ON
#ifdef MUSIC_ON
#pragma comment(lib, "winmm.lib")
#include "Mmsystem.h"
#include "Digitalv.h"
MCI_OPEN_PARMS m_mciOpenParms;
MCI_PLAY_PARMS m_mciPlayParms;
DWORD m_dwDeviceID;
MCI_OPEN_PARMS mciOpen;
MCI_PLAY_PARMS mciPlay;

int dwID;
#endif
using namespace std;
//
// Definitions
//
float eyePosition[3] = { 0,0,1 };
float up[3] = { 0,1,0 };
float scale = 1;

GLfloat Ipos[4] = { 1,1,1,0 };
GLfloat diffuse[] = { 1,1,1,1};
GLfloat specular[] = { 1,1,1,1 };
GLfloat ambient[] = { 1,1,1,1 };

GLfloat diffuse3[] = { -1,-1,-1,1 };


GLfloat Ipos2[4] = { 1,1,1,0 };
GLfloat diffuse2[] = { 1,1,1,1 };
GLfloat specular2[] = { 0,1,0,1 };
GLfloat ambient2[] = { 0,1,0,1 };

GLfloat Ipos4[4] = { 1,1,1,0 };
GLfloat diffuse4[] = { 1,1,1,1 };
GLfloat specular4[] = { 1,0.87,0,1 };
GLfloat ambient4[] = { 1,0.87,0,1 };

GLfloat Ipos8[4] = { 1,1,1,0 };
GLfloat diffuse8[] = { 1,1,1,1 };
GLfloat specular8[] = { 0,0,1,1 };
GLfloat ambient8[] = { 0,0,1,1 };

int time_ = 0;
int check = 0;

int secm;

int mouse_prev_x = 0, mouse_prev_y = 0;
int mouse_dx = 0, mouse_dy = 0;

bool left_button_pressed = false;
bool middle_button_pressed = false;

float anglex = 0, angley = 0, anglez = 0;
float zoom = 1.35;
float positionx = 0, positiony = 0, positionz = 0;

GLfloat width, height;

float* vertex = NULL;
int* face = NULL;
float* normal = NULL;
float* vertexnormal = NULL;
int num_vertex, num_face, zero;

int score;

struct SphereComponent {
	float startPositionx = 2;
	float startPositiony = 0;
	float startPositionz = 0;
	float directionx = 0;
	float directiony = 0;
	float directionz = 0;
	int checkx = 0;
	int checky = 0;
	int checkz = 0;
	float speedx = 0;
	float speedy = 0;
	float speedz = 0;
	bool is_Collision = false;
};

struct SphereComponent SC[100];

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
	ifstream fin("../nv.txt");
	for (int i = 0; i < 34836; i++)
		fin >> vertexnormal[i * 3] >> vertexnormal[i * 3 + 1] >> vertexnormal[i * 3 + 2];
	fin.close();
}


void CreateCube(float size)
{
	glBegin(GL_QUADS);

	glTexCoord2d(0, 0); glVertex3d(-size, -size, size);
	glTexCoord2d(1, 0); glVertex3d(size, -size, size);
	glTexCoord2d(1, 1); glVertex3d(size, size, size);
	glTexCoord2d(0, 1); glVertex3d(-size, size, size);

	glTexCoord2d(0, 0); glVertex3d(-size, -size, -size);
	glTexCoord2d(1, 0); glVertex3d(size, -size, -size);
	glTexCoord2d(1, 1); glVertex3d(size, -size, size);
	glTexCoord2d(0, 1); glVertex3d(-size, -size, size);

	glTexCoord2d(0, 0); glVertex3d(-size, size, size);
	glTexCoord2d(1, 0); glVertex3d(size, size, size);
	glTexCoord2d(1, 1); glVertex3d(size, size, -size);
	glTexCoord2d(0, 1); glVertex3d(-size, size, -size);

	glTexCoord2d(0, 0); glVertex3d(-size, size, -size);
	glTexCoord2d(1, 0); glVertex3d(size, size, -size);
	glTexCoord2d(1, 1); glVertex3d(size, -size, -size);
	glTexCoord2d(0, 1); glVertex3d(-size, -size, -size);

	glTexCoord2d(0, 0); glVertex3d(-size, -size, size);
	glTexCoord2d(1, 0); glVertex3d(-size, size, size);
	glTexCoord2d(1, 1); glVertex3d(-size, size, -size);
	glTexCoord2d(0, 1); glVertex3d(-size, -size, -size);

	glTexCoord2d(0, 0); glVertex3d(size, size, size);
	glTexCoord2d(1, 0); glVertex3d(size, -size, size);
	glTexCoord2d(1, 1); glVertex3d(size, -size, -size);
	glTexCoord2d(0, 1); glVertex3d(size, size, -size);
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

char timerBuffer[6 + 1];
void secToHHMMSS(int secs, char* s, size_t size) {
	int hour, min, sec;
	secm = secs;
	sec = secs % 60;
	min = secs / 60 % 60;
	//hour = secs / 3600;
	sprintf_s(s, size, "%02d:%02d", min, sec);
}

int stopwatch(int onOff) {
	static int oldTime;

	if (onOff == 1) { // Timer on
		oldTime = (int)time(NULL);
		return oldTime;
	}

	if (onOff == 0) { //Timer off
		secToHHMMSS(
			(int)time(NULL) - oldTime,
			timerBuffer,
			sizeof(timerBuffer)
		);
		return (int)time(NULL) - oldTime;

	}

}

// text added
void text(double x, double y)
{
	char text[32];
	char text2[32];
	//sprintf(text, ": %.1f", zoom);
	int sec = stopwatch(0);
	score = 100 * sec;
	sprintf(text, "TIME: %s\n", timerBuffer);
	sprintf(text2, "Score: %d\n", score);
	glColor3f(0.0, 0.0, 0.0);
	glRasterPos2f(x, y);
	for (int i = 0; text[i] != '\0'; i++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, text[i]);
	glColor3f(0.0, 0.0, 0.0);
	glRasterPos2f(x, y - 0.05);
	for (int i = 0; text2[i] != '\0'; i++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, text2[i]);
}


void convertDirection(int i)
{
	if (SC[i].startPositionx + SC[i].directionx < -10)
		SC[i].checkx = 1;
	else if (SC[i].startPositionx + SC[i].directionx > 10)
		SC[i].checkx = 0;

	if (SC[i].checkx == 1)
		SC[i].directionx += SC[i].speedx;
	else if (SC[i].checkx == 0)
		SC[i].directionx -= SC[i].speedx;

	if (SC[i].startPositiony + SC[i].directiony < -10)
		SC[i].checky = 1;
	else if (SC[i].startPositiony + SC[i].directiony > 10)
		SC[i].checky = 0;
	\
		if (SC[i].checky == 1)
			SC[i].directiony += SC[i].speedy;
		else if (SC[i].checky == 0)
			SC[i].directiony -= SC[i].speedy;

	if (SC[i].startPositionz + SC[i].directionz < -10)
		SC[i].checkz = 1;
	else if (SC[i].startPositionz + SC[i].directionz > 10)
		SC[i].checkz = 0;

	if (SC[i].checkz == 1)
		SC[i].directionz += SC[i].speedz;
	else if (SC[i].checkz == 0)
		SC[i].directionz -= SC[i].speedz;

}

void SettingPositionandDirection(int i)
{
	if (SC[i].startPositionx > 1.5)
	{
		if (i % 3 == 0) {
			//srand(time(NULL));
			SC[i].startPositionx = 1;
			SC[i].startPositiony = float((rand() % 100)) / 100;
			SC[i].startPositionz = float((rand() % 100)) / 100;
			printf("%f %f %f\n", SC[i].startPositionx, SC[i].startPositiony, SC[i].startPositionz);
		}
		else if (i % 3 == 1) {
			//srand(time(NULL));
			SC[i].startPositionx = float((rand() % 100)) / 100;
			SC[i].startPositiony = -1;
			SC[i].startPositionz = float((rand() % 100)) / 100;
		}
		else if (i % 3 == 2) {
			//srand(time(NULL));
			SC[i].startPositionx = -1;
			SC[i].startPositiony = float((rand() % 100)) / 100;
			SC[i].startPositionz = float((rand() % 100)) / 100;
		}

		//srand(time(NULL));

		SC[i].directionx = float((rand() % 50)) / 1000;
		SC[i].directiony = float((rand() % 50)) / 1000;
		SC[i].directionz = float((rand() % 50)) / 1000;

		SC[i].checkx = 1;
		SC[i].checky = 1;
		SC[i].checkz = 1;

		SC[i].speedx = float((rand() % 10)) / 200;
		SC[i].speedy = float((rand() % 10)) / 200;
		SC[i].speedz = float((rand() % 10)) / 200;

	}
}

void Make_Sphere(int i)
{
	glPushMatrix();
	SettingPositionandDirection(i);
	convertDirection(i);

	if (i < 40) {
		glEnable(GL_LIGHT1);

		glLightfv(GL_LIGHT1, GL_POSITION, Ipos2);
		glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuse2);
		glLightfv(GL_LIGHT1, GL_SPECULAR, specular2);
		glLightfv(GL_LIGHT1, GL_AMBIENT, ambient2);
	}
	else {
		glEnable(GL_LIGHT5);

		glLightfv(GL_LIGHT5, GL_POSITION, Ipos8);
		glLightfv(GL_LIGHT5, GL_DIFFUSE, diffuse8);
		glLightfv(GL_LIGHT5, GL_SPECULAR, specular8);
		glLightfv(GL_LIGHT5, GL_AMBIENT, ambient8);
	}
	glTranslatef(SC[i].startPositionx + SC[i].directionx, SC[i].startPositiony + SC[i].directiony, SC[i].startPositionz + SC[i].directionz);
	glutSolidSphere(2f, 100, 100);

	if (i < 40) {
		glDisable(GL_LIGHT1);
	}
	else
		glDisable(GL_LIGHT5);
	glPopMatrix();

}

void check_Collision(int i)
{
	float tempx = SC[i].startPositionx + SC[i].directionx - positionx;
	float tempy = SC[i].startPositiony + SC[i].directiony - positiony;
	float tempz = SC[i].startPositionz + SC[i].directionz - positionz;
	if (sqrt(tempx * tempx + tempy * tempy + tempz * tempz) < 0.) {
		SC[i].is_Collision = true;
		exit(0);
	}
}
void display(void)
{
	// update dynamic cubemap per frame
	glEnable(GL_LIGHTING);
	
	

	// render something here...
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(eyePosition[0] + positionx, eyePosition[1] + positiony, eyePosition[2] + positionz,
		positionx, positiony, positionz,
		0.0f, 1.0f, 0.0f);
	glEnable(GL_TEXTURE_2D);
	glPushMatrix();

	glEnable(GL_LIGHT3);

	glLightfv(GL_LIGHT3, GL_POSITION, Ipos);
	glLightfv(GL_LIGHT3, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT3, GL_SPECULAR, specular);
	glLightfv(GL_LIGHT3, GL_AMBIENT, ambient);

	glEnable(GL_LIGHT4);

	glLightfv(GL_LIGHT4, GL_POSITION, Ipos);
	glLightfv(GL_LIGHT4, GL_DIFFUSE, diffuse3);
	glLightfv(GL_LIGHT4, GL_SPECULAR, specular);
	glLightfv(GL_LIGHT4, GL_AMBIENT, ambient);

	glTranslatef(0, 0, 0);
	glScalef(zoom, zoom, zoom);
	
	CreateCube(10.0);

	glDisable(GL_LIGHT3);
	glDisable(GL_LIGHT4);

	glPopMatrix();

	glPushMatrix();

	glLineWidth(4);

	glColor3f(1.0f, 0.0f, 0.0f);
	glBegin(GL_LINE_LOOP);
	glVertex3f(10.0, 0.0, 0.0);
	glVertex3f(-10.0, 0.0, 0.0);
	glEnd();

	glColor3f(0.0f, 1.0f, 0.0f);
	glBegin(GL_LINE_LOOP);
	glVertex3f(0.0, 10.0, 0.0);
	glVertex3f(0.0, -10.0, 0.0);
	glEnd();

	glColor3f(0.0f, 0.0f, 1.0f);
	glBegin(GL_LINE_LOOP);
	glVertex3f(0.0, 0.0, 10.0);
	glVertex3f(0.0, 0.0, -10.0);
	glEnd();

	glPopMatrix();

	//glPushMatrix();

	//glTranslatef(0, time, time + 7);
	//glutSolidSphere(0.2f, 100, 100);
	//glTranslatef(positionx, positiony, 7);
	//glPopMatrix();

	for (int i = 0; i < 40; i++) {
		Make_Sphere(i);
		check_Collision(i);
	}

	for (int i = 40; i < 40 + secm / 5; i++) {
		if (i < 100) {
			Make_Sphere(i);
			check_Collision(i);
		}
	}
	glLoadIdentity();
	gluLookAt(0, 0.0, 3.0,
		0.0, 0.0, 0.0,
		0.0f, 1.0f, 0.0f);
	glPushMatrix();
	text(-0.8, -0.8); // text added
	glPopMatrix();

	glPushMatrix();

	glEnable(GL_LIGHT0);

	glLightfv(GL_LIGHT0, GL_POSITION, Ipos4);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse4);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular4);
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient4);
	//glTranslatef(positionx, positiony, 1);
	glScalef(zoom, zoom, zoom); //scale up down 


	/*GLfloat temp_matrix[16];
	glRotatef(anglex, 1.0f, 0.0f, 0.0f);
	glRotatef(angley, 0.0f, 1.0f, 0.0f);
*/

//	glColor3ub(169, 200, 250);
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
	glDisable(GL_LIGHT0);
	glPopMatrix();

	glFlush();

	glutSwapBuffers();
}

void reshape(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(40.0, (GLfloat)w / (GLfloat)h, 0.1, 300.0);
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
	case 'w':
		positionx -= 0.1 * eyePosition[0];
		positiony -= 0.1 * eyePosition[1];
		positionz -= 0.1 * eyePosition[2];
		break;
	case 's':
		positionx += 0.1 * eyePosition[0];
		positiony += 0.1 * eyePosition[1];
		positionz += 0.1 * eyePosition[2];
		break;
	case '2':

		break;
	case '3':

		break;
	case 'o':

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
		char msg[100];
		wsprintf(msg, TEXT("Your score is %d."), score); //message
		MessageBox(NULL, msg, TEXT("Game Over"), NULL); //Box name
		exit(1);
		break;
	}
	//glutPostRedisplay();
}

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

#ifdef MUSIC_ON
	mciOpen.lpstrElementName = "BGM.mp3"; // 파일 경로 입력
	mciOpen.lpstrDeviceType = "mpegvideo";

	mciSendCommand(NULL, MCI_OPEN, MCI_OPEN_ELEMENT | MCI_OPEN_TYPE,
		(DWORD)(LPVOID)& mciOpen);

	dwID = mciOpen.wDeviceID;

	mciSendCommand(dwID, MCI_PLAY, MCI_DGV_PLAY_REPEAT, // play & repeat
		(DWORD)(LPVOID)& m_mciPlayParms);


	/* mciSendCommandW(dwID, MCI_PAUSE, MCI_NOTIFY,
					(DWORD)(LPVOID)&m_mciPlayParms);     */// Pause

	/* mciSendCommandW(dwID, MCI_RESUME, 0, NULL);       */// resume

	/* mciSendCommandW(dwID, MCI_CLOSE, 0, NULL);        */// stop




#endif


	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(screenSize, screenSize);
	glutInitWindowPosition(100, 100);
	glutCreateWindow(argv[0]);
	init();
	stopwatch(1); // stopwatch ON
	glutSetCursor(GLUT_CURSOR_DESTROY);
	get_vertex_face();
	calculate_normal_vertex();
	glutDisplayFunc(display);
	glutIdleFunc(idle);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouseClick);
	glutMotionFunc(mouseMove);

	glutMainLoop();
	return 0;
}