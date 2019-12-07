/*
 * Skeleton code for CSE471 Intro to Computer Graphics
 *
 * Won-Ki Jeong, wkjeong@unist.ac.kr
 */

#include <stdio.h>
#include <GL/glew.h>
#include <GL/glut.h>


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

void reshape(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(40.0, (GLfloat)w / (GLfloat)h, 1.0, 300.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0.0, 0.0, -20.0);
}

void renderScene(void) 
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Draw something here!
	glPushMatrix();
	glTranslatef(positionx, positiony, positionz);
	glRotatef(anglex, 1.0f, 0.0f, 0.0f);
	glRotatef(angley, 0.0f, 1.0f, 0.0f);
	glColor3f(0, 0, 1);
	glutSolidTeapot(1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(2,2,2);
	glRotatef(anglex, 1.0f, 0.0f, 0.0f);
	glRotatef(angley, 0.0f, 1.0f, 0.0f);
	glColor3f(0, 0, 1);
	glutSolidTeapot(1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-2, -2, -2);
	glRotatef(anglex, 1.0f, 0.0f, 0.0f);
	glRotatef(angley, 0.0f, 1.0f, 0.0f);
	glColor3f(0, 0, 1);
	glutSolidTeapot(1);
	glPopMatrix();

    glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'm':
		positionx += 0.1;
		break;
	case 'n':
		positiony -= 0.1;
		break;
	case 'b':
		positionz += 0.1;
		break;
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
		positionx += mouse_dx * 0.002;
		positiony -= mouse_dy * 0.002;
	}

	glutPostRedisplay();

}

void idleFunc()
{
	glutPostRedisplay();
}

int main(int argc, char **argv) 
{
	// init GLUT and create Window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(800,800);
	glutCreateWindow("CSE471 - Assignment 1");

	glewInit();

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