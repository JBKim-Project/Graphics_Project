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
#include <time.h>
#include <GL/glew.h>

 //
 // Definitions
 //

GLfloat Ipos[4] = { 0,0,10,0 };
GLfloat diffuse[] = { 1,1,1,0 };
GLfloat specular[] = { 1,1,1,0 };
GLfloat ambient[] = { 1,1,1,0 };

//float time = 0;
int check = 0;
//int checkx = 0, checky = 0, checkz = 0;

float timex = 0, timey = 0, timez = 0;

int p = 1;

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
};

struct SphereComponent SC[20];

int mouse_prev_x = 0, mouse_prev_y = 0;
int mouse_dx = 0, mouse_dy = 0;

bool left_button_pressed = false;
bool middle_button_pressed = false;

float anglex = 0, angley = 0, anglez = 0;
float zoom = 1.35;
float positionx = 0.21, positiony = -0.34, positionz = -0.12;

GLfloat width, height;

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
#ifdef DYNAMIC_CUBEMAP
	//RGBA8 Cubemap texture, 24 bit depth texture, 256x256
#endif
}

void idle()
{
	// do something for animation here b/c this will be called when idling

	glutPostRedisplay();
}

void convertDirection(int i)
{
	if (SC[i].startPositionx + SC[i].directionx < -1)
		SC[i].checkx = 1;
	else if (SC[i].startPositionx + SC[i].directionx > 1)
		SC[i].checkx = 0;

	if (SC[i].checkx == 1)
		SC[i].directionx += SC[i].speedx;
	else if (SC[i].checkx == 0)
		SC[i].directionx -= SC[i].speedx;

	if (SC[i].startPositiony + SC[i].directiony < -1)
		SC[i].checky = 1;
	else if (SC[i].startPositiony + SC[i].directiony > 1)
		SC[i].checky = 0;
\
	if (SC[i].checky == 1)
		SC[i].directiony += SC[i].speedy;
	else if (SC[i].checky == 0)
		SC[i].directiony -= SC[i].speedy;

	if (SC[i].startPositionz + SC[i].directionz < -1)
		SC[i].checkz = 1;
	else if (SC[i].startPositionz + SC[i].directionz > 1)
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

		SC[i].speedx = float((rand() % 10)) / 1000;
		SC[i].speedy = float((rand() % 10)) / 1000;
		SC[i].speedz = float((rand() % 10)) / 1000;

	}
}

void Make_Sphere(int i)
{
	glPushMatrix();
	SettingPositionandDirection(i);
	convertDirection(i);
	glTranslatef(SC[i].startPositionx + SC[i].directionx, SC[i].startPositiony + SC[i].directiony, SC[i].startPositionz + SC[i].directionz);
	glTranslatef(0, float(i)/5, 1);
	glutSolidSphere(0.15f, 100, 100);
	
	glPopMatrix();

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
		gluLookAt(0, 0.0, 5.0,
			0.0, 0.0, 0.0,
			0.0f, 1.0f, 0.0f);
		glEnable(GL_TEXTURE_2D);
		glPushMatrix();
		glTranslatef(0, 0, 0);
		//glScalef(zoom, zoom, zoom);
		//glRotatef(anglex, 1.0f, 0.0f, 0.0f);
		//glRotatef(angley, 0.0f, 1.0f, 0.0f);

		CreateCube();
		glPopMatrix();


		for (int i = 0; i < 20; i++)
			Make_Sphere(i);

		/*
		glPushMatrix();

		//glTranslatef(0, time, time);
		convertDirection(positionx + timex, positiony + timey, positionz + timez);
		glTranslatef(positionx + timex, positiony + timey, positionz + timez);
		glColor3f(1, 1, 1);
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
		//printf("time : %f\n", time);
		*/
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

}

void mousemotion(int x, int y)
{
	printf("x: %d\n", x);
	printf("y: %d\n", y);

	mouse_dx = x - mouse_prev_x;
	mouse_dy = y - mouse_prev_y;

	mouse_prev_x = x;
	mouse_prev_y = y;

	if (left_button_pressed == true) {
		anglex += (float)mouse_dy;
		angley -= (float)mouse_dx;
	}
	else if (middle_button_pressed == true) {
		positionx += mouse_dx * 0.006;
		positiony -= mouse_dy * 0.006;
	}

	glutPostRedisplay();

}

void mousebutton(int button, int state, int x, int y)
{
	if (button == GLUT_RIGHT_BUTTON)
	{
		if (state == GLUT_DOWN)
			zoom -= 0.1;

	}
	else if (button == GLUT_MIDDLE_BUTTON)
	{
		if (state == GLUT_DOWN)
		{
			if (middle_button_pressed == false) {
				middle_button_pressed = true;
				mouse_prev_x = x;
				mouse_prev_y = y;
			}
		}

		else if (state == GLUT_UP)
		{
			middle_button_pressed = false;
			mouse_dx = 0;
			mouse_dy = 0;
		}
	}

	else if (button == GLUT_LEFT_BUTTON) {

		if (state == GLUT_DOWN)
		{
			if (left_button_pressed == false) {
				left_button_pressed = true;
				mouse_prev_x = x;
				mouse_prev_y = y;
			}
		}
		else if (state == GLUT_UP)
		{
			left_button_pressed = false;
			mouse_dx = 0;
			mouse_dy = 0;
		}

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
	glutDisplayFunc(display);
	glutIdleFunc(idle);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mousebutton);
	glutMotionFunc(mousemotion);


	glutMainLoop();
	return 0;
}