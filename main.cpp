#include <iostream>
#include <object.h>			// Own library "./Include/object.h"
#include <vector.h>			// Own library "./Include/vector.h"
#include <glut.h>			// Own library "./Include/glut.h"

object objs;

POINT cp1, cp2;

bool clicked = false;

float zoom = -60;
float actualzoom;

float x = 60, y = 30;
float actualx, actualy;
bool wireframe = false;

void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	// BACKGROUND COLOR using of QUADS

	glBegin(GL_QUADS);
	{
		glColor3f(23.0f/255.0f, 23.0f/255.0f, 23.0f/255.0f);
		glVertex3f(-200000,-29000, -90000);
		glVertex3f( 200000,-29000, -90000);
		glColor3f(110.000f/255.0f, 131.0f/255.0f, 146.0f/255.0f);
		glVertex3f( 200000, 29000, -90000);
		glVertex3f(-200000, 29000, -90000);
	}
	glEnd();

	glColor3f(0, 0, 0);
	glTranslatef(0, 0, actualzoom);
	glRotatef(actualy / 2.0f, 1, 0,0);
	glRotatef(actualx / 2.0f, 0, 1, 0);

	// X, Y, Z lines for reference

	glBegin(GL_LINES);
	{
		glColor4f(1, 0, 0, 0.5f);
		glVertex3f(-100000, 0, 0);
		glVertex3f(100000, 0, 0);

		glColor4f(0, 1, 0, 0.5f);
		glVertex3f(0, -100000, 0);
		glVertex3f(0, 100000, 0);

		glColor4f(0, 0, 1, 0.5f);
		glVertex3f(0, 0, -100000);
		glVertex3f(0, 0, 100000);
	}
	glEnd();

	//objs.setCurrent(0);				// changing active object in library

	objs.renderObject();			// calling render function in from object class

	glutSwapBuffers();
}

void mouse(int button, int state, int x, int y) {
	if (!state && button == GLUT_MIDDLE_BUTTON)clicked = true;
	else clicked = false;

	if (!state && button == GLUT_RIGHT_BUTTON)objs.obj[objs.getCurrent()].wireframe ? objs.obj[objs.getCurrent()].wireframe = false : objs.obj[objs.getCurrent()].wireframe = true;
	if (!state && button == GLUT_LEFT_BUTTON) {
		if (objs.getCurrent() < objs.count() - 1)objs.setCurrent(objs.getCurrent() + 1);
		else objs.setCurrent(0);
	}

	glutPostRedisplay();
}

void mouseWheel(int, int direction, int x, int y) {
	if (direction > 0)zoom /= 1.1f;
	else zoom *= 1.1f;

	glutPostRedisplay();
}

void resize(int w, int h) {
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(35.0f, (GLfloat)w / (GLfloat)h, 0.1f, 100000.0f);
	glMatrixMode(GL_MODELVIEW);
	glClearColor(.2f, .3f, .2f, 1.0f);
}

void timer(int) {
	GetCursorPos(&cp1);

	glutTimerFunc(1000 / 60, timer, 0);

	if (clicked) {
		if (cp2.y == 0)SetCursorPos(cp2.x, GetSystemMetrics(SM_CYSCREEN) - 2);
		if (cp2.y >= GetSystemMetrics(SM_CYSCREEN) - 1)SetCursorPos(cp2.x, 1);
		if (cp2.x == 0)SetCursorPos(GetSystemMetrics(SM_CXSCREEN) - 2, cp2.y);
		if (cp2.x >= GetSystemMetrics(SM_CXSCREEN) - 1)SetCursorPos(1, cp2.y);
	}

	if (clicked) {
		x -= (float)(cp2.x - cp1.x);
		y -= (float)(cp2.y - cp1.y);
	}

	actualzoom += (zoom - actualzoom) / 8.0f;

	actualx += (x - actualx) / 4.0f;
	actualy += (y - actualy) / 4.0f;

	GetCursorPos(&cp2);

	glutPostRedisplay();
}

void keyBoard(unsigned char key, int x, int y) {
	if (key == 27)exit(0);
}

int main(int argc, char* argv[]) {
	objs.addObject("res/StoneFort.obj", 10);
	objs.addObject("res/test_loading.obj", 10);
	objs.addObject("res/drill.obj", 10);
	objs.addObject("res/Piramyda1.obj", 10);
	objs.addObject("res/House2.obj", 1000);

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(600, 600);

	int window = glutCreateWindow("MAIN_WINDOW");
	//glutFullScreen();

	glutDisplayFunc(display);
	glutKeyboardFunc(keyBoard);
	glutMouseFunc(mouse);
	glutMouseWheelFunc(mouseWheel);
	glutTimerFunc(0, timer, 0);
	glutReshapeFunc(resize);

	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);
	//glEnable(GL_NORMALIZE);
	//glEnable(GL_LIGHTING);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glutMainLoop();
}