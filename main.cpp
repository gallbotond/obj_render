#include <GL/freeglut.h>	// Freeglut (glut library)
#include <iostream>
#include <object.h>			// Own library "./Include/object.h"
#include <vector.h>			// Own library "./Include/vector.h"

object objs;

POINT cp1, cp2;

bool clicked = false;

float zoom = -60;

float x = 0, y = 0;
bool wireframe = false;

void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	if (clicked) {
		x += (float)((float)cp2.x - (float)cp1.x);
		y += (float)((float)cp2.y - (float)cp1.y);
	}

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
	glTranslatef(0, 0, zoom);
	glRotatef(y / 2.0f, 1, 0,0);
	glRotatef(x / 2.0f, 0, 1, 0);

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

	for (unsigned long i = 0; i < objs.count(); i++) {
		int m = 0;
		for (unsigned long j = 0; j < objs.obj[i].f.size(); j++) {
			
			if (!wireframe) {
				glBegin(GL_POLYGON);
				glColor3f(j/100000.0f, j/100000.0f, j/100000.0f);
			}
			else {
				glBegin(GL_LINE_LOOP);
				glColor3f(0, 0, 1);
			}
			{
				for (unsigned long k = 0; k < objs.obj[i].f[j].n.size(); k++) {
					glVertex3f(objs.obj[i].v[objs.obj[i].f[j].n[k][0]].x, objs.obj[i].v[objs.obj[i].f[j].n[k][0]].y, objs.obj[i].v[objs.obj[i].f[j].n[k][0]].z);
				}
			}glEnd();
		}
	}

	glutSwapBuffers();

	GetCursorPos(&cp1);
	Sleep(1000 / 60);
	GetCursorPos(&cp2);
	if (clicked) {
		if (cp2.y == 0)SetCursorPos(cp2.x, GetSystemMetrics(SM_CYSCREEN) - 1);
		if (cp2.y >= GetSystemMetrics(SM_CYSCREEN) - 1)SetCursorPos(cp2.x, 1);
		if (cp2.x == 0)SetCursorPos(GetSystemMetrics(SM_CXSCREEN) - 1, cp2.y);
		if (cp2.x >= GetSystemMetrics(SM_CXSCREEN) - 1)SetCursorPos(1, cp2.y);
	}

	glutPostRedisplay();
}

void mouse(int button, int state, int x, int y) {
	if (!state && button == GLUT_MIDDLE_BUTTON)clicked = true;
	else clicked = false;

	if (!state && button == GLUT_RIGHT_BUTTON)wireframe ? wireframe = false : wireframe = true;

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

int main(int argc, char* argv[]) {
	objs.addObject("res/drill.obj");

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(800, 600);

	int window = glutCreateWindow("MAIN_WINDOW");

	glutDisplayFunc(display);
	glutMouseFunc(mouse);
	glutMouseWheelFunc(mouseWheel);
	glutReshapeFunc(resize);

	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);
	//glEnable(GL_NORMALIZE);
	//glEnable(GL_LIGHTING);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glutMainLoop();
}