/*
 * Skeleton code for CSE471 Intro to Computer Graphics
 *
 * Won-Ki Jeong, wkjeong@unist.ac.kr
 */


#include <stdio.h>
#include <GL/glew.h>
#include <GL/glut.h>
#include <math.h>

float* vertex	= NULL;
int* face		= NULL;
float* normal	= NULL;
float* vertexnormal = NULL;

float anglex = 0, angley = 0, anglez = 0;
float zoom = 1;
float positionx = 0, positiony = 0, positionz = 0;

int num_vertex, num_face, zero;

float window_width;
float window_height;

int mouse_prev_x = 0, mouse_prev_y = 0;
int mouse_dx = 0, mouse_dy = 0;

bool left_button_pressed = false;
bool middle_button_pressed = false;

void init_light1();
void init_light2();
void light_off();

void reshape(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(40.0, (GLfloat)w / (GLfloat)h, 1.0, 300.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	//glTranslatef(0.0, 0.0, -20.0);

	/*
	glLoadIdentity();
	window_width = w;
	window_height = h;
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-1, 1, -1, 1, -10, 10);
	glTranslatef(0, 0, 3);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	*/
}

void renderScene(void) 
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	
	//glMatrixMode(GL_PROJECTION);
	//glLoadIdentity();

	//glOrtho(-0.2, 0.2, -0.2, 0.2, -0.2, 0.2);
	glPushMatrix();
	glTranslatef(positionx, positiony, positionz);
	glScalef(zoom, zoom, zoom); //scale up down 
	//glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
	
	
	//GLuint buffer;
	//glGenVertexArrays(1, &buffer);
	//glBindVertexArray(buffer);

	//gluLookAt(0, 1, 1, 0, 0, 0, 0, 1, 0);
	
	GLfloat temp_matrix[16];
	glRotatef(anglex, 1.0f, 0.0f, 0.0f);
	glRotatef(angley, 0.0f, 1.0f, 0.0f);
	
	//glGetFloatv(GL_MODELVIEW_MATRIX, temp_matrix);
	glColor3ub(169, 200, 250);
	GLuint vertexbuffer;
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER,  num_vertex * 3 * 4 * 2, 0, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, num_vertex * 3 * 4, vertex);
	glBufferSubData(GL_ARRAY_BUFFER, num_vertex * 3 * 4, num_vertex * 3 * 4, vertexnormal);
	//glBindBuffer(GL_ARRAY_BUFFER, 0);

	GLuint indexbuffer;
	glGenBuffers(1, &indexbuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexbuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, num_face * 3 * 4, face, GL_STATIC_DRAW);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

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

	glLoadIdentity();
	glPushMatrix();
	glTranslatef(2.0, 2.0, 0);
	glColor3f(1.0, 1.0, 0.0);
	glutSolidTeapot(0.05);
	
	glPopMatrix();

	glutSwapBuffers();
}

void idleFunc()
{
	glutPostRedisplay();
}

void prespective() {


	/*
	glViewport(0, 0, window_width, window_height);
	gluLookAt(-10, -10, -10, 10, 10, 10, 0, 1, 0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	GLfloat v = (GLfloat)window_height / (GLfloat)window_width;

	if (window_width <= window_height)
	{
		glFrustum(-0.5, 0.5, -0.5 * v, 0.5 / v, 0.5, 20.0);
	}
	else
	{
		glFrustum(-0.5, 0.5, -0.5 * v, 0.5 / v, 0.5, 20.0);
	}

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glutPostRedisplay();
	*/
}

void orthogonal() {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glOrtho(-10, 10, -10, 10, -10, 10);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glutPostRedisplay();

}

void smoothshading(void)
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void wireframe(void)
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'p':
		prespective();
		break;
	case 'o':
		orthogonal();
		break;
	case 's':
		smoothshading();
		break;
	case 'w':
		wireframe();
	case 'm':
		positionx+=0.01;
		break;
	case 'n':
		positiony-=0.01;
		break;
	case 'b':
		positionz+=0.01;
		break;
	case 'l':
		init_light1();
		break;
	case 'k':
		init_light2();
		break;
	case 'f':
		light_off();
	default:
		break;
	}
}

void mousebutton(int button, int state, int x, int y)
{
	if (button == GLUT_RIGHT_BUTTON)
	{
		if (state == GLUT_DOWN)
			zoom += 0.1;
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
		angley += (float)mouse_dx;
	}
	else if (middle_button_pressed == true) {
		positionx += mouse_dx*0.002;
		positiony -= mouse_dy*0.002;
	}
	
	glutPostRedisplay();

}


void get_vertex_face(void)
{
	FILE* MeshFile = NULL;
	MeshFile = fopen("../../mesh-data/bunny.off", "r");

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

void init_light1(void)
{
	GLfloat lightPos[] = { -200, 0, 200, 1 };
	GLfloat diffuse[] = { 0.9,0.0,0.0,1 };
	GLfloat specular[] = { 1,1,1,1 };
	GLfloat ambient[] = { 1,1,1,1 };
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
}

void init_light2(void)
{
	GLfloat lightPos[] = { 200, 0, 200, 1 };
	GLfloat diffuse[] = { 0.0,0.9,0.0,1 };
	GLfloat specular[] = { 1,1,1,1 };
	GLfloat ambient[] = { 1,1,1,1 };
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT1);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT1, GL_SPECULAR, specular);
	glLightfv(GL_LIGHT1, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT1, GL_POSITION, lightPos);
}

void light_off(void)
{
	glDisable(GL_LIGHTING);
	glDisable(GL_LIGHT0);
	glDisable(GL_LIGHT1);
}


int main(int argc, char **argv) 
{
	// init GLUT and create Window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(800,800);
	glutCreateWindow("CSE471 - Assignment 1");

	glClearColor(1.0, 1.0, 1.0, 0.0);

	glEnable(GL_DEPTH_TEST);
	glClearDepth(1.0);

	glewInit();

	get_vertex_face();
	calculate_normal_vertex();
	init_light1();
	
	// register callbacks
	glutReshapeFunc(reshape);
	glutDisplayFunc(renderScene);
	glutIdleFunc(idleFunc);

	glutKeyboardFunc(keyboard);
	glutMouseFunc(mousebutton);
	glutMotionFunc(mousemotion);


	// enter GLUT event processing cycle
	glutMainLoop();

	return 1;
}

