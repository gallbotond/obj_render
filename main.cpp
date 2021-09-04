#include <iostream>
#include <object.h>			// Own library "./Include/object.h"
#include <vector.h>			// Own library "./Include/vector.h"
#include <glut.h>			// Own library "./Include/glut.h"

object objs;

POINT cp1, cp2;

struct MOUSE {
	bool LMB = false, MMB = false, RMB = false;
};

vec3 angle = { 60, 40, 0 }, translate = { 0, 0, -60 }, actual_angle, actual_translate;

MOUSE clicked;
bool editMode = true;

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
	glTranslatef(actual_translate.x, actual_translate.y, actual_translate.z);
	glRotatef(actual_angle.y / 2.0f, 1, 0,0);
	glRotatef(actual_angle.x / 2.0f, 0, 1, 0);

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
	if (!state && button == GLUT_RIGHT_BUTTON && editMode)clicked.RMB = true;
	else clicked.RMB = false;

	if (!state && button == GLUT_MIDDLE_BUTTON && editMode)clicked.MMB = true;
	else clicked.MMB = false;

	if (!state && button == GLUT_LEFT_BUTTON && editMode) {
		if (objs.getCurrent() < objs.count() - 1)objs.setCurrent(objs.getCurrent() + 1);
		else objs.setCurrent(0);
	}

	glutPostRedisplay();
}

void mouseWheel(int, int direction, int x, int y) {
	if (editMode)
		if (direction > 0)translate.z /= 1.1f;
		else translate.z *= 1.1f;

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

	if (editMode) {
		if (clicked.RMB) {
			if (cp2.y == 0)SetCursorPos(cp2.x, GetSystemMetrics(SM_CYSCREEN) - 2);
			if (cp2.y >= GetSystemMetrics(SM_CYSCREEN) - 1)SetCursorPos(cp2.x, 1);
			if (cp2.x == 0)SetCursorPos(GetSystemMetrics(SM_CXSCREEN) - 2, cp2.y);
			if (cp2.x >= GetSystemMetrics(SM_CXSCREEN) - 1)SetCursorPos(1, cp2.y);
			angle.x -= (float)(cp2.x - cp1.x);
			angle.y -= (float)(cp2.y - cp1.y);
		}

		if (clicked.MMB) {
			translate.x += (float)(cp2.x - cp1.x) / 10.0f * (actual_translate.z / 100.0f);
			translate.y -= (float)(cp2.y - cp1.y) / 10.0f * (actual_translate.z / 100.0f);
		}

		actual_translate.x += (translate.x - actual_translate.x) / 4.0f;
		actual_translate.y += (translate.y - actual_translate.y) / 4.0f;
		actual_translate.z += (translate.z - actual_translate.z) / 8.0f;

		actual_angle.x += (angle.x - actual_angle.x) / 4.0f;
		actual_angle.y += (angle.y - actual_angle.y) / 4.0f;
	}
	else {
		if ((int)actual_translate.z != (int)0) {
			actual_translate.z -= (actual_translate.z) / 5.0f;
		}
	}

	GetCursorPos(&cp2);

	glutPostRedisplay();
}

void keyBoard(unsigned char key, int x, int y) {
	if (key == 27)exit(0);
	if (key == 'p') {
		editMode ? glutSetCursor(GLUT_CURSOR_NONE) : glutSetCursor(GLUT_CURSOR_INHERIT);
		editMode ? editMode = false : editMode = true;
	}
	if(key == 'w')objs.obj[objs.getCurrent()].wireframe ? objs.obj[objs.getCurrent()].wireframe = false : objs.obj[objs.getCurrent()].wireframe = true;
	if (key == '.') {
		angle.x = 60;
		angle.y = 40;
		angle.z = 0;
		translate.x = 0;
		translate.y = 0;
		translate.z = -60;
	}
}

int main(int argc, char* argv[]) {
	objs.addObject("res/StoneFort.obj", 10);
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