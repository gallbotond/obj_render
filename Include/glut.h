#ifndef SHL_GLUT_H
#define SHL_GLUT_H

#include <GL/freeglut.h>

class glut
{
public:
	glut(int* argc, char* argv[]);
	~glut();

private:
	char* windiw_title = nullptr;

};

glut::glut(int* argc, char* argv[]) {
	glutInit(argc, argv);
}

glut::~glut() {

}

#endif // !SHL_GLUT_H
