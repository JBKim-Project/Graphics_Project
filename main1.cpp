/*
 * Skeleton code for CSE471 Intro to Computer Graphics
 *
 * Won-Ki Jeong, wkjeong@unist.ac.kr
 */

#include <stdio.h>
#include <fstream>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include <string.h>
#include <math.h>
using namespace std;

#define PI 3.14159

GLuint vertex_buffer, index_buffer, normal_buffer;
string file_OFF;
int vertex_num, face_num;
float* vertices = nullptr;
unsigned int* faces = nullptr;
float* normals = nullptr;
int temp;
GLfloat lightPos[] = { 2, 1, 2, 1 };
GLfloat diffuse[] = { 1, 1, 1, 0 };
GLfloat specular[] = { 1.0, 0.5, 0.2, 0 };
GLfloat ambient[] = { 0.4, 0.2, 0.25, 0 };

bool trackingMouse = false;
float angle = 0;
float lastPos[3] = { 0,0,0 };
float axis[3];



void trackball_ptov(int x, int y, int width, int height,
	float v[3])
{
	float d, a;
	v[0] = (2.0F * x - width) / width;
	v[1] = (height - 2.0F * y) / height;
	d = (float)sqrt(v[0] * v[0] + v[1] * v[1]);
	v[2] = (float)cos((PI / 2.0F) * ((d < 1.0F) ? d : 1.0F));
	a = 1.0F / (float)sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
	v[0] *= a;
	v[1] *= a;
	v[2] *= a;
}

void startMotion(int x, int y)
{
	trackingMouse = true;
	trackball_ptov(x, y, 800, 800 , lastPos);
}

void stopMotion(int x, int y)
{
	trackingMouse = false;
	angle = 0;
}

void mouseMotion(int x, int y)
{
	float curPos[3],
		dx, dy, dz;
		trackball_ptov(x, y, 800, 800, curPos);

	if (trackingMouse)
	{
		dx = curPos[0] - lastPos[0];
		dy = curPos[1] - lastPos[1];
		dz = curPos[2] - lastPos[2];
		if (dx || dy || dz)
		{
			angle = 90.0 * sqrt(dx * dx + dy * dy + dz * dz);
			axis[0] = lastPos[1] * curPos[2] -	lastPos[2] * curPos[1];
			axis[1] = lastPos[2] * curPos[0] -	lastPos[0] * curPos[2];
			axis[2] = lastPos[0] * curPos[1] -	lastPos[1] * curPos[0];
			lastPos[0] = curPos[0];
			lastPos[1] = curPos[1];
			lastPos[2] = curPos[2];
		}
	}
	glutPostRedisplay();
}



void mouseButton(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON) switch (state)
	{
	case GLUT_DOWN:
		startMotion(x, y);
		break;
	case GLUT_UP:
		stopMotion(x, y); 
		break;
	}
}
void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 's' :
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glutPostRedisplay();
		break;
	case 'w' :
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glutPostRedisplay();
		break;
	case 'o' :
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(-1, 1, -1, 1, 1, -1);
		glMatrixMode(GL_MODELVIEW);
		glutPostRedisplay();
		break;
	case 'p' :
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glFrustum(-0.2, 0.2, -0.1, 0.3, 0.2, -0.2);
		glMatrixMode(GL_MODELVIEW);
		glutPostRedisplay();
		break;

	}
}

void reshape(int w, int h)
{
	glLoadIdentity();
	glViewport(0, 0, w, h);
	glOrtho(-0.2, 0.2, -0.1, 0.3, 0.2, -0.2);
}

void renderScene(void)
{
	//glClearColor(0.0, 0.0, 4.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glShadeModel(GL_FLAT);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

	glEnableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
	glVertexPointer(3, GL_FLOAT, 0, (void*)0);

	glEnableClientState(GL_NORMAL_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, normal_buffer);
	glNormalPointer(GL_FLOAT, 0, (void*)0);

	glEnableClientState(GL_INDEX_ARRAY);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);
	glIndexPointer(3, GL_UNSIGNED_INT, (void*)0);


	glRotatef(angle, axis[0], axis[1], axis[2]);

	glDrawElements(GL_TRIANGLES, face_num * 3, GL_UNSIGNED_INT, 0);
	
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_INDEX_ARRAY);
	
//	glColor3f(0, 0, 1);
//	glRectf(30, 30, 70, 70);

//	glutSolidTeapot(10);
	glutSwapBuffers();
}


int main(int argc, char** argv)
{
	ifstream inFile("bunny.off");
	if (!inFile.is_open()) {
		cout << "file open fail" << endl;
		exit(-1);
	}
	inFile >> file_OFF >> vertex_num >> face_num >> temp;

	vertices = new float[3 * vertex_num];
	faces = new unsigned int[3 * face_num];
	for (int i = 0; i < 3 * vertex_num; i++) {
		inFile >> vertices[i];
	}
	for (int i = 0; i < face_num; i++) {
		inFile >> temp >> faces[3 * i] >> faces[3 * i + 1] >> faces[3 * i + 2];
	}

	normals = new float[3 * vertex_num];
	float edge1[3], edge2[3];
	float normalize;
	for (int i = 0; i < 3 * vertex_num; i++) {
		normals[i] = 0;
	}
	float face_normal[3];
	for (int i = 0; i < 3 * face_num; i = i + 3) {
		edge1[0] = vertices[faces[i + 1]] - vertices[faces[i]];
		edge1[1] = vertices[faces[i + 1] + 1] - vertices[faces[i] + 1];
		edge1[2] = vertices[faces[i + 1] + 2] - vertices[faces[i] + 2];

		edge2[0] = vertices[faces[i + 2]] - vertices[faces[i]];
		edge2[1] = vertices[faces[i + 2] + 1] - vertices[faces[i] + 1];
		edge2[2] = vertices[faces[i + 2] + 2] - vertices[faces[i] + 2];

		face_normal[0] = edge1[1] * edge2[2] - edge1[2] * edge2[1];
		face_normal[1] = edge1[2] * edge2[0] - edge1[0] * edge2[2];
		face_normal[2] = edge1[0] * edge2[1] - edge1[1] * edge2[0];
		
		normals[faces[i]] = face_normal[0];
		normals[faces[i] + 1] = face_normal[1];
		normals[faces[i] + 2] = face_normal[2];

		normals[faces[i + 1]] = face_normal[0];
		normals[faces[i + 1] + 1] = face_normal[1];
		normals[faces[i + 1] + 2] = face_normal[2];

		normals[faces[i + 2]] = face_normal[0];
		normals[faces[i + 2] + 1] = face_normal[1];
		normals[faces[i + 2] + 2] = face_normal[2];
	}

	for (int i = 0; i < 3 * vertex_num; i = i + 3) {
		normalize = sqrtf(pow(normals[i], 2) + pow(normals[i + 1], 2) + pow(normals[i + 2], 2));
		normals[i] = normals[i] / normalize;
		normals[i + 1] = normals[i + 1] / normalize;
		normals[i + 2] = normals[i + 2] / normalize;
	}
	

	// init GLUT and create Window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(800, 800);
	glutCreateWindow("CSE471 - Assignment 1");

	glewInit();

	glGenBuffers(1, &vertex_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
	glBufferData(GL_ARRAY_BUFFER, 3 * vertex_num * sizeof(float) , vertices, GL_STATIC_DRAW);

	glGenBuffers(1, &normal_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, normal_buffer);
	glBufferData(GL_ARRAY_BUFFER, 3 * vertex_num * sizeof(float), normals, GL_STATIC_DRAW);

	glGenBuffers(1, &index_buffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3 * face_num * sizeof(unsigned int), faces, GL_STATIC_DRAW);
	
	// register callbacks
	glutReshapeFunc(reshape);
	glutDisplayFunc(renderScene);
	glutKeyboardFunc(keyboard); // keyboard callback
	glutMouseFunc(mouseButton); // Mouse callback
	glutMotionFunc(mouseMotion); //Mouse motion
	// enter GLUT event processing cycle
	glutMainLoop();

	return 1;
}