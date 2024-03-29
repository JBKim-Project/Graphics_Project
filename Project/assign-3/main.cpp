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
#include <MMSystem.h> 
#include <dshow.h>

#pragma comment (lib, "strmiids.lib")
#pragma comment(lib, "Winmm.lib") 

#define NUM_OF_GREEN 100
#define NUM_OF_MAX 120
#define CUBESIZE 40.0
#define RABBIT_SPEED 0.2
#define BALL_SPEED 45
#define BALLSIZE 0.5
#define RABBITSIZE 0.15
#define BONUS_POINT 1000

using namespace std;
//
// Definitions
//
float eyePosition[3] = { 0,0,1 };
float up[3] = { 0,1,0 };
float scale = 1;
int timetemp = 0;

IGraphBuilder* pGraph = NULL;
IMediaControl* pControl = NULL;
IMediaEvent* pEvent = NULL;
HRESULT hr = CoInitialize(NULL);

GLfloat Ipos[4] = { 1,1,1,0 };
GLfloat diffuse[] = { 1,1,1,1 };
GLfloat specular[] = { 1,1,1,1 };
GLfloat ambient[] = { 1,1,1,1 };

GLfloat diffuse3[] = { -1,-1,-1,1 };


GLfloat Ipos2[4] = { 1,1,1,0 };
GLfloat diffuse2[] = { 1,1,1,1 };
GLfloat specular2[] = { 1,1,1,1 };
GLfloat ambient2[] = { 1,1,1,1 };

GLfloat Ipos4[4] = { 1,1,1,0 };
GLfloat diffuse4[] = { 1,1,1,1 };
GLfloat specular4[] = { 1,0.87,0,1 };
GLfloat ambient4[] = { 1,0.87,0,1 };

GLfloat Ipos8[4] = { 1,1,1,0 };
GLfloat diffuse8[] = { 1,1,1,1 };
GLfloat specular8[] = { 0,0,1,1 };
GLfloat ambient8[] = { 0,0,1,1 };

GLfloat Ipos6[4] = { 1,1,1,0 };
GLfloat diffuse6[] = { 1,1,1,1 };
GLfloat specular6[] = { 1,0,0,1 };
GLfloat ambient6[] = { 1,0,0,1 };


int time_ = 0;
int check = 0;

int ms, sec, min;

int mouse_prev_x = 0, mouse_prev_y = 0;
int mouse_dx = 0, mouse_dy = 0;

bool left_button_pressed = false;
bool middle_button_pressed = false;

float anglex = -118.0, angley = 223.0, anglez = 0;
float zoom = 1.35;
float positionx = 0, positiony = 0, positionz = 0;
float btime = 0;
GLfloat width, height;

float* vertex = NULL;
int* face = NULL;
float* normal = NULL;
float* vertexnormal = NULL;
int num_vertex, num_face, zero;

int score = 0;
int sec__ = 0;
bool bonus = false;
bool life_extension = false;
struct SphereComponent {
	float startPositionx = 45;
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
	bool is_eaten = false;
};

struct SphereComponent SC[NUM_OF_MAX];

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
GLuint cube_tex, color_tex, color_tex2;
GLuint fb, depth_rb;

//
// Functions
//
void playBGM()
{
	// Initialize the COM library.

	if (FAILED(hr)) {
		printf("ERROR - Could not initialize COM library");
		//	exit(-1);
	} // Create the filter graph manager and query for interfaces.
	hr = CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER, IID_IGraphBuilder, (void**)& pGraph);
	if (FAILED(hr)) {
		printf("ERROR - Could not create the Filter Graph Manager.");
		//	exit(-1);
	}
	hr = pGraph->QueryInterface(IID_IMediaControl, (void**)& pControl);
	hr = pGraph->QueryInterface(IID_IMediaEvent, (void**)& pEvent);
	// Build the graph.IMPORTANT: Change this string to a file on your system.

	hr = pGraph->RenderFile(L"../NEWBGM.mp3", NULL);

	if (SUCCEEDED(hr)) {
		// Run the graph.
		hr = pControl->Run();
	}
}

void endGame()
{
	char msg[100];
	wsprintf(msg, TEXT("Your score is %d."), score); //message
	MessageBox(NULL, msg, TEXT("Game Over"), NULL); //Box name
	exit(1);
}

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


	LoadBMPFile(&dst, &width, &height, "../wall.bmp"); // this is how to load image
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, &color_tex);
	glBindTexture(GL_TEXTURE_2D, color_tex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, dst);

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

}

void before_init(void)
{
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
	LoadBMPFile(&dst, &width, &height, "../Main.bmp"); // this is how to load image
	glPixelStorei(GL_UNPACK_ALIGNMENT, 2);
	glGenTextures(2, &color_tex);
	glBindTexture(GL_TEXTURE_2D, color_tex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, dst);

#define STATIC_MAP


#ifdef STATIC_MAP
	// create static cubemap from synthetic data
	glGenTextures(2, &cube_tex);
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

#endif
}

void idle()
{
	// do something for animation here b/c this will be called when idling

	glutPostRedisplay();
}

char timerBuffer[6 + 1];
void secToHHMMSS(int secs, char* s, size_t size) {
	int hour, sec_;
	ms = secs;
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
int lifeTime;
float check_time = 0;
bool time_check = true;


// text added
void text(double x, double y)
{
	const int num_of_text = 3;
	char text[num_of_text][32];
	char text_eff[32];

	stopwatch(0);
	score += 1;
	sprintf(text_eff, "           +%d", BONUS_POINT);
	int c = (int)time(NULL);

	if (bonus) {
		if (time_check) {
			check_time = ms;
			time_check = false;
		}
		glRasterPos2f(x, y + 0.05 + btime);
		for (int i = 0; text_eff[i] != '\0'; i++) {
			//	for (int i = 0; i < score / 10; i++)

			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, text_eff[i]);
		}

		if (ms - check_time > 1) { //print during 1 second
			time_check = true;
			bonus = false;
		}
	}
	else
		btime = 0;

	sprintf(text[1], "TIME: %s\n", timerBuffer);
	sprintf(text[0], "Score: %d\n", score);

	timetemp = sec % 10;

	if (sec == 59) {
		sec__ = 0;
		life_extension = true;
	}
	if (min > 0)
	{
		glRasterPos2f(x, y - 0.15);

		if (!life_extension)
		{
			sec__ = sec;
			life_extension = true;
		}

		lifeTime = 20 - (sec - sec__);
		cout << "LT__ : " << lifeTime << endl;

		if (lifeTime == 0) {
			hr = pControl->Stop();
			PlaySound(TEXT("../ENDING.wav"), NULL, SND_ASYNC | SND_FILENAME);
			endGame();
		}
		sprintf(text[2], "Time Left: %d\n", lifeTime);

		for (int j = 0; text[2][j] != '\0'; j++)
		{
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, text[2][j]);
		}
	}
	glColor3f(0.0, 0.0, 0.0);

	glRasterPos2f(x, y - 0.05);
	for (int j = 0; text[0][j] != '\0'; j++)
	{
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, text[0][j]);
	}
	glRasterPos2f(x, y - 0.1);
	for (int j = 0; text[1][j] != '\0'; j++)
	{
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, text[1][j]);
	}

}


void convertDirection(int i)
{
	if (SC[i].startPositionx + SC[i].directionx < -CUBESIZE)
		SC[i].checkx = 1;
	else if (SC[i].startPositionx + SC[i].directionx > CUBESIZE)
		SC[i].checkx = 0;

	if (SC[i].checkx == 1)
		SC[i].directionx += SC[i].speedx;
	else if (SC[i].checkx == 0)
		SC[i].directionx -= SC[i].speedx;

	if (SC[i].startPositiony + SC[i].directiony < -CUBESIZE)
		SC[i].checky = 1;
	else if (SC[i].startPositiony + SC[i].directiony > CUBESIZE)
		SC[i].checky = 0;

	if (SC[i].checky == 1)
		SC[i].directiony += SC[i].speedy;
	else if (SC[i].checky == 0)
		SC[i].directiony -= SC[i].speedy;

	if (SC[i].startPositionz + SC[i].directionz < -CUBESIZE)
		SC[i].checkz = 1;
	else if (SC[i].startPositionz + SC[i].directionz > CUBESIZE)
		SC[i].checkz = 0;

	if (SC[i].checkz == 1)
		SC[i].directionz += SC[i].speedz;
	else if (SC[i].checkz == 0)
		SC[i].directionz -= SC[i].speedz;

	if (i >= NUM_OF_GREEN && ((i % 3 == 1) || (i % 3 == 2))) {
		if (SC[i].startPositionx + SC[i].directionx < -CUBESIZE ||
			SC[i].startPositionx + SC[i].directionx > CUBESIZE ||
			SC[i].startPositiony + SC[i].directiony < -CUBESIZE ||
			SC[i].startPositiony + SC[i].directiony > CUBESIZE ||
			SC[i].startPositionz + SC[i].directionz < -CUBESIZE ||
			SC[i].startPositionz + SC[i].directionz > CUBESIZE)
			SC[i].startPositionx = 45;
	}

}

void SettingPositionandDirection(int i)
{
	if (SC[i].startPositionx > 43)
	{
		if (i % 6 == 0) {
			//srand(time(NULL));
			SC[i].startPositionx = -CUBESIZE;
			SC[i].startPositiony = float(rand() % int(CUBESIZE));
			SC[i].startPositionz = float(rand() % int(CUBESIZE));
		}
		else if (i % 6 == 1) {
			//srand(time(NULL));
			SC[i].startPositionx = float(rand() % int(CUBESIZE));
			SC[i].startPositiony = -CUBESIZE;
			SC[i].startPositionz = float(rand() % int(CUBESIZE));
		}
		else if (i % 6 == 2) {
			//srand(time(NULL));
			SC[i].startPositionx = float(rand() % int(CUBESIZE));
			SC[i].startPositiony = float(rand() % int(CUBESIZE));
			SC[i].startPositionz = -CUBESIZE;
		}
		else if (i % 6 == 3) {
			//srand(time(NULL));
			SC[i].startPositionx = CUBESIZE;
			SC[i].startPositiony = float(rand() % int(CUBESIZE));
			SC[i].startPositionz = float(rand() % int(CUBESIZE));
		}
		else if (i % 6 == 4) {
			//srand(time(NULL));
			SC[i].startPositionx = float(rand() % int(CUBESIZE));
			SC[i].startPositiony = CUBESIZE;
			SC[i].startPositionz = float(rand() % int(CUBESIZE));
		}
		else if (i % 6 == 5) {
			//srand(time(NULL));
			SC[i].startPositionx = float(rand() % int(CUBESIZE));
			SC[i].startPositiony = float(rand() % int(CUBESIZE));
			SC[i].startPositionz = CUBESIZE;
		}

		//srand(time(NULL));

		if (i >= NUM_OF_GREEN && ((i % 3 == 1) || (i % 3 == 2))) {
			SC[i].directionx = (positionx - SC[i].startPositionx) / 100;
			SC[i].directiony = (positiony - SC[i].startPositiony) / 100;
			SC[i].directionz = (positionz - SC[i].startPositionz) / 100;
		}
		else {
			SC[i].directionx = float((rand() % 50)) / 1000;
			SC[i].directiony = float((rand() % 50)) / 1000;
			SC[i].directionz = float((rand() % 50)) / 1000;
		}

		SC[i].checkx = 1;
		SC[i].checky = 1;
		SC[i].checkz = 1;


		if (i < NUM_OF_GREEN) {
			SC[i].speedx = float((rand() % 10)) / (1000 / BALL_SPEED);
			SC[i].speedy = float((rand() % 10)) / (1000 / BALL_SPEED);
			SC[i].speedz = float((rand() % 10)) / (1000 / BALL_SPEED);
		}
		else if (i >= NUM_OF_GREEN && ((i % 3 == 1) || (i % 3 == 2))) {
			SC[i].speedx = SC[i].directionx;
			SC[i].speedy = SC[i].directiony;
			SC[i].speedz = SC[i].directionz;
		}
		else {
			SC[i].speedx = float((rand() % 10)) / 100;
			SC[i].speedy = float((rand() % 10)) / 100;
			SC[i].speedz = float((rand() % 10)) / 100;
		}

	}
}

void Make_Sphere(int i)
{
	glPushMatrix();
	SettingPositionandDirection(i);
	convertDirection(i);

	if (i < NUM_OF_GREEN) {
		glEnable(GL_LIGHT1);

		glLightfv(GL_LIGHT1, GL_POSITION, Ipos2);
		glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuse2);
		glLightfv(GL_LIGHT1, GL_SPECULAR, specular2);
		glLightfv(GL_LIGHT1, GL_AMBIENT, ambient2);
		glTranslatef(SC[i].startPositionx + SC[i].directionx, SC[i].startPositiony + SC[i].directiony, SC[i].startPositionz + SC[i].directionz);
		glutSolidSphere(BALLSIZE, 100, 100);
	}
	else if (i >= NUM_OF_GREEN && i % 3 == 0) {
		glEnable(GL_LIGHT5);

		glLightfv(GL_LIGHT5, GL_POSITION, Ipos8);
		glLightfv(GL_LIGHT5, GL_DIFFUSE, diffuse8);
		glLightfv(GL_LIGHT5, GL_SPECULAR, specular8);
		glLightfv(GL_LIGHT5, GL_AMBIENT, ambient8);
		glTranslatef(SC[i].startPositionx + SC[i].directionx, SC[i].startPositiony + SC[i].directiony, SC[i].startPositionz + SC[i].directionz);
		glutSolidTorus(BALLSIZE / 4, BALLSIZE * 0.8, 100, 100);
	}
	else {
		glEnable(GL_LIGHT6);

		glLightfv(GL_LIGHT6, GL_POSITION, Ipos6);
		glLightfv(GL_LIGHT6, GL_DIFFUSE, diffuse6);
		glLightfv(GL_LIGHT6, GL_SPECULAR, specular6);
		glLightfv(GL_LIGHT6, GL_AMBIENT, ambient6);
		glTranslatef(SC[i].startPositionx + SC[i].directionx, SC[i].startPositiony + SC[i].directiony, SC[i].startPositionz + SC[i].directionz);
		glutSolidSphere(BALLSIZE, 100, 100);
	}


	if (i < NUM_OF_GREEN) {
		glDisable(GL_LIGHT1);
	}
	else if (i >= NUM_OF_GREEN && i % 3 == 0) {
		glDisable(GL_LIGHT5);
	}
	else
		glDisable(GL_LIGHT6);
	glPopMatrix();

}


void check_Collision(int i)
{
	float tempx = SC[i].startPositionx + SC[i].directionx - positionx;
	float tempy = SC[i].startPositiony + SC[i].directiony - positiony;
	float tempz = SC[i].startPositionz + SC[i].directionz - positionz;
	if (((sqrt(tempx * tempx + tempy * tempy + tempz * tempz)) < RABBITSIZE + BALLSIZE) &&
		((i < NUM_OF_GREEN) || ((i >= NUM_OF_GREEN) && ((i % 3 == 1) || (i % 3 == 2))))) {
		hr = pControl->Stop();
		PlaySound(TEXT("../ENDING.wav"), NULL, SND_ASYNC | SND_FILENAME);
		endGame();
	}
	else if (sqrt(tempx * tempx + tempy * tempy + tempz * tempz) < RABBITSIZE + BALLSIZE && i >= NUM_OF_GREEN && (i % 3 == 0))
	{
		PlaySound(TEXT("../bbok.wav"), NULL, SND_SYNC | SND_FILENAME);
		score += 1000;
		SC[i].startPositionx = 45;
		bonus = true;
		if (min > 0) { // over the 1 min
			life_extension = false;
		}
	}
}

void CreateMap(float size)
{
	glBegin(GL_QUADS);

	glTexCoord2d(0, 0); glVertex3d(-size, -size, size);
	glTexCoord2d(1, 0); glVertex3d(size, -size, size);
	glTexCoord2d(1, 1); glVertex3d(size, size, size);
	glTexCoord2d(0, 1); glVertex3d(-size, size, size);

	glEnd();
}
bool before_start = true;
bool first = true;
bool first_ = true;
bool speed_check = true;

void display(void)
{
	if (speed_check && min > 0) {
		for (int i = 0; i < NUM_OF_GREEN; i++)
		{
			SC[i].speedx = float((rand() % 10)) / (1000 / 200);
			SC[i].speedy = float((rand() % 10)) / (1000 / 200);
			SC[i].speedz = float((rand() % 10)) / (1000 / 200);
		}
		speed_check = false;
	}

	if (before_start) {
		if (first) {
			before_init();
			first = false;
		}

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		gluLookAt(0, 0, 50,
			0, 0, 0,
			0.0f, 1.0f, 0.0f);
		glEnable(GL_TEXTURE_2D);
		glPushMatrix();


		glTranslatef(0, 0, 0);
		glScalef(zoom, zoom, zoom);

		CreateMap(10);
		glPopMatrix();

		glFlush();

		glutSwapBuffers();
	}
	else {
		// update dynamic cubemap per frame

		glEnable(GL_LIGHTING);
		if (first_) {
			init();
			stopwatch(1); // stopwatch ON
			first_ = false;

		}


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

		CreateCube(CUBESIZE);

		glDisable(GL_LIGHT3);
		glDisable(GL_LIGHT4);

		glPopMatrix();

		for (int i = 0; i < NUM_OF_GREEN; i++) {
			Make_Sphere(i);
			check_Collision(i);
		}

		for (int i = NUM_OF_GREEN; i < NUM_OF_GREEN + ms / 5; i++) {
			if (i < NUM_OF_MAX) {
				Make_Sphere(i);
				check_Collision(i);
			}
		}

		btime = btime + 0.001;

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

		glScalef(zoom, zoom, zoom); //scale up down 
		glTranslatef(0, -0.1, 0);
		glRotatef(anglex, 0.0f, 1.0f, 0.0f);

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

bool boundary(float b)
{
	if (b > CUBESIZE - 5 || b < -CUBESIZE + 5)
		return false;
	return true;
}

void bangbang(void)
{
	if (!boundary(positionx) && positionx > 0)
		positionx = CUBESIZE - 5.1;
	else if (!boundary(positionx) && positionx < 0)
		positionx = -CUBESIZE + 5.1;
	if (!boundary(positiony) && positiony > 0)
		positiony = CUBESIZE - 5.1;
	else if (!boundary(positiony) && positiony < 0)
		positiony = -CUBESIZE + 5.1;
	if (!boundary(positionz) && positionz > 0)
		positionz = CUBESIZE - 5.1;
	else if (!boundary(positionz) && positionz < 0)
		positionz = -CUBESIZE + 5.1;
}

void keyboard(unsigned char key, int x, int y)
{

	switch (key)
	{
	case 'w':
		if (boundary(positionx) && boundary(positiony) && boundary(positionz)) {
			positionx -= RABBIT_SPEED * eyePosition[0];
			positiony -= RABBIT_SPEED * eyePosition[1];
			positionz -= RABBIT_SPEED * eyePosition[2];
		}
		else
			bangbang();
		break;
	case 's':
		if (boundary(positionx) && boundary(positiony) && boundary(positionz)) {
			positionx += RABBIT_SPEED * eyePosition[0];
			positiony += RABBIT_SPEED * eyePosition[1];
			positionz += RABBIT_SPEED * eyePosition[2];
		}
		else
			bangbang();
		break;
	case '1':
		before_start = true;
		break;
	case 13:
		before_start = false;
		break;

	case 27:
		endGame();
		break;
	}
}

int mouseButton = -1;
int mousePos[2];
void  mouseClick(int button, int state, int x, int y) {
	mouseButton = button;
	mousePos[0] = x;
	mousePos[1] = y;

	if (button == GLUT_LEFT_BUTTON) {
		if (state == GLUT_DOWN)
		{
			if (left_button_pressed == false) {
				left_button_pressed = true;
				mouse_prev_x = x;
				mouse_prev_y = y;
			}
		}
		else if (state == GLUT_UP) {
			left_button_pressed = false;
			mouse_dx = 0;
			mouse_dy = 0;
			anglex = -118.0;
		}

	}

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

	mouse_dx = x - mouse_prev_x;
	mouse_dy = y - mouse_prev_y;

	mouse_prev_x = x;
	mouse_prev_y = y;

	if (left_button_pressed == true) {
		anglex -= (float)mouse_dx / 10;
		angley += (float)mouse_dy;
	}
	else
		anglex = 223;

	glutPostRedisplay();
}

int main(int argc, char** argv)
{
	playBGM();
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(screenSize, screenSize);
	glutInitWindowPosition(100, 100);
	glutCreateWindow(argv[0]);

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